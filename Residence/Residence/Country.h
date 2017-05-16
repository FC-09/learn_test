#include <iostream>
#include <map>
#include "Residence.h"

class Country
{
public:
    Country()	{};

    ~Country();

    int AddResidence(int residenceNo, const std::string& name);	//增加一户

    int DelResidence(int residenceNo);	//删除一户

    int SetResidence(int residenceNo);	//设置

    int DisPlayResidenceNo();	//打印户号

    int DisPlayResidenceInfo(int residenceNo);	//显示户信息

    int AddMember(int residenceNo);	//新增成员

    int DelMember(int residenceNo);	//删除成员

    int ChangeMember(int residenceNo);	//修改成员

    int SaveFile(const char*);    // 保存信息到指定文件

    int OpenFile(const char*);    // 从指定文件中加载信息（删除之前所有信息，仅保留文件中的信息）

private:
    std::map<int, Residence*> residenceInfo_;	//<户号，户信息>
};