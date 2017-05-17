#pragma once
#include <iostream>
#include <vector>
#include <string>


class Residence
{
public:
    Residence(int residenceNo, const std::string& name);

    ~Residence()	{};

    void DisPlay();	//打印户信息

    int AddMember(int age);	//增加成员，返回错误码

    int DelMember(int memberNo);	//删除成员，返回错误码

    int ChangeMember(int memberNo, int newAge);	//修改成员，返回错误码

    int ClearMember();  //清空户中成员

    int Size();   //判断户是否为空

    std::string Savefile();

private:
    std::string name_;			//户主名
    const int residenceNo_;		//户号
    std::vector<int> vecAge_;	//年龄
};
