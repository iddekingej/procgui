#include "base/run_base.h"
#include "data/run_data.h"
#include "lib/testconfig.h"
#include <iostream>

int main( int argc,char **argv) {
	TTestConfig *l_config=new TTestConfig();
	if(!l_config->initConfig()){
		std::cout << "init config failed" <<std::endl;
		return 255;
	}
	if(run_base(l_config)){
		std::cout <<"run_base failed" <<std::endl;
		return 255;
	}
	if(run_data(l_config)){
		std::cout <<"run data failed " <<std::endl;
		return 255;
	}
	delete l_config;
	return 0;
}
