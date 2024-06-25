# 1 Protobuf概述

protobuf也叫protocol buffer是google 的一种数据交换的格式，它独立于语言，独立于平台。google 提供了多种语言的实现：java、c#、c++、go 和 python 等，每一种实现都包含了相应语言的编译器以及库文件。

由于它是一种二进制的格式，比使用 xml 、json进行数据交换快许多。可以把它用于分布式应用之间的数据通信或者异构环境下的数据交换。作为一种效率和兼容性都很优秀的二进制数据传输格式，可以用于诸如网络传输、配置文件、数据存储等诸多领域。

### 1.1 源码安装

```http
https://github.com/protocolbuffers/protobuf/releases
```

### 1.2 在Linux安装

```shell
#自行下载压缩包
#解压
[root@localhost package]# tar zxvf protobuf-cpp-3.21.12.tar.gz
#进入目录
[root@localhost package]# cd protobuf-3.21.11
#进行构建和安装
[root@localhost protobuf-3.21.11]# ./configure
#编译
[root@localhost protobuf-3.21.11]# make
#安装
[root@localhost protobuf-3.21.11]# make install
```

### 1.3 验证安装

```shell
[root@localhost protobuf-3.21.11]# protoc --version
#安装成功会输出protobuf当前版本号
libprotoc 3.21.11
```

### 1.4 安装可能出现的错误

```shell
[root@localhost protobuf-3.21.11]# protoc --version
protoc: error while loading shared libraries: libprotoc.so.32: cannot open shared object file: No such file or directory
```

通过输出的信息可以看到找不到动态库，此时可以通过 **find** 进行搜索

```shell
[root@localhost protobuf-3.21.11]# find / -name libprotoc.so.32
/usr/local/lib/libprotoc.so.32
```

此时存储动态库的路径就找到了。

可以看到这个动态库位于/usr/local/lib目录，接下来需要需要将这目录添加到/etc/ld.so.conf这个配置文件中：

```shell
[root@localhost protobuf-3.21.11]# vim /etc/ld.so.conf
```

将路径添加到第二行，保存退出。

再更新配置：

```shell
[root@localhost protobuf-3.21.11]# ldconfig
```

### 1.5 如何使用

使用protbuf进行数据的序列化主要有几个步骤：

- 确定数据格式

  ```c++
  // 要序列化的数据
  // 第一种: 单一数据类型
  int number;
  
  // 第二种: 复合数据类型
  struct Person
  {
      int id;
      string name;
      string sex;	
      int age;
  };
  ```

- 创建一个新的文件, 文件名随意指定, 文件后缀为 .proto

- 使用protobuf的语法,根据确定的数据格式，编辑相对应的.proto文件

- 使用 protoc 命令将 .proto 文件转化为相应的 C++ 文件

  ```c++
  //源文件: xxx.pb.cc –> xxx对应的名字和 .proto文件名相同
  //头文件: xxx.pb.h –> xxx对应的名字和 .proto文件名相同
  ```

- 需要将生成的c++文件添加到项目中, 通过文件中提供的类 API 实现数据的序列化/反序列化



**protobuf中的数据类型 和 C++ 数据类型对照表:**

| Protobuf类型 | C++类型            | 备注                                                         |
| ------------ | ------------------ | ------------------------------------------------------------ |
| double       | double             | 64位浮点数                                                   |
| float        | float              | 32位浮点数                                                   |
| int32        | int                | 32位整数                                                     |
| int64        | long               | 64位整数                                                     |
| uint32       | unsigned int       | 32位无符号整数                                               |
| uint64       | unsigned long      | 64位无符号整数                                               |
| sint32       | signed int         | 32位整数，处理负数效率比int32更高                            |
| sint64       | signed long        | 64位整数，处理负数效率比int64更高                            |
| fixed32      | unsigned int(32位) | 总是4个字节。如果数值总是比总是比228大的话，这个类型会比uint32高效。 |
| fixed64      | unsigned int(64位) | 总是8个字节。如果数值总是比总是比256大的话，这个类型会比uint64高效。 |
| sfixed32     | int(32位)          | 总是4个字节                                                  |
| sfixed64     | long(64位)         | 总是8个字节                                                  |
| bool         | bool               | 布尔类型                                                     |
| string       | string             | 一个字符串必须是UTF-8编码或者7-bit ASCII编码的文本           |
| **bytes**    | string             | 处理多字节的语言字符、如中文, 建议protobuf中字符型类型使用 bytes |
| enum         | enum               | 枚举                                                         |
| message      | object of class    | 自定义的消息类型                                             |



# 2 Protobuf语法

### 2.1 基本使用

假设我C++代码中定义了这样一个结构体，现在要基于这个结构体完成数据的序列化，结构体原型如下：

