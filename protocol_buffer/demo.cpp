#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include "test/Person.pb.h"
// #include "MyTest.h"

int main()
{
    // 序列化
    std::string str;
    PersonPackage::Person p1; // 序列化对象
    p1.set_id(1);
    p1.set_name("张三");
    p1.set_sex("man");
    p1.add_address("GDUFE");
    p1.add_address("CSAU");
    p1.set_age(22);
    p1.set_color(ColorPackage::Color::Red);

    p1.SerializeToString(&str);
    std::cout << "序列化结果： " << "\n"
              << str << std::endl;

    // 反序列化
    PersonPackage::Person p2;
    p2.ParseFromString(str);
    std::cout << "反序列化结果：" << "\n"
              << "id: " << p2.id() << "\n"
              << "姓名：" << p2.name() << "\n"
              << "性别：" << p2.sex() << "\n"
              << "年龄：" << p2.age() << "\n"
              << "颜色：" << p2.color() << "\n";

    for (int i = 0; i < p2.address_size(); ++i)
    {
        std::cout << "地址：" << p2.address(i) << "\n";
    }
    return 0;
}