#include "Residence.h"


Residence::Residence(int residenceNo, const std::string& name) : residenceNo_(residenceNo)
{
    name_ = name;
}

void Residence::DisPlay()
{
    std::cout << "户主姓名：" << name_ << std::endl;
    if (vecAge_.size() == 0)
    {
        std::cout << "该户没有成员" << std::endl;
        return;
    }

    for (std::vector<int>::iterator iter = vecAge_.begin(); iter != vecAge_.end(); iter++)
    {
        std::cout << *iter << ", ";
    }
    std::cout << std::endl;
    return;
}

int Residence::AddMember(int age)
{
    vecAge_.push_back(age);
    return 0;
}

int Residence::DelMember(int memberNo)
{
    vecAge_.erase(vecAge_.begin() + memberNo);
    return 0;
}

int Residence::ChangeMember(int memberNo, int newAge)
{
    vecAge_[memberNo + 1] = newAge;
    return 0;
}

int Residence::ClearMember()
{
    vecAge_.clear();
    return 0;
}

int Residence::Size()
{
    if (vecAge_.size() == 0)
        return 0;
    else
        return 1;
}

std::string Residence::Savefile()
{
    std::string member;
    member += residenceNo_; 
    member += '<';
    member += name_;
    member += '>';
    for (std::vector<int>::iterator it = vecAge_.begin(); it != vecAge_.end(); ++it)
    {
        member += *it; 
    }
    member += ',';
    return member;
}