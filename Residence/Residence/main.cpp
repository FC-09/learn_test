#include "Residence.h"
#include "Country.h"
#include <iostream>

int main()
{
    Country country;
    char cmd = 0;
    while (1)
    {
        std::cout << "���������" << std::endl;
        std::cin >> cmd;
        if ('L' == cmd)
            country.DisPlayResidenceNo();
        else if ('A' == cmd)
        {
            std::cout << "�����뻧�źͻ�������" << std::endl;
            int residence;
            std::string name;
            std::cin >> residence >> name;
            country.AddResidence(residence, name);
        }
        else if ('I' == cmd)
        {
            std::cout << "�����뻧��" << std::endl;
            int residence;
            std::cin >> residence;
            country.DisPlayResidenceInfo(residence);
        }
        else if ('D' == cmd)
        {
            std::cout << "�����뻧��" << std::endl;
            int residence;
            std::cin >> residence;
            country.DelResidence(residence);
        }
        else if ('S' == cmd)
        {
            std::cout << "�����뻧��" << std::endl;
            int residence;
            std::cin >> residence;
            country.SetResidence(residence);
        }
        else if ('a' == cmd)
        {
            std::cout << "�����뻧��" << std::endl;
            int residence;
            std::cin >> residence;
            country.AddMember(residence);
        }
        else if ('d' == cmd)
        {
            std::cout << "�����뻧��" << std::endl;
            int residence;
            std::cin >> residence;
            country.DelMember(residence);
        }
        else if ('m' == cmd)
        {
            std::cout << "�����뻧��" << std::endl;
            int residence;
            std::cin >> residence;
            country.ChangeMember(residence);
        }
        else if ('B' == cmd)        // ������Ϣ��ָ���ļ�
        {
            /*char* file_path = new char[100];
            std::cout << "������Ҫ������ļ�·��" << std::endl;
            std::cin >> file_path;*/
            char* file_path = "C:\\Users\\Administrator\\Desktop\\000.txt";
            country.SaveFile(file_path);
            //delete[] file_path;
        }
        else if ('J' == cmd)     // ��ָ���ļ��м�����Ϣ
        {
           /* char* file_path = new char[100];
            std::cout << "������Ҫ�򿪵��ļ�·��" << std::endl;
            std::cin >> file_path;*/
            char* file_path = "C:\\Users\\Administrator\\Desktop\\000.txt";
            country.OpenFile(file_path);
            //delete[] file_path;
        }
    }
    return 0;
}

//A������������Ϣ��ָ���ļ��� 
//O:��ָ���ļ��м���������Ϣ ��֮ǰ����Ϣ��������ȫ��ɾ����

//�ļ�������ʽ��
//����������������/0����0��Ա����0��Ա����0-1����0��Ա����0��Ա����0