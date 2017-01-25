#include "testclass.h"
#include <iostream>
#include "../src/base/utils.h"


void TTestClass::error(const char * p_file,const char *p_method,const char* p_message)
{
	std::cout << p_file << ":" << p_method <<":" << p_message <<std::endl;
	failed=true;
}

void TTestClass::errorv(const char* p_file, const char* p_method, const char* p_message, const char *p_result)
{
	std::cout << p_file << ":" << p_method <<":" << p_message <<" returned:" << p_result <<std::endl;
	failed=true;

}

void TTestClass::errorv(const char* p_file, const char* p_method, const char* p_message, QString p_value)
{
	errorv(p_file,p_method,p_message,qstr(p_value));
}


void TTestClass::errorv(const char* p_file, const char* p_method, const char* p_message, long p_value)
{
	std::cout << p_file << ":" << p_method <<":" << p_message <<" returned:" << p_value <<std::endl;
	failed=true;
}



bool TTestClass::fexpect(const char* p_file, const char* p_method, const char* p_message, bool p_expected, bool p_found)
{
	if(p_expected != p_found){
		std::cout << p_file << ":" <<p_method <<":" << p_message <<": expected " << p_expected << " returned "<<p_found <<std::endl;
		failed=true;
		return true;
	} 
	return false;
}

bool TTestClass::fexpect(const char* p_file, const char* p_method, const char* p_message,  int p_expected, int p_found)
{
	if(p_expected != p_found){
		std::cout << p_file << ":" <<p_method <<":" << p_message <<": expected " << p_expected << " returned "<<p_found <<std::endl;
		failed=true;
		return true;
	} 
	return false;
}


bool TTestClass::fexpect(const char* p_file, const char* p_method, const char* p_message, QString p_expected, QString p_found)
{
	if(p_expected != p_found){
		std::cout << p_file << ":" <<p_method <<":" << p_message <<": expected " << qstr(p_expected) << " returned "<<qstr(p_found) <<std::endl;
		failed=true;
		return true;
	} 
	return false;
}

bool TTestClass::run()
{
	failed=false;
	doRun();	
	return failed;
}





