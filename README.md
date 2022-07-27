## 个人STL实现

### 介绍
- 以《STL源码剖析》（也即是SGI STL）为蓝本实现的C++ STL库，不重性能，只为学习，Headr-Only，即包即用。
- 全部内容均定义在`namespace tstd`中。
- 所有头文件包含在`include/`目录中。
- 所有已实现内容均有完善的正确性测试和与编译器标准实现(主要是libstdc++)的效率对比测试，包含在`test/`目录中。
- STL仅为标准库的中最重要的组成部分，并不包含C++标准库中的非STL部分，比如IO、本地化、时间日期、正则等。
- STL六大组件：容器、迭代器、适配器、函数对象、分配器。
- 书中内容是有一点过时的，这里会至少实现到C++11或者14的东西，已经废弃的内容可能不会选择实现，将要废弃的内容会做注释但会实现。
- 使用C++17标准实现，不使用任何操作系统接口，无任何编译配置。会使用一部分非STL标准库的内容，理论上可以在任何支持标准C++的编译器上成功编译。
- 很生僻的东西可能不会选择实现（可能压根实现不了），比如[std::pointer_safety](https://zh.cppreference.com/w/cpp/memory/gc/pointer_safety)之类。
- 头文件全部命名为`txxx.hpp`，其中`xxx`对应标准库中的头文件名，以示区分。如`<vector>`对应于`<tvector.hpp>`。
- 内部实现文件以`tstl_`为前缀，不需要关注。

### 运行正确性测试

```sh
cd ./test
make run_all_tests # show summary informations
make run_all_tests_in_detail # show detail informations
```
注：测试代码中使用了部分C++20的特性，需要编译器支持C++20（GCC11/12，Clang15，MSVC2019）。
- [安装最新版本的GCC](https://github.com/tch0/notes/blob/master/CppToolChain.md)

### 运行效率测试
```sh
cd ./test
make run_all_eff_tests
make run_all_eff_tests_in_detail
```

### 已实现内容列表

C++ 标准模板库的完整内容见[Cpp_STL_ReferenceManual.pdf](https://www.cppreference.com/Cpp_STL_ReferenceManual.pdf)。

详表：
|头文件|实现的内容|
|:-:|:-
|`<tmemory.hpp>` <br/>对应于 [`<memory>`](https://zh.cppreference.com/w/cpp/header/memory)|分配器：allocator <br/>智能指针：unique_ptr, shared_ptr, weak_ptr <br/>辅助类：owner_less, enable_shared_from_this, bad_weak_ptr, default_delete, std::hash\<std::unique_ptr\>, std::hash\<std::shared_ptr\> <br/>未初始化存储：uninitialized_copy, uninitialized_copy_n, uninitialized_fill, uninitialized_fill_n <br/>智能指针非成员操作：make_unique, operator ==/!=/</<=/>/>=(std::unique_ptr, std::shared_ptr), make_shared, operator<<(std::shared), get_deleter, std::swap(std::unique_ptr), std::swap(std::weak_ptr), std::swap(std::shared_ptr)
|||