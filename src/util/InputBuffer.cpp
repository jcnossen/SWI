
#include "StdIncl.h"

#include <cstdarg>

#include "InputBuffer.h"


std::string ReadZStr(FILE *f)
{
	std::string s;
	int c,i=0;
	while((c = fgetc(f)) != EOF && c) 
		s += c;
	return s;
}


void WriteZStr(FILE *f, const std::string& s)
{
	int c;
	c = s.length ();
	fwrite(s.c_str(),c+1,1,f);
}


std::string SPrintf(const char *fmt, ...) {
	va_list ap;
	va_start(ap, fmt);

	char buf[256];
	VSNPRINTF(buf, sizeof(buf), fmt, ap);

	va_end(ap);
	return buf;
}

//-------------------------------------------------------------------------
// InputBuffer - serves as an input for the config value nodes
//-------------------------------------------------------------------------

void InputBuffer::showLocation() const
{
	d_trace(location().c_str());
}

std::string InputBuffer::location() const
{
	return SPrintf("In %s on line %d:", filename, line);
}

bool InputBuffer::compareIdent (const char *str) const
{
	int i = 0;
	while (str[i] && i+pos < len)
	{
		if (str[i] != data[pos+i])
			return false;

		i++;
	}

	return !str[i];
}



bool InputBuffer::skipWhitespace ()
{
	// Skip whitespaces and comments
	for(;!end();next())
	{
		if (get() == ' ' || get() == '\t' || get() == '\r' || get() == '\f' || get() == 'v')
			continue;
		if (get() == '\n')
		{
			line ++;
			continue;
		}
		if (get(0) == '/' && get(1) == '/')
		{
			pos += 2;
			while (get() != '\n' && !end())
				next();
			++line;
			continue;
		}
		if (get() == '/' && get(1) == '*')
		{
			pos += 2;
			while (!end())
			{
				if(get(0) == '*' && get(1) == '/')
				{
					pos+=2;
					break;
				}
				if(get() == '\n')
					line++;
				next();
			}
			continue;
		}
		break;
	}

	return end();
}


void InputBuffer::skipKeyword (const char *kw)
{
	skipWhitespace();

	int a=0;
	while (!end() && kw[a])
	{
		if (get() != kw[a])
			break;
		next(), ++a;
	}

	if (kw[a])
		throw ContentException(SPrintf("%s: Expecting keyword %s\n", location().c_str(), kw));
}

void InputBuffer::expecting (const char *s)
{
	throw ContentException (location() + "Expecting " + std::string(s) + "\n");
}

std::string InputBuffer::parseIdent ()
{
	std::string name;

	if(isalnum (get()))
	{
		name += get();
		for (next(); isalnum (get()) || get() == '_' || get() == '-'; next())
			name += get();

		return name;
	}
	else 
		throw ContentException(SPrintf("%s: Expecting an identifier instead of '%c'\n", location().c_str(), get()));
}