```c++
struct Person
{
    int id;
    string name;
    string sex;
    int age;
};
```

接下来，我们需要新建一个文件叫**"Person.proto"**，后缀指定为 .proto，在文件的第一行需要指定Protobuf的版本号，有两个版本Protobuf 2 和 Protobuf 3，默认情况下是版本2，此处我们使用的是版本3。

```protobuf
syntax="proto3"
//文件第一行
```

接着需要定义一个消息体，其格式如下：

```protobuf
message 名字	
{
 // 类中的成员, 格式, 编号
    数据类型 成员名字 = 1;
    数据类型 成员名字 = 2;
    数据类型 成员名字 = 3;
	   ......     
	   ......
}

//message后面的名字就是生成的类的名字，自己指定一个合适的名字即可
//等号后面的编号要从1开始，每个成员都有一个唯一的编号，不能重复，且序号需要递增，一般连续编号即可：（1，2，3，4，5...）

```

基于上面的语法，上面结构体对应的**Person.proto**文件的内容可以写成这样：

```protobuf
syntax = "proto3";

// 在该文件中对要序列化的结构体进行描述
message Person
{
    int32 id = 1;
    bytes name = 2;
    bytes sex = 3;	
    int32 age = 4;
}
```

**.proto**文件编辑好之后就可以使用**protoc**工具将其转换为C++文件了。

```shell
$ protoc -I path *.proto --cpp_out=OUT_DIR

#   protoc：编译.protoc文件的工具
#       -I：指定.protoc文件的目录
#     path：.protoc文件的目录
#  *.proto：.protoc文件的名称
#--cpp_out：生成C++文件
#  OUT_DIR：储存生成C++文件的路径
```

### 2.2 repearted限定修饰符

在使用Protocol Buffers（Protobuf）中，可以使用repeated关键字作为限定修饰符来表示一个字段可以有多个值，即重复出现的字段。

repeated关键字可以用于以下数据类型：**基本数据类型、枚举类型、自定义消息类型。**



比如要序列化的数据中有一个数组，结构如下：

```c++
// 要序列化的数据
struct Person
{
    int id;
    string name;
    string sex;	
    string address[10];
    int age;
};
```

Protobuf 的消息体就可以写成下面的样子:

```protobuf
message Person
{
    int32 id = 1;
    bytes name = 2;
    bytes sex = 3;	
    repeated bytes address = 4;
    int32 age = 5;
}
```

**使用repeated关键字定义的字段在Protobuf序列化和反序列化时会被当作一个集合或数组来处理。这个address可以作为一个动态数组来使用。**

### 2.3 枚举

在 Protocol Buffers 中，枚举类型用于定义一组特定的取值。下面定义一个枚举，并将其添加到**Person**中：

```c++
// 要序列化的数据
// 枚举
enum Color
{
    Red = 5,	// 可以不给初始值, 默认为0
    Green,
    Yellow,
    Blue
};

// 要序列化的数据
struct Person
{
    int id;
    string name;
    string sex;	
    string address[10];
    int age;
    // 枚举类型
    Color color;
};
```

以下是如何在 Protobuf 中定义和使用枚举类型，其语法如下：

```protobuf
enum 名字
{
    元素名 = 0;	// 枚举中第一个原素的值必须为0
    元素名 = 数值;
}
```

**枚举元素之间使用分号间隔 ;，并且需要注意一点proto3 中的第一个枚举值必须为 0，第一个元素以外的元素值可以随意指定。**

上面例子中的数据在.proto文件中可以写成如下格式:

```protobuf
// 定义枚举类型
enum Color
{
    Red = 0;
    Green = 3;		// 第一个元素以外的元素值可以随意指定，最好是连续编号：（1，2，3，4...）
    Yellow = 6;
    Blue = 9;
}
// 在该文件中对要序列化的结构体进行描述
message Person
{
    int32 id = 1;
    repeated bytes name = 2;
    bytes sex = 3;	
    int32 age = 4;
    // 枚举类型
    Color color = 5;
}
```

### 2.4 proto文件的导入

在 Protocol Buffers 中，可以使用import语句在当前.ptoto中导入其它的.proto文件。

这样就可以在一个.proto文件中引用并使用其它文件中定义的消息类型和枚举类型。

**其操作和C++引入头文件类似。**

语法格式如下:

```protobuf
import "要使用的proto文件的名字";
```

假设现在我把刚才**Person.proto**文件中的**枚举体Color**分离出来，存放在**Color.proto**文件里面

```protobuf
syntax = "proto3";

// 定义枚举类型
enum Color
{
    Red = 0;
    Green = 3;		
    Yellow = 6;
    Blue = 9;
}
```

