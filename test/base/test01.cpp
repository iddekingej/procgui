#include "test01.h"
#include "test/lib/testclass.h"
/**
 *  Testing linked list
 */
void TTest01::doRun()
{
	test01();
	if(getFailed()) return;
	test02();
	if(getFailed()) return;
	test03();
}

void TTest01::test01()
{
	if(list->getStart() != nullptr) fail(" getStart is not null");
	if(list->getEnd() != nullptr) fail(" End of linklist is not null");
	if(list->getLength() != 0) fail(" Length !=0");
	if(!list->isEmpty()) fail("isEmopty returns false");
}


void TTest01::test02()
{
	item1=new TTestItem(1);
	list->append(item1);
	if(list->getStart()==nullptr) {
		fail(" GetStart is null after append");
		return;
	}
	if(list->getStart() != list->getEnd()){
		fail("Start and end not equal after appending one item");
	}
	if(list->getLength()!= 1){
		fail(" length != 1 after appending one item");
		return;
	}
	if(list->getStart()->getItem() != item1){
		fail("getItem doesn't return appended item");
		return;
	}
	if(list->getStart()->getItem()->getSomeValue()!=1){
		fail(" GetSomeValue !=1");
		return;
	}
}


void TTest01::test03()
{
	item2=new TTestItem(2);
	list->append(item2);
	if(list->getStart()->getItem() != item1){
		fail("After appending second item=>fist item != item1");		
	}
	if(list->getEnd()->getItem() != item2){
		fail("After appending second item end 1= item2");
	}
	if(list->getLength() != 2){
		fail("After appending second item length != 2");
	}
	if(list->isEmpty()){
		fail("After appending =>isEmpty returns true");
	}
	return;
}

TTest01::~TTest01()
{
	delete list;
}

TTest01::TTest01()
{	
	list=new TLinkList<TTestItem>();
}

