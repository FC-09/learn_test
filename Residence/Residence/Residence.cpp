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

    for (int i = 0; vecAge_[i] != 0; i++)
    {
        std::cout << vecAge_[i] << ", ";
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

int* Residence::Savefile()
{
    int* member_age = new int[10];
    int i = 0;
    member_age[i] = 0;
    ++i;
    for (std::vector<int>::iterator it = vecAge_.begin(); it != vecAge_.end(); ++it)
    {
        member_age[i] = *it;
        ++i;
        member_age[i] = 0;
        ++i;
    }
    member_age[i] = -1;
    return member_age;
}

char* Residence::SaveName()
{
    char* name = new char[10];
    char i = 0;
    for (i; i<name_.length(); ++i)
        name[i] = name_[i];
    name[i] = '\0';
    return name;
}