#include <iostream>
#include <map>
#include "Residence.h"

class Country
{
public:
    Country()	{};

    ~Country();

    int AddResidence(int residenceNo, const std::string& name);	//����һ��

    int DelResidence(int residenceNo);	//ɾ��һ��

    int SetResidence(int residenceNo);	//����

    int DisPlayResidenceNo();	//��ӡ����

    int DisPlayResidenceInfo(int residenceNo);	//��ʾ����Ϣ

    int AddMember(int residenceNo);	//������Ա

    int DelMember(int residenceNo);	//ɾ����Ա

    int ChangeMember(int residenceNo);	//�޸ĳ�Ա

    int SaveFile(const char*);    // ������Ϣ��ָ���ļ�

    int OpenFile(const char*);    // ��ָ���ļ��м�����Ϣ��ɾ��֮ǰ������Ϣ���������ļ��е���Ϣ��

private:
    std::map<int, Residence*> residenceInfo_;	//<���ţ�����Ϣ>
};