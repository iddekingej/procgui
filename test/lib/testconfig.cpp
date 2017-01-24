#include "testconfig.h"
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

const char *g_tempDir="/tmp/bdguiXXXXXX";

TTestConfig::TTestConfig()
{
}

TTestConfig::~TTestConfig()
{

}


bool TTestConfig::initConfig()
{
	char *l_tempDir=strdup(g_tempDir);
	if(l_tempDir==nullptr) return false;
	char *l_result=mkdtemp(l_tempDir);		
	if(l_result ==nullptr){
		free(l_tempDir);
		return false;
	}
	tempDir=l_tempDir;
	free(l_tempDir);
	return true;
}

bool TTestConfig::tmpMkDir(const std::string &p_path,std::string *p_fullPath)
{
	std::string l_fullPath=tempDir+"/"+p_path;
	if(p_fullPath!=nullptr){
		*p_fullPath=l_fullPath;
	}
	return mkdir(l_fullPath.c_str(),0777)==0;	
}

QString TTestConfig::getFilePath(const QString& p_path)
{
	return QStringLiteral("../../test/testdata/")+p_path;
}