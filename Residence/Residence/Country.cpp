#include "Country.h"

Country::~Country()
{
    for (std::map<int, Residence*>::iterator it = residenceInfo_.begin(); it != residenceInfo_.end(); ++it)
    {
        delete it->second;
    }
}

int Country::AddResidence(int residenceNo, const std::string& name)
{
    if (residenceInfo_.find(residenceNo) == residenceInfo_.end())
    {
        Residence* p = new Residence(residenceNo, name);
        residenceInfo_[residenceNo] = p;
        //residenceInfo_.insert(std::pair<int, Residence*>(residenceNo, p));
    }
    else
    {
        std::cout << "该户号已存在，添加失败" << std::endl;
        return -1;
    }

    return 0;
}

int Country::DelResidence(int residenceNo)
{
    std::map<int, Residence*>::iterator it = residenceInfo_.find(residenceNo);
    if (it == residenceInfo_.end())
    {
        std::cout << "不存在该户号，删除失败" << std::endl;
        return -1;
    }
    else
    {
        delete it->second;
        residenceInfo_.erase(it);
    }
    return 0;
}

int Country::SetResidence(int residenceNo)
{
    if (residenceInfo_.find(residenceNo) != residenceInfo_.end())
    {
        std::cout << "该户不存在" << std::endl;
        return -1;
    }
    else
    {
        std::map<int, Residence*>::iterator it = residenceInfo_.find(residenceNo);

        it->second->ClearMember();
        std::cout << "请输入成员个数" << std::endl;
        int count, age;
        std::cin >> count;
        if (count <= 0)
        {
            std::cout << "输入参数不合法，设置成员失败" << std::endl;
            return -1;
        }
        for (int i = 0; i <= count - 1; i++)
        {
            std::cout << "请输入第" << i + 1 << "个成员年龄" << std::endl;
            std::cin >> age;
            it->second->AddMember(age);
        }
    }
    return 0;
}

int Country::DisPlayResidenceNo()
{
    if (residenceInfo_.size() == 0)
    {
        std::cout << "户口表为空" << std::endl;
        return 0;
    }
    std::cout << "目前所有户号为：" << std::endl;
    for (std::map<int, Residence*>::iterator it = residenceInfo_.begin(); it != residenceInfo_.end(); ++it)
    {
        std::cout << it->first << std::endl;
    }
    return 0;
}

int Country::DisPlayResidenceInfo(int residenceNo)
{
    std::map<int, Residence*>::iterator it = residenceInfo_.find(residenceNo);
    if (it == residenceInfo_.end())
    {
        std::cout << "没有该户信息" << std::endl;
        return -1;
    }
    it->second->DisPlay();
    return 0;
}

int Country::AddMember(int residenceNo)
{
    std::map<int, Residence*>::iterator it = residenceInfo_.find(residenceNo);
    if (it == residenceInfo_.end())
    {
        std::cout << "没有该户信息" << std::endl;
        return -1;
    }
    int age;
    std::cout << "请输入新增成员年龄：" << std::endl;
    std::cin >> age;
    it->second->AddMember(age);
    return 0;
}

int Country::DelMember(int residenceNo)
{
    std::map<int, Residence*>::iterator it = residenceInfo_.find(residenceNo);
    if (it == residenceInfo_.end())
    {
        std::cout << "没有该户信息" << std::endl;
        return -1;
    }
    int No;
    std::cout << "请输入删除成员编号：" << std::endl;
    std::cin >> No;
    it->second->DelMember(No);
    return 0;
}

int Country::ChangeMember(int residenceNo)
{
    std::map<int, Residence*>::iterator it = residenceInfo_.find(residenceNo);
    if (it == residenceInfo_.end())
    {
        std::cout << "没有改户信息" << std::endl;
        return -1;
    }
    int No, age;
    std::cout << "请输入修改成员编号：" << std::endl;
    std::cin >> No;
    std::cout << "请输入修改成员年龄：" << std::endl;
    std::cin >> age;
    it->second->ChangeMember(No, age);
    return 0;
}

int Country::SaveFile(const char* file_path)
{
    FILE* file;
    int* member = new int[100];
    int i = 0;
    for (std::map<int, Residence*>::iterator it = residenceInfo_.begin(); it != residenceInfo_.end(); ++it)
    {
        member[i] = it->first;
        ++i;
        for (int a = 0; it->second->Savefile()[a] != 0; ++a)
        {
            member[i] = it->second->Savefile()[a];
            ++i;
            member[i] = 0;
            ++i;
        }
    }
    if (file = fopen(file_path, "wb"))    //打开指定路径下文件，如果不存在，则新建此文件
    {
        fprintf(file, "%d", member);
        std::cout << "已将信息保存到指定文件中" << std::endl;
    }
    else
        std::cout << "输入的路径有误" << std::endl;
    delete[] member;
    fclose(file);
    return 0;
}

int Country::OpenFile(const char* file_path)
{
    FILE* file;
    int* member = new int[100];
    if (file = fopen(file_path, "rb"))
    {
        //--------------------------------------
        fscanf(file, "%d", &member);
        residenceInfo_.clear();
        for (int i = 0; member[i] != 0; ++i)
        {
            member[i]
        }

        std::cout << "已加载指定文件信息" << std::endl;
    }
    else
        std::cout << "打开文件失败" << std::endl;
    delete[] member;
    fclose(file);
    return 0;
}