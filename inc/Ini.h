#include <map>
#include <string>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
using namespace std;

#define CONFIGLEN 256

enum INI_RES
{
	INI_SUCCESS,
	INI_ERROR,
	INI_OPENFILE_ERROR,
	INI_NO_ATTR
};

typedef map<std::string, std::string> KEYMAP;

typedef map<std::string, KEYMAP> MAINKEYMAP;

class CIni
{
public:
	CIni();
	virtual ~CIni();

public:
	int getInt(const char* mAttr, const char* cAttr);

	char *getStr(const char* mAttr, const char* cAttr);

	double getDouble(const char* mAttr, const char* cAttr);

	INI_RES openFile(const char* pathName, const char* type);  

	INI_RES closeFile();  

protected:
	INI_RES getKey(const char* mAttr, const char* cAttr, char* value);  

protected:
	FILE* m_fp;  
    char  m_szKey[ CONFIGLEN ];  
    MAINKEYMAP m_Map; 
};