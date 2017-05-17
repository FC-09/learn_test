#include "Residence.h"
#include "Country.h"
#include <iostream>

int main()
{
    Country country;
    char cmd = 0;
    while (1)
    {
        std::cout << "请输入命令：" << std::endl;
        std::cin >> cmd;
        if ('L' == cmd)
            country.DisPlayResidenceNo();
        else if ('A' == cmd)
        {
            std::cout << "请输入户号和户主姓名" << std::endl;
            int residence;
            std::string name;
            std::cin >> residence >> name;
            country.AddResidence(residence, name);
        }
        else if ('I' == cmd)
        {
            std::cout << "请输入户号" << std::endl;
            int residence;
            std::cin >> residence;
            country.DisPlayResidenceInfo(residence);
        }
        else if ('D' == cmd)
        {
            std::cout << "请输入户号" << std::endl;
            int residence;
            std::cin >> residence;
            country.DelResidence(residence);
        }
        else if ('S' == cmd)
        {
            std::cout << "请输入户号" << std::endl;
            int residence;
            std::cin >> residence;
            country.SetResidence(residence);
        }
        else if ('a' == cmd)
        {
            std::cout << "请输入户号" << std::endl;
            int residence;
            std::cin >> residence;
            country.AddMember(residence);
        }
        else if ('d' == cmd)
        {
            std::cout << "请输入户号" << std::endl;
            int residence;
            std::cin >> residence;
            country.DelMember(residence);
        }
        else if ('m' == cmd)
        {
            std::cout << "请输入户号" << std::endl;
            int residence;
            std::cin >> residence;
            country.ChangeMember(residence);
        }
        else if ('B' == cmd)        // 保存信息到指定文件
        {
            /*char* file_path = new char[100];
            std::cout << "请输入要保存的文件路径" << std::endl;
            std::cin >> file_path;*/
            char* file_path = "C:\\Users\\Administrator\\Desktop\\000.txt";
            country.SaveFile(file_path);
            //delete[] file_path;
        }
        else if ('J' == cmd)     // 从指定文件中加载信息
        {
           /* char* file_path = new char[100];
            std::cout << "请输入要打开的文件路径" << std::endl;
            std::cin >> file_path;*/
            char* file_path = "C:\\Users\\Administrator\\Desktop\\000.txt";
            country.OpenFile(file_path);
            //delete[] file_path;
        }
    }
    return 0;
}

//A：保存所有信息到指定文件中 
//O:从指定文件中加载所有信息 （之前的信息不保留，全部删除）

//文件保存形式：
//户主名，户主名，/0户号0成员年龄0成员年龄0-1户号0成员年龄0成员年龄0