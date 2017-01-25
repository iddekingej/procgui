#include "procinfo01.h"
TTestDataProcInfo::TTestDataProcInfo()
{
	info=new TProcessInfoList();
}

TTestDataProcInfo::~TTestDataProcInfo()
{
	if(info!=nullptr) delete info;
}


void TTestDataProcInfo::doRun()
{
	test01();
	if(getFailed()){
		return;
	}
	test02();
}

/**
 *  This routine read processes from test folder
 *  and checks of number processes read is correct
 */

void TTestDataProcInfo::test01()
{
	TTestConfig *l_config=getConfig();
	info->setBasePath(l_config->getProcTest());
	info->readInfo();
	expect("Number of processes",2,info->getLength());
}

void TTestDataProcInfo::test02()
{
	TLinkListIterator<TProcessInfo> l_iter(info);
	if(!l_iter.hasNext()){
		fail("TlinkListIterator.hasNext returns false for the first time, expect item");
		return;
	}
	TProcessInfo *l_item=l_iter.next();
	if(l_item==nullptr){
		fail("First item is null");
		return;
	}
	expect("ProcessInfo.cmdline","aa bb cc",l_item->getCmdLine());
	expect("ProcessInfo.comm","test1",l_item->getComm());
	expect("ProcessInfo.cwd",getConfig()->getFilePath("101/cwd"),l_item->getCWD()+"/");
}
