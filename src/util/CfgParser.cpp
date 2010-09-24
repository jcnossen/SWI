#include "StdIncl.h"

#include "CfgParser.h"
#include "InputBuffer.h"

#include <algorithm>

CfgList* CfgList::EmptyList = new CfgList();

#define CT_IDENT	1
#define CT_LIST		2
#define CT_LITERAL	3
#define CT_NUMERIC	4
#define CT_COMMAND	5
#define CT_COMMA	6
#define CT_REFERENCE 7

static int nextToken (InputBuffer& buf) {
	buf.skipWhitespace();

	char r = *buf;
	if (r == '@')  // special command
		return CT_COMMAND;
	else if(isalpha (r))
		return CT_IDENT;
	else if(isdigit (r) || r == '.' || r == '-')
		return CT_NUMERIC;
	else if(r == '"')
		return CT_LITERAL;
	else if(r == '{')
		return CT_LIST;
	else if(r == ',')
		return CT_COMMA;
	else if(r == '$')
		return CT_REFERENCE;

	throw ContentException(SPrintf("Unexpected character '%c'. ", r) + buf.location());
}

//-------------------------------------------------------------------------
// CfgWriter - "outputdevice" for the config value nodes
//-------------------------------------------------------------------------

CfgWriter::CfgWriter (const char *name)
{
	indentLevel =0;

	out = fopen (name, "w");
}

CfgWriter::~CfgWriter()
{
	if (out) {
		fclose (out);
		out = 0;
	}
}

bool CfgWriter::IsFailed() { return out == 0 || ferror(out); }

void CfgWriter::DecIndent () { indentLevel--; }
void CfgWriter::IncIndent () { indentLevel++; }

CfgWriter& CfgWriter::operator <<(char c)
{
	fputc (c,out);
	MakeIndent(c);
	return *this;
}

CfgWriter&  CfgWriter::operator<<(const std::string& s)
{
	fputs (s.c_str(),out);
	if (!s.empty()) MakeIndent(s.at(s.size()-1));
	return *this;
}

CfgWriter& CfgWriter::operator<<(const char* str)
{
	fputs (str,out);
	int l = strlen(str);
	if (l) MakeIndent (str[l-1]);
	return *this;
}

void CfgWriter::MakeIndent(char c)
{
	if (c != 0x0D && c != 0x0A) return;

	for (int a=0;a<indentLevel;a++)
		fputs("  ",out);
}

//-------------------------------------------------------------------------
// CfgValue - base config parsing class
//-------------------------------------------------------------------------


void CfgValue::release()
{
	if (--refCount == 0)
		delete this;
}

void CfgValue::addRef() 
{
	refCount++;
}

void CfgValue::dbgPrint (int depth)
{
	d_trace("??\n");
}

CfgValue* CfgValue::ParseValue (InputBuffer& buf)
{
	CfgValue *v = 0;

	if (buf.skipWhitespace ())
		buf.expecting ("Value");

	// parse value types:
	int token = nextToken (buf);

	switch (token) {
		case CT_LIST:
			v = new CfgList();
			break;
		case CT_NUMERIC:
			v = new CfgNumeric();
			break;
		case CT_LITERAL:
			v = new CfgLiteral();
			break;
		case CT_COMMAND: {
			++buf;
			std::string cmd = buf.parseIdent();
			if (cmd == "include")
				return LoadNestedFile(buf);
			else
				throw ContentException(buf.location() + "Unknown command " + cmd);
		}
		case CT_REFERENCE: {
			v = new CfgReference();
			break;
		}
		case CT_IDENT:
			throw ContentException(buf.location() + SPrintf("Unexpected identifier: '%s'", buf.parseIdent().c_str()) );
	}

	if (v) {
		v->Parse(buf);
		v->cfgFilePath = buf.filename;
	}
	return v;
}


void CfgValue::Write (CfgWriter& w) {}

CfgList* CfgValue::LoadNestedFile (InputBuffer& buf)
{
	std::string s;
	buf.skipKeyword ("@include");

	buf.skipWhitespace ();

	CfgLiteral l;
	l.Parse (buf);
	s = l.value;

	// insert the path of the current file in the string
	int i = strlen (buf.filename)-1;
	while (i > 0) {
		if (buf.filename [i]=='\\' || buf.filename [i] == '/')
			break;
		i--;
	}

	s.insert (s.begin(), buf.filename, buf.filename + i + 1);
	return LoadFile (s.c_str());
}

