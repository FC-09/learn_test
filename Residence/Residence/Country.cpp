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
        std::cout << "�û����Ѵ��ڣ����ʧ��" << std::endl;
        return -1;
    }

    return 0;
}

int Country::DelResidence(int residenceNo)
{
    std::map<int, Residence*>::iterator it = residenceInfo_.find(residenceNo);
    if (it == residenceInfo_.end())
    {
        std::cout << "�����ڸû��ţ�ɾ��ʧ��" << std::endl;
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
        std::cout << "�û�������" << std::endl;
        return -1;
    }
    else
    {
        std::map<int, Residence*>::iterator it = residenceInfo_.find(residenceNo);

        it->second->ClearMember();
        std::cout << "�������Ա����" << std::endl;
        int count, age;
        std::cin >> count;
        if (count <= 0)
        {
            std::cout << "����������Ϸ������ó�Աʧ��" << std::endl;
            return -1;
        }
        for (int i = 0; i <= count - 1; i++)
        {
            std::cout << "�������" << i + 1 << "����Ա����" << std::endl;
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
        std::cout << "���ڱ�Ϊ��" << std::endl;
        return 0;
    }
    std::cout << "Ŀǰ���л���Ϊ��" << std::endl;
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
        std::cout << "û�иû���Ϣ" << std::endl;
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
        std::cout << "û�иû���Ϣ" << std::endl;
        return -1;
    }
    int age;
    std::cout << "������������Ա���䣺" << std::endl;
    std::cin >> age;
    it->second->AddMember(age);
    return 0;
}

int Country::DelMember(int residenceNo)
{
    std::map<int, Residence*>::iterator it = residenceInfo_.find(residenceNo);
    if (it == residenceInfo_.end())
    {
        std::cout << "û�иû���Ϣ" << std::endl;
        return -1;
    }
    int No;
    std::cout << "������ɾ����Ա��ţ�" << std::endl;
    std::cin >> No;
    it->second->DelMember(No);
    return 0;
}

int Country::ChangeMember(int residenceNo)
{
    std::map<int, Residence*>::iterator it = residenceInfo_.find(residenceNo);
    if (it == residenceInfo_.end())
    {
        std::cout << "û�иĻ���Ϣ" << std::endl;
        return -1;
    }
    int No, age;
    std::cout << "�������޸ĳ�Ա��ţ�" << std::endl;
    std::cin >> No;
    std::cout << "�������޸ĳ�Ա���䣺" << std::endl;
    std::cin >> age;
    it->second->ChangeMember(No, age);
    return 0;
}

int Country::SaveFile(const char* file_path)
{
    FILE* file;
    std::string member;
    char* member_file = new char[100]();
    for (std::map<int, Residence*>::iterator it = residenceInfo_.begin(); it != residenceInfo_.end(); ++it)
    {
        member += it->second->Savefile();  
        
    }
    for (int i = 0; i <= strlen(member.c_str()); ++i)
    {
        member_file[i] = member[i];
    }
    if (file = fopen(file_path, "wb"))    //��ָ��·�����ļ�����������ڣ����½����ļ�
    {
        fwrite(member_file, 1, 100, file);
        std::cout << "�ѽ���Ϣ���浽ָ���ļ���" << std::endl;
    }
    else
        std::cout << "�����·������" << std::endl;
    delete[] member_file;
    fclose(file);
    return 0;
}


int Country::OpenFile(const char* file_path)
{
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
        std::cout << "�Ѽ���ָ���ļ���Ϣ" << std::endl;
    }
    else
        std::cout << "���ļ�ʧ��" << std::endl;
    delete[] member_file;
    fclose(file);
    return 0;
}