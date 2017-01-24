#include "runner.h"
#include <iostream>
bool TRunner::run()
{
	
	doRun();	
	return failed;
}

TRunner::TRunner(TTestConfig* p_config)
{
	config=p_config;
}


void TRunner::runner(TTestClass *p_object)
{
	p_object->setConfig(config);
	if(p_object->run()) failed=true;	
	delete p_object;
}

