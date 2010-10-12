
#include "StdIncl.h"
#include "Util.h" 

#include <sys/stat.h>

std::string GetPathFromFile(const std::string& file) 
{
	// remove the filename
	int x=file.size()-1;
	while (x>=0) {
		if (file[x] == '/' || file[x] == '\\')
			break;
		--x;
	}
	return file.substr(0, x+1);
}

std::vector<std::string> StringSplit(const std::string& str, char sep)
{
	std::vector<std::string> r;
	std::string cur;
	for(int x=0;x<str.size();x++) {
		if (str[x] == sep) {
			r.push_back(cur);
			cur="";
		} else {
			cur+=str[x];
		}
	}
	if (cur.size()>0)
		r.push_back(cur);
	return r;
}


bool ReadFileToBuf(const char* filename, MemoryBuf& mb) {
	FILE* f = fopen(filename, "rb");
	
	if (!f) 
		return false;
	
	fseek(f, 0, SEEK_END);
	int len = ftell(f);
	fseek(f, 0, SEEK_SET);
	
	char* data = new char[len];
	if (fread(data, 1, len, f) != len) {
		fclose(f);
		delete[] data;
		return false;
	}
	fclose(f);
	
	mb.init(data, len);
	return true;
}

bool FileExists(const char *file) {
#ifdef _MSC_VER
	struct _stat s;
	return !_stat(file, &s);
#else
	return true;
#endif
}

float UnitRandom()
{
	return rand() / (float)RAND_MAX;
}


float RandomNormalDistributed(double mean, double stdev)
{
  float U1=rand()/(float)RAND_MAX;
  float U2=rand()/(float)RAND_MAX;

  float Z=sqrt(-2*log(U1))*cos(2*3.14159265358979*U2);

  return Z*stdev+mean;
}

float UniformRandom(float low, float high)
{
  return (rand()/(float)RAND_MAX)*(high-low)+low;
}



