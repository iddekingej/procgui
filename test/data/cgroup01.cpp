#include "cgroup01.h"
#include "src/data/cgroupinfo.h"

TDataCGroup::TDataCGroup():TTestClass()
{
	
}

TDataCGroup::~TDataCGroup()
{

}

void TDataCGroup::doRun()
{
    test01();
}


void TDataCGroup::test01()
{
	TCGroupInfo *l_info=new TCGroupInfo(101,"bla1","bla2");
	expect("Hyrarchie ID",101,l_info->getHierarchyId());
	expect("cgroup","bla2",l_info->getCGroup());
	expect("bla2","bla1",l_info->getSubsystems());
}