我现需要再上面定义的**Person.proto**中添加**Color**，因此就需要用到**Color.proto**中定义的消息体：

```protobuf
syntax = "proto3";
// 使用另外一个proto文件中的数类型, 需要导入这个文件
import "Color.proto";

// 在该文件中对要序列化的结构体进行描述
message Person
{
    int32 id = 1;
    bytes name = 2;
    bytes sex = 3;	
    repeated address = 4;
    int32 age = 5;
    // 外部proto文件中定义的数据类型
    Color color = 6;
}
```

- import语句中指定的文件路径可以是相对路径或绝对路径。如果文件在相同的目录中，只需指定文件名即可。

- 导入的文件将会在编译时与当前文件一起被编译。
- 导入的文件也可以继续导入其他文件，形成一个文件依赖的层次结构。

### 2.5 包(package)

在 Protobuf 中，可以使用**package**关键字来定义一个消息所属的包（package）。

包是用于组织和命名消息类型的一种机制，类似于命名空间的概念。



在一个.proto文件中，可以通过在顶层使用package关键字来定义包：

```protobuf
syntax = "proto3";
package mypackage;	//包

message MyMessage 
{
  // ...
}
```

在这个示例中，我们使用**package**关键字将MyMessage消息类型定义在名为**mypackage**的包中。包名作为一个标识符来命名，可以使用任何有效的标识符，按惯例使用小写字母和下划线。

使用包可以避免不同**.proto**文件中的消息类型名称冲突，同时也可以更好地组织和管理大型项目中的消息定义。可以将消息类型的名称定义在特定的包中，并使用限定名来引用这些类型。



**下面有两个proto文件，分别给他们添加一个package：**

- proto文件 - Color.proto

  ```protobuf
  syntax = "proto3";
  
  //添加包 ColorPackage 
  package ColorPackage;
  
  enum Color
  {
      Red = 0;
      Green = 3;		
      Yellow = 6;
      Blue = 9;
  }
  ```

- proto文件 - Person.proto

  ```protobuf
  syntax = "proto3";
  import "Color.proto";
  
  //添加包 PersonPackage 
  package PersonPackage;
  
  message Person
  {
      int32 id = 1;
      bytes name = 2;
      bytes sex = 3;	
      repeated address = 4;
      int32 age = 5;
      // 添加信息, 使用的是外部proto文件中定义的数据类型
      // 如果这个外边类型属于某个包, 语法格式:
      // 包的名字.类名 变量名=编号;
      ColorPackage.Color color = 6;
  }
  ```

Protobuf里面的包(package)与C++命名空间(namespace)类似，

在使用**protoc**编译生成的C++文件后，**.proto**文件中的**包(package)**在C++文件里面的表达格式就是**命名空间(namespace)**。

# 3 序列化和反序列化

### 3.1 *.pb.h文件

通过**protoc**命令对.proto文件的转换，得到的头文件中有一个类，这个类的名字和 **.proto**文件中**message关键字后边指定的名字**相同，**.proto**文件中**message**消息体的成员就是生成的类的**私有成员**。

那么如何访问生成的类的私有成员呢？ 可以调用生成的类提供的公共成员函数，这些函数有如下规律：

- 清空(初始化) 私有成员的值:  **clear_变量名()**
- 获取类私有成员的值:  **变量名()**
- 给私有成员进行值的设置:  **set_变量名(参数)**
- 得到类私有成员的地址, 通过这块地址读/写当前私有成员变量的值:  **mutable_变量名()**
- 如果这个变量是数组类型:
- 数组中元素的个数:  **变量名_size()**
- 添加一块内存, 存储新的元素数据:  **add_变量名() 、add_变量名(参数)**

### 3.2 序列化

序列化是指将数据结构或对象转换为可以在储存或传输中使用的二进制格式的过程。在计算机科学中，序列化通常用于将内存中的对象持久化存储到磁盘上，或者在分布式系统中进行数据传输和通信。

Protobuf 中为我们提供了相关的用于数据序列化的 API，如下所示：

```c++
// 头文件目录: google\protobuf\message_lite.h
// --- 将序列化的数据 数据保存到内存中

// 将类对象中的数据序列化为字符串, c++ 风格的字符串, 参数是一个传出参数
bool SerializeToString(std::string* output) const;

// 将类对象中的数据序列化为字符串, c 风格的字符串, 参数 data 是一个传出参数
bool SerializeToArray(void* data, int size) const;

// ------ 写磁盘文件, 只需要调用这个函数, 数据自动被写入到磁盘文件中
// -- 需要提供流对象/文件描述符关联一个磁盘文件
// 将数据序列化写入到磁盘文件中, c++ 风格

// ostream 子类 ofstream -> 写文件
bool SerializeToOstream(std::ostream* output) const;

// 将数据序列化写入到磁盘文件中, c 风格
bool SerializeToFileDescriptor(int file_descriptor) const;
```

