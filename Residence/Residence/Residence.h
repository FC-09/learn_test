#pragma once
#include <iostream>
#include <vector>
#include <string>


class Residence
{
public:
    Residence(int residenceNo, const std::string& name);

    ~Residence()	{};

    void DisPlay();	//��ӡ����Ϣ

    int AddMember(int age);	//���ӳ�Ա�����ش�����

    int DelMember(int memberNo);	//ɾ����Ա�����ش�����

    int ChangeMember(int memberNo, int newAge);	//�޸ĳ�Ա�����ش�����

    int ClearMember();  //��ջ��г�Ա

    int Size();   //�жϻ��Ƿ�Ϊ��

    int* Savefile();    // ���滧�г�Ա

private:
    std::string name_;			//������
    const int residenceNo_;		//����
    std::vector<int> vecAge_;	//����
};
