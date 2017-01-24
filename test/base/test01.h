#ifndef __TEST01_H_
#define __TEST01_H_
#include "../lib/testclass.h"
#include "../../src/base/linklist.h"
class TTestItem
{
private:
	int someValue;
public:
	inline int getSomeValue(){ return someValue;}
	TTestItem(int p_someValue){ someValue=p_someValue;}
};

class TTest01:public TTestClass
{
private:
	TLinkList<TTestItem> *list;
	TTestItem *item1=nullptr;
	TTestItem *item2=nullptr;
protected:
	virtual void doRun() override;		
public:
	void test01();
	void test02();
	void test03();
	TTest01();
	virtual ~TTest01();
};

#endif
