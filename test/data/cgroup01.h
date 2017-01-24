#ifndef __CGROUP_H_
#define __CGROUP_H_
#include "test/lib/testclass.h"

class TDataCGroup:public TTestClass{
protected:
	virtual void doRun() override;		
	void test01();
public:
	TDataCGroup();
	virtual ~TDataCGroup();
};
#endif
