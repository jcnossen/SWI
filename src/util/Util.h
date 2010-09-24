#pragma once

// Delete all items in a container of pointers
template<typename T> inline void DeleteAll(T& items) {
	for(typename T::iterator i=items.begin();	i!=items.end();	++i) {
		delete (*i);
		*i = NULL;
	}
	items.clear();
}

template<typename T> inline void SafeDelete(T*& item) {
	if (item) {
		delete item;
		item = 0;
	}
}

template<typename T> inline void SafeDeleteArray(T& item) {
	if (item) {
		delete[] item;
		item = 0;
	}
}

template<typename T> inline void SafeRelease(T& item) {
	if (item) {
		item->release();
		item=0;
	}
}

template<typename T, typename E> inline void ToggleFlag(T& value, E mask, bool state) {
	if (state)
		value |= (T)mask;
	else
		value &= ~(T)mask;
}

class MemoryBuf {
	char* d;
	int length;
	
public:
	MemoryBuf(){d=0;length=0;}
	MemoryBuf(char* d, int len){ this->d=d; length=len; }
	~MemoryBuf() { SafeDeleteArray(d); }
	
	void clear() { SafeDeleteArray(d); }
	void init(char* data, int len) { clear(); d=data; length=len; }
	char* steal() { char* data=d; d=0; return data; }
	int len () { return length; }
	char* data() { return d; }
	char& operator[](int i) { return d[i]; }
	
	// restrict copy constructor
private:
	MemoryBuf& operator=(const MemoryBuf& o) {return *this;}
	MemoryBuf(const MemoryBuf& o) {}
};


bool ReadFileToBuf(const char* filename, MemoryBuf& mb);
std::vector<std::string> StringSplit(const std::string& str, char sep);
std::string GetPathFromFile(const std::string& file);
bool FileExists(const char* filename);