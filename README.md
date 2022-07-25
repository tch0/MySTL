## 个人STL实现

介绍：
- 以《STL源码剖析》为蓝本实现的C++ STL库，不重性能，只为学习，Headr-Only，即包即用。
- 全部内容包含在`namespace mystd`中。
- 所有头文件包含在`include/`中。
- 所有已实现内容均有完善的正确性测试和效率对比测试，包含在`test/`中。
- STL仅为标准库的中最重要的组成部分，并不包含C++标准库中的非STL部分，比如IO、本地化、时间日期库、正则等。
- STL六大组件：容器、迭代器、适配器、函数对象、分配器。

运行正确性测试：
```sh
cd test
make run_all_tests # show summary informations
make run_all_tests_in_detail # show detail informations
```

运行效率测试：
```sh
cd test
make run_all_efficiency_tests
make run_all_efficiency_tests_in_detail
```
