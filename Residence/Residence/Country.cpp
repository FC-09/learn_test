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
    if (FILE* file = fopen(file_path, "wb"))
    {
        for (std::map<int, Residence*>::iterator it = residenceInfo_.begin(); it != residenceInfo_.end(); ++it)
        {
            if (fwrite(&it->first, sizeof(int), 1, file) != 1)
            {
                std::cout << "文件写入失败" << std::endl;
                return -1;
            }
                
            if (it->second->SaveFile(file) == -1)
            {
                std::cout << "文件写入失败" << std::endl;
                return -1;
            }
        }
        fclose(file);
        std::cout << "文件写入成功" << std::endl;
    }
    else
    {
        std::cout << "输入的路径有误" << std::endl;
        return -1;
    } 
    return 0;
}


    /*
    FILE* file;
    std::string member;
    char* member_file = new char[100]();
    for (std::map<int, Residence*>::iterator it = residenceInfo_.begin(); it != residenceInfo_.end(); ++it)
    {
        member += it->second->Savefile();    
    }
    int len = member.size();
    for (int i = 0; i < len; ++i)
    {
        member_file[i] = member[i];
    }
    if (file = fopen(file_path, "wb"))    //打开指定路径下文件，如果不存在，则新建此文件
    {
        fwrite(member.c_str(), 1, 100, file);
        std::cout << "已将信息保存到指定文件中" << std::endl;
    }
    else
        std::cout << "输入的路径有误" << std::endl;
    delete[] member_file;
    fclose(file);*/
    

int Country::OpenFile(const char* file_path)
{
    if (FILE* file = fopen(file_path, "rb"))
    {
        residenceInfo_.clear();
        int* file_buff = new int[100]();
        fread(file_buff, sizeof(int), 100, file);
        for (int i = 0; file_buff[i] != 0; ++i)
        {
            std::string name;    //户主名
            int No = file_buff[i++];    //户号
            int n_len = file_buff[i++];   //户主名长度
            //添加户主名  
            for (int n = n_len; n > 0; n--)
            {
                for (char m = 0; m < 4; ++m)
                {
                    if (n_len > 0)
                    {
                        name += (file_buff[i] >> m * 8);
                        n_len--;
                    }   
                }
                i++;
            }
            Residence* p = new Residence(No, name);
            residenceInfo_[No] = p;
            //添加成员年龄
            int m_len = file_buff[i];     //成员年龄个数
            for (int n = 0; n < m_len; ++n)
            {
                p->AddMember(file_buff[++i]);
            }
        }
        fclose(file);
        std::cout << "文件加载成功" << std::endl;
    }
    else
    {
        std::cout << "文件打开失败" << std::endl;
        return -1;
    }
    return 0;
}
    /*
    FILE* file;
    char* member_file = new char[100]();
    if (file = fopen(file_path, "rb"))
    {
        fread(member_file, 1, 100, file);
        residenceInfo_.clear();
        for (char i = 0; member_file[i] != 0; ++i)
        {
            if (member_file[i] != ',')
            {
                int No = member_file[i];
                i++;
                i++;
                std::string name;
                for (i; member_file[i] != '>'; ++i)
                    name += member_file[i];
                i++;
                Residence* p = new Residence(No, name);
                residenceInfo_[No] = p;
                for (i; member_file[i] != ','; ++i)
                {
                    int member_age = member_file[i];
                    p->AddMember(member_age);
                }
            }
            else
                continue;
        }
        std::cout << "已加载指定文件信息" << std::endl;
    }
    else
        std::cout << "打开文件失败" << std::endl;
    delete[] member_file;
    fclose(file);
    */
   