CfgList* CfgValue::LoadFile (const std::string& name)
{
	InputBuffer buf;
	
	FILE *f = fopen (name.c_str(), "rb");
	if (!f) 
		throw ContentException(SPrintf("Failed to open file %s\n", name.c_str()));

	fseek (f, 0, SEEK_END);
	buf.len = ftell(f);
	buf.data = new char [buf.len];
	fseek (f, 0, SEEK_SET);
	if (!fread (buf.data, buf.len, 1, f))
	{
		fclose (f);
		delete[] buf.data;
		throw ContentException(SPrintf("Failed to read file %s\n", name.c_str()));
	}
	buf.filename = name.c_str();

	fclose (f);

	CfgList *nlist = new CfgList;
	nlist->Parse (buf, true);
	nlist->ResolveReferences();
	delete[] buf.data;
	return nlist;
}


//-------------------------------------------------------------------------
// CfgNumeric - parses int/float/double
//-------------------------------------------------------------------------

void CfgNumeric::Parse (InputBuffer& buf)
{
	bool dot=*buf=='.';
	std::string str;
	str+=*buf;
	++buf;
	while (!buf.end()) {
		char c = *buf;
		if(c=='.') {
			if(dot) break;
			else dot=true;
		}
		if(!c || !(isdigit(c) || c=='.'))
			break;
		str += c;
		++buf;
	}
	if(dot) value = atof (str.c_str());
	else value = atoi (str.c_str());
}

void CfgNumeric::dbgPrint (int depth)
{
	d_trace( "%g\n", value);
}

void CfgNumeric::Write (CfgWriter &w)
{
	CfgValue::Write (w);

	char tmp[20];
	SNPRINTF(tmp,20,"%g", value);
	w << tmp;
}

//-------------------------------------------------------------------------
// CfgLiteral - parses string constants
//-------------------------------------------------------------------------
void CfgLiteral::Parse (InputBuffer& buf)
{
	++buf;
	while (*buf != '\n')
	{
		if(*buf == '\\') {
			if(buf[1] == '"')
				value += buf[1];
			else if (buf[1] == '\\') 
				value += buf[1];
			else {
				throw ContentException(SPrintf("%s: Unknown escape char: '%c' \n", buf.location().c_str(), buf[1]));
			}
			buf.pos += 2;
			continue;
		}

		if(*buf == '"')
			break;

		value += *buf;
		++buf;
	}
	++buf;
}

void CfgLiteral::dbgPrint (int depth)
{
	d_trace( "%s\n", value.c_str());
}

void CfgLiteral::Write (CfgWriter& w)
{
	w << '"' << value << '"';
}

//-------------------------------------------------------------------------
// CfgReference
//-------------------------------------------------------------------------

void CfgReference::Parse(InputBuffer& buf)
{
	assert(buf.get() == '$');
	++buf;
	
	CfgLiteral::Parse(buf);
}
	
//-------------------------------------------------------------------------
// CfgList & CfgListElem - parses a list of values enclosed with { }
//-------------------------------------------------------------------------

void CfgListElem::Parse (InputBuffer& buf)
{
	if (buf.skipWhitespace ())
		throw ContentException("Unexpected end of file in list element: " + buf.location());

	// parse name
	int token = nextToken(buf);
	if (token == CT_COMMA) {
		++buf;
		token = nextToken(buf);
	}
	if (token == CT_IDENT) {
		name = buf.parseIdent();
		buf.skipWhitespace ();

		if (*buf != '=')
			buf.expecting("'=' character");

		++buf;
	}

	value = CfgValue::ParseValue (buf);
	value->addRef();
}

void CfgList::ResolveReferences(CfgList* rootList)
{
	if(!rootList)
		rootList=this;

	for (ChildList::iterator li=childs.begin(); li!=childs.end(); ++li) {

		Type t = li->value->getType();

		if(t == CfgValue::List) {
			((CfgList*)li->value)->ResolveReferences(rootList);
		}
		else if (li->value->getType() == CfgValue::Reference) {
			CfgReference* refItem = (CfgReference*)li->value;
			
			CfgValue* nv = rootList->GetValue(refItem->value.c_str());
			if (!nv) 
				throw ContentException(SPrintf("Invalid reference to item '%s'", refItem->value.c_str()));
			else {
				li->value->release();
				li->value = nv;
				nv->addRef();
			}
		}
	}
}

void CfgListElem::Write (CfgWriter& w)
{
	w << name;

	if (value) {
		w << " = ";
		value->Write (w);
	}
	w << "\n";
}

