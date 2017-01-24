#ifndef __RUNNER_H_
#define __RUNNER_H_
#include "testclass.h"
#include "testconfig.h"

class TRunner
{
private:
	bool failed=false;
	TTestConfig *config;
protected:
	virtual void doRun()=0;
public:
	bool run();
	void runner(TTestClass *p_object);
	TRunner(TTestConfig *p_config);
	virtual ~TRunner(){};
};
#endif
