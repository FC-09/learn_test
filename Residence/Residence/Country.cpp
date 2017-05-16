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
    int* member = new int[100];
    char* name = new char[100];
    int i = 0;
    char b = 0;
    for (std::map<int, Residence*>::iterator it = residenceInfo_.begin(); it != residenceInfo_.end(); ++it)
    {
        member[i] = it->first;
        ++i;
        for (int a = 0; it->second->Savefile()[a] != -1; ++a)
        {
            member[i] = it->second->Savefile()[a];
            ++i;
        }
        member[i] = -1;
        ++i;
        for (char a = 0; it->second->SaveName()[a] != 0; ++a)
        {
            name[b] = it->second->SaveName()[a];
            ++b;
        }
        name[b] = ',';
        ++b;
    }
    if (file = fopen(file_path, "wb"))    //��ָ��·�����ļ�����������ڣ����½����ļ�
    {
        fprintf(file, "%s", name);
        fprintf(file, "%d", member);
        std::cout << "�ѽ���Ϣ���浽ָ���ļ���" << std::endl;
    }
    else
        std::cout << "�����·������" << std::endl;
    delete[] name;
    delete[] member;
    fclose(file);
    return 0;
}

int Country::OpenFile(const char* file_path)
{
    FILE* file;
    int* member = new int[100];
    char* name = new char[100];
    if (file = fopen(file_path, "rb"))
    {
        fscanf(file, "%s%/0%d", name, member);
        residenceInfo_.clear();
        int a = 0;
        std::string n;
        for (char i = 0; name[i] != 0; ++i)
        {
            if (name[i] != ',')
                n += name[i];
            else
            {
                Residence* p = new Residence(member[a], n);
                residenceInfo_[member[a]] = p;
                for (a; member[a] != -1; ++a)
                {
                    if (member[a] != 0)
                        p->AddMember(member[a]);
                }
                ++a;
            }
        }
        std::cout << "�Ѽ���ָ���ļ���Ϣ" << std::endl;
    }
    else
        std::cout << "���ļ�ʧ��" << std::endl;
    delete[] name;
    delete[] member;
    fclose(file);
    return 0;
}