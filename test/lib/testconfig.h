#ifndef __TESTCONFIG_H_
#define __TESTCONFIG_H_
#include <string>
#include <QString>

class TTestConfig
{
private:
	QString tempDir;
	QString procTest;
	QString dataTest;
public:
	inline QString &getProcTest(){ return procTest;}
	inline QString &getTempDir(){return tempDir;};
	bool tmpMkDir(const QString &p_path,QString &p_fullPath);
	bool initConfig();
	bool makeTestData();
	QString getFilePath(const QString& p_path);
	bool makeProc(const QString &p_name,const QString &p_source);
	TTestConfig();
	~TTestConfig();
};

#endif
