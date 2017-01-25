#ifndef __TESTDATAPROCINFO_H_
#define __TESTDATAPROCINFO_H_
#include "test/lib/testclass.h"
#include "src/data/processinfolist.h"

class TTestDataProcInfo:public TTestClass{
protected:
	
	virtual void doRun() override;		
	void test01();
	void test02();
	TProcessInfoList *info=nullptr;
public:
	TTestDataProcInfo();
	virtual ~TTestDataProcInfo();
};
#endif
