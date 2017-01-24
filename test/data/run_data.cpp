#include "run_data.h"
#include "cgroup01.h"

class TRunDataRunner:public TRunner
{
protected:
		
	virtual void doRun()
	{
		runner(new TDataCGroup());
	}
	
public:
	TRunDataRunner(TTestConfig *p_testConfig):TRunner(p_testConfig)
	{
		
	}

};

bool run_data(TTestConfig* p_config)
{

	TRunDataRunner l_run(p_config);
	return l_run.run();
}