### 3.3 反序列化

反序列化是指将序列化后的二进制数据重新转换为原始的数据结构或对象的过程。通过反序列化，我们可以将之前序列化的数据重新还原为其原始的形式，以便进行数据的读取、操作和处理。

Protobuf 中为我们提供了相关的用于数据序列化的 API，如下所示：

```c++
// 头文件目录: google\protobuf\message_lite.h

bool ParseFromString(const std::string& data) ;
bool ParseFromArray(const void* data, int size);

// istream -> 子类 ifstream -> 读操作
// wo ri
// w->写 o: ofstream , r->读 i: ifstream
bool ParseFromIstream(std::istream* input);
bool ParseFromFileDescriptor(int file_descriptor);
```

# 4 示例程序

### 4.1 .proto文件

**Color.proto**

```protobuf
syntax = "proto3";

//添加包 ColorPackage 
package ColorPackage;

enum Color
{
    Red = 0;
    Green = 3;		
    Yellow = 6;
    Blue = 9;
}
```

**Person.proto**

```protobuf
syntax = "proto3";
import "Color.proto";

//添加包 PersonPackage 
package PersonPackage;

message Person
{
    int32 id = 1;
    bytes name = 2;
    bytes sex = 3;	
    repeated bytes address = 4;
    int32 age = 5;
    // 添加信息, 使用的是外部proto文件中定义的数据类型
    // 如果这个外边类型属于某个包, 语法格式:
    // 包的名字.类名 变量名=编号;
    ColorPackage.Color color = 6;
}
```

### 4.2 测试文件

**demo.cpp**

```c++
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
    p1.add_address();
    p1.add_address("GDUFE");
    p1.add_address("CSAU");
    p1.set_age(22);
    p1.set_color(ColorPackage::Color::Red);

    
    //因为protobuf序列化之后是以二进制格式保存数据的，直接输出大概率是乱码
    p1.SerializeToString(&str);
    std::cout << "序列化结果： " << "\n"
              << str << std::endl;
    //因为protobuf序列化之后是以二进制格式保存数据的，直接输出大概率是乱码

    
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
```

### 4.3 Makefile

```makefile
#仅参考
PROTOC=protoc
GPP=g++
PROTO_DIR=/home/asuiiiyi/Documents/protocol_buffer/test
OUTPUT_DIR=/home/asuiiiyi/Documents/protocol_buffer

all: generate compile

generate:
	$(PROTOC) --proto_path=$(PROTO_DIR) $(PROTO_DIR)/Person.proto --cpp_out=$(PROTO_DIR) && $(PROTOC) --proto_path=$(PROTO_DIR) $(PROTO_DIR)/Color.proto --cpp_out=$(PROTO_DIR)
compile:
	$(GPP) -o $(OUTPUT_DIR)/demo $(OUTPUT_DIR)/demo.cpp $(PROTO_DIR)/Person.pb.cc $(PROTO_DIR)/Color.pb.cc -std=c++11 -lprotobuf -L/usr/lib

clean:
	rm -f $(PROTO_DIR)/Person.pb.h $(PROTO_DIR)/Person.pb.cc $(PROTO_DIR)/Color.pb.h $(PROTO_DIR)/Color.pb.cc $(OUTPUT_DIR)/demo

```

### 4.4 测试结果

```shell
[asuiiiyi@localhost protocol_buffer]$ make
protoc --proto_path=/home/asuiiiyi/Documents/protocol_buffer/test /home/asuiiiyi/Documents/protocol_buffer/test/Person.proto --cpp_out=/home/asuiiiyi/Documents/protocol_buffer/test && protoc --proto_path=/home/asuiiiyi/Documents/protocol_buffer/test /home/asuiiiyi/Documents/protocol_buffer/test/Color.proto --cpp_out=/home/asuiiiyi/Documents/protocol_buffer/test
g++ -o /home/asuiiiyi/Documents/protocol_buffer/demo /home/asuiiiyi/Documents/protocol_buffer/demo.cpp /home/asuiiiyi/Documents/protocol_buffer/test/Person.pb.cc /home/asuiiiyi/Documents/protocol_buffer/test/Color.pb.cc -std=c++11 -lprotobuf -L/usr/lib
[asuiiiyi@localhost protocol_buffer]$ ./demo 
序列化结果： 
张三man"GDUFE"CSAU(
反序列化结果：
id: 1
姓名：张三
性别：man
年龄：22
颜色：0
地址：GDUFE
地址：CSAU
```

