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


int Residence::OpenFile(FILE* f)
{
    int name_len;
    if (fread(&name_len, sizeof(int), 1, f) != 1)
        return -1;
    char name[256];
    if (fread(name, name_len, 1, f) != 1)
        name_.assign(name, name_len);
    /*
    name[name_lne] = 0;
    name_ = name;
    */

    int count;
    if (fread(&count, sizeof(int), 1, f) != 1)
        return -1;
    for (int i = 0; i < count; i++)
    {
        int age;
        if (fread(&age, sizeof(int), 1, f) != 1)
            vecAge_.push_back(age);
    }
}
int Residence::SaveFile(FILE* f)
{
    int temp = name_.size();
    //write name len
    if (fwrite(&temp, sizeof(int), 1, f) != 1)
        return -1;

    // write name
    if (fwrite(name_.c_str(), temp*sizeof(int), 1, f) != 1)
        return -1;

    //write Age count
    temp = vecAge_.size();
    if (fwrite(&temp, sizeof(int), 1, f) != 1)
        return -1;

    //wiate age
   /* for (std::vector<int>::iterator it = vecAge_.begin(); it != vecAge_.end(); ++it)
    {
        if (fwrite(&(*it), sizoef(int), 1, f) != 1)
            return -1;
    }*/
    return fwrite(vecAge_.data(), temp*sizeof(int), 1, f) == 1 ? 0 : -1;
}

 /*   std::string member;
    member += residenceNo_; 
    member += '<';
    member += name_;
    member += '>';
    for (std::vector<int>::iterator it = vecAge_.begin(); it != vecAge_.end(); ++it)
    {
        member += *it; 
    }
    member += ',';
    return member;*/
