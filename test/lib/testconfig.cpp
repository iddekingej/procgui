#include "testconfig.h"
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <QFile>
#include "src/base/utils.h"
#include <iostream>
#include <QDir>
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
	return makeTestData();	
}

bool TTestConfig::tmpMkDir(const QString &p_path,QString &p_fullPath)
{
	QString l_fullPath=tempDir+"/"+p_path+"/";
	p_fullPath=l_fullPath;	
	return mkdir(l_fullPath.toUtf8().data() ,0777)==0;	
}



QString TTestConfig::getFilePath(const QString& p_path)
{
	return QDir(QStringLiteral("../../test/testdata/")+p_path+QStringLiteral("/")).absolutePath()+"/";
}

bool TTestConfig::makeProc(const QString& p_name, const QString& p_source)
{
	QString l_source=getFilePath(p_source);
	QDir(procTest).mkdir(p_name);
	QString l_dest=procTest+p_name+"/";		
	QFile::link(l_source+"exe",l_dest+"exe");
	QFile::copy(l_source+"comm",l_dest+"comm");
	QFile::link(l_source+"cwd",l_dest+"cwd");
	QFile::copy(l_source+"cmdline",l_dest+"cmdline");
	return true;
}


bool TTestConfig::makeTestData()
{
	QString l_procTest;
	QString l_dataTest;
	if(!tmpMkDir("proc",l_procTest)){
		std::cout  <<"Failed" << std::endl;
		return false;
	}
	procTest=l_procTest;
	std::cout << qstr(procTest) <<std::endl;
	makeProc("101","101");
	makeProc("102","102");
	return true;
}