void CfgList::Parse (InputBuffer& buf, bool root)
{
	if (!root)
	{
		buf.skipWhitespace ();

		if (*buf != '{')
			buf.expecting ("{");

		++buf;
	}

	while (!buf.skipWhitespace ())
	{
		if (*buf == '}') {
			++buf;
			return;
		}

		childs.push_back (CfgListElem());
		childs.back ().Parse (buf);
	}

	if (!root && buf.end())
		throw ContentException(buf.location() + "Unexpected end of node at line\n");
}

void CfgList::Write (CfgWriter &w, bool root)
{
	if (!root) {
		w << '{';
		w.IncIndent (); w << "\n";
	}
	for(CfgList::iterator i = childs.begin(); i != childs.end(); ++i)
		i->Write (w);
	if (!root) {
		w << '}';
		w.DecIndent (); w << "\n";
	}
}

CfgValue* CfgList::Index(int i) {
	for(CfgList::iterator li=childs.begin();li!=childs.end();++li)
		if(i--==0) return li->value;
	return 0;
}

Vector2 CfgList::toVector2()
{
	CfgValue* x=Index(0);
	CfgValue* y=Index(1);
	return Vector2(
		(x&&x->getType()==CfgValue::Numeric?((CfgNumeric*)x)->value:0.0f),
		(y&&y->getType()==CfgValue::Numeric?((CfgNumeric*)y)->value:0.0f)
		);
}

CfgValue* CfgList::GetValue (const char *path)
{
	std::string nodeName;
	int i=0;
	for(;path[i];i++) {
		if (path[i] == '/')
			break;
		nodeName+=path[i];
	}
			
	for(CfgList::iterator li=childs.begin();li!=childs.end();++li)
		if (!STRCASECMP (li->name.c_str(), nodeName.c_str())) {
			
			if (path[i]=='/') {
				std::string subPath(&path[i+1]);
			
				if (li->value->getType() != CfgValue::List)
					throw ContentException(SPrintf("Expecting '%s' to be a list node", nodeName.c_str()));
										   
				return ((CfgList*)li->value)->GetValue(subPath.c_str());
			} else
				return li->value;
		}
	return 0;
}

CfgList* CfgList::GetList(const char *name) {
	CfgValue *val = GetValue(name);
	if (val && val->getType() == CfgValue::List) {
		return (CfgList*)val;
	}

	return 0;
}

double CfgList::GetNumeric (const char *name, double def)
{
	CfgValue *v = GetValue (name);
	if (!v) return def;

	if (v->getType() == CfgValue::Numeric)
		return ((CfgNumeric*)v)->value;

	return def;
}

const char* CfgList::GetLiteral (const char *name, const char *def)
{
	CfgValue *v = GetValue (name);
	if (!v) return def;

	if (v->getType() == CfgValue::Literal)
		return ((CfgLiteral*)v)->value.c_str();

	return def;
}

void CfgList::dbgPrint(int depth)
{
	int n = 0;
	if (depth)
		d_trace("list of %d elements\n", childs.size());

	for (CfgList::iterator i = childs.begin(); i != childs.end(); ++i)
	{
		for (int a=0;a<depth;a++)
			d_trace("  ");
		d_trace("List element(%d): %s", n++, i->name.c_str());
		if (i->value) {
			d_trace(" = ");
			i->value->dbgPrint (depth+1);
		} else
			d_trace("\n");
	}
}

void CfgList::Add(const char *name, const char *val)
{
	CfgLiteral *l=new CfgLiteral;
	l->value = val;
	childs.push_back(CfgListElem());
	childs.back().value=l;
	if (name)
		childs.back().name=name;
}

void CfgList::Add(const char *name, double val)
{
	CfgNumeric *n=new CfgNumeric;
	n->value=val;
	childs.push_back(CfgListElem());
	childs.back().value=n;
	if (name) childs.back().name=name;
}

void CfgList::Add (const char *name,CfgValue *val)
{
	childs.push_back(CfgListElem());
	childs.back().value=val;
	if (name) childs.back().name=name;
}

void CfgList::Add(const char* name, Vector2 v)
{
	CfgList* l = new CfgList();

	l->Add("x", v.x);
	l->Add("y", v.y);

	Add(name, l);
}

Color CfgList::GetColor( const char *name, Color def/*=Color::White*/ )
{
	const char* val = GetLiteral(name, 0);
	if (!val) return def;

	Color c( (uint)strtoul(val, 0, 16) );

	return c;
}