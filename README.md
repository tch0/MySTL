## 个人STL实现

### 介绍
- 部分参考《STL源码剖析》（也即是SGI STL）实现的C++ STL，细节有很多简化，不重性能，只为学习，Headr-Only，即包即用。
- 全部内容均定义在`namespace tstd`中，其中内部实现位于`namespace tstd::impl`。
- 所有头文件包含在`include/`目录中。
- 所有已实现内容均有完善的正确性测试和与编译器标准实现(主要是libstdc++)的效率对比测试，包含在`test/`目录中。
- STL仅为标准库的中最重要的组成部分，并不包含C++标准库中的非STL部分，比如IO、本地化、时间日期、正则等。
- STL六大组件：容器、迭代器、适配器、函数对象、分配器。
- 书中内容是有一点过时的，这里大部分会实现到C++17或者C++20标准甚至C++23（但不会是全部），已经废弃的内容可能不会选择实现，将要废弃的内容会做注释但通常会实现。
- 使用C++17标准实现，不使用任何操作系统接口，无任何编译配置。会使用一部分非STL标准库的内容，理论上可以在任何支持标准C++的编译器上成功编译。
- 很生僻的东西可能不会选择实现（可能压根实现不了），比如[std::pointer_safety](https://zh.cppreference.com/w/cpp/memory/gc/pointer_safety)之类。
- 头文件全部命名为`txxx.hpp`，其中`xxx`对应标准库中的头文件名，以示区分。如`<vector>`对应于`<tvector.hpp>`。
- 内部实现文件以`tstl_`为前缀，不需要关注。
- 为了与STL良好配合、专注于重点、避免繁琐冗余的实现，迭代器种类和所有的类型特性(type traits)都使用现有STL内容。
- `std::initializer_list`由编译器直接提供了一定支持，直接使用，不提供实现，况且也无法自行实现所有功能。
- 为简化实现复杂度，不考虑异常安全。
- 算法实现中不考虑任何与执行策略、并行算法、向量化有关的内容，所有算法均为顺序执行。
- 算法实现保证正确性和复杂度满足要求，实现怎么简单怎么来，基本没有做任何优化。

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

|头文件|实现的内容|
|:-:|:-
|[`<tmemory.hpp>`](https://github.com/tch0/MySTL/blob/master/include/tmemory.hpp)<br/>对应于<br/>[`<memory>`](https://zh.cppreference.com/w/cpp/header/memory)|分配器：`allocator` <br/>智能指针：`unique_ptr`, `shared_ptr`, `weak_ptr` <br/>辅助类：`owner_less`, `enable_shared_from_this`, `bad_weak_ptr`, `default_delete`, `tstd::hash<tstd::unique_ptr>`, `tstd::hash<tstd::shared_ptr>` <br/>未初始化存储算法：`uninitialized_copy`, `uninitialized_copy_n`, `uninitialized_fill`, `uninitialized_fill_n`, `uninitialized_move`, `uninitialized_move_n`, `construct_at`, `destroy_at`, `destroy`<br/>智能指针非成员操作：`make_unique`, `operator ==/!=/</<=/>/>=`, `make_shared`, `operator<<`, `get_deleter`, `tstd::swap`
|[`<titerator.hpp>`](https://github.com/tch0/MySTL/blob/master/include/titerator.hpp)<br/>对应于<br/>[`<iterator>`](https://zh.cppreference.com/w/cpp/header/iterator)|原语：`iterator` <br/>适配器类型：`reverse_iterator`, `move_iterator`, `back_insert_iterator`, `front_insert_iterator`, `insert_iterator`<br/>适配器函数：`make_reverse_iterator`, `make_move_iterator`, `front_inserter`, `back_inserter`, `inserter`<br/>迭代器操作：`advance`, `distance`, `next`, `prev`<br/>范围访问：`begin/cbegin`, `end/cend`, `rbegin/crbegin`, `rend/crend`, `size`, `empty`, `data`<br/>其他东西直接使用标准库版本
|[`<tvector.hpp>`](https://github.com/tch0/MySTL/blob/master/include/tvector.hpp)<br/>对应于<br/>[`<vector>`](https://zh.cppreference.com/w/cpp/header/vector)|类：`vector`, `vector<bool>`特化则并未实现<br/>函数：`operator ==/!=/</<=/>/>=`
|[`<tarray.hpp>`](https://github.com/tch0/MySTL/blob/master/include/tarray.hpp)<br/>对应于<br/>[`<array>`](https://zh.cppreference.com/w/cpp/header/array)|类：`array`<br/>函数：`operator ==/!=/</<=/>/>=,` `tstd::swap`, `to_array`, `tstd::get`
|[`<tlist.hpp>`](https://github.com/tch0/MySTL/blob/master/include/tlist.hpp)<br/>对应于<br/>[`<list>`](https://zh.cppreference.com/w/cpp/header/list)|类：`list`<br/>函数：`operator ==/!=/</<=/>/>=`, `tstd::swap`
|[`<tforward_list.hpp>`](https://github.com/tch0/MySTL/blob/master/include/tforward_list.hpp)<br/>对应于<br/>[`<forward_list>`](https://zh.cppreference.com/w/cpp/header/forward_list)|类：`forward_list`<br/>函数：`operator ==/!=/</<=/>/>=`, `tstd::swap`
|[`<tdeque.hpp>`](https://github.com/tch0/MySTL/blob/master/include/tdeque.hpp)<br/>对应于<br/>[`<deque>`](https://zh.cppreference.com/w/cpp/header/deque)|类：`deque`<br/>函数：`operator ==/!=/</<=/>/>=`, `tstd::swap`
|[`<tstack.hpp>`](https://github.com/tch0/MySTL/blob/master/include/tstack.hpp)<br/>对应于<br/>[`<stack>`](https://zh.cppreference.com/w/cpp/header/stack)|类：`stack`<br/>函数：`operator ==/!=/</<=/>/>=`, `tstd::swap`
|[`<tqueue.hpp>`](https://github.com/tch0/MySTL/blob/master/include/tqueue.hpp)<br/>对应于<br/>[`<queue>`](https://zh.cppreference.com/w/cpp/header/queue)|类：`queue`, `priority_queue`<br/>函数：`operator ==/!=/</<=/>/>=`, `tstd::swap`
|[`<tset.hpp>`](https://github.com/tch0/MySTL/blob/master/include/tset.hpp)<br/>对应于<br/>[`<set>`](https://zh.cppreference.com/w/cpp/header/set)|类：`set`, `multiset`<br/>函数：`operator ==/!=/</<=/>/>=`, `tstd::swap`
|[`<tmap.hpp>`](https://github.com/tch0/MySTL/blob/master/include/tmap.hpp)<br/>对应于<br/>[`<map>`](https://zh.cppreference.com/w/cpp/header/map)|类：`map`, `multimap`<br/>函数：`operator ==/!=/</<=/>/>=`, `tstd::swap`
|[`<tunordered_set.hpp>`](https://github.com/tch0/MySTL/blob/master/include/tunordered_set.hpp)<br/>对应于<br/>[`<unordered_set>`](https://zh.cppreference.com/w/cpp/header/unordered_set)|类：`unordered_set`, `unordered_multiset`<br/>函数：`operator ==/!=`, `tstd::swap`
|[`<tunordered_map.hpp>`](https://github.com/tch0/MySTL/blob/master/include/tunordered_map.hpp)<br/>对应于<br/>[`<unordered_map>`](https://zh.cppreference.com/w/cpp/header/unordered_map)|类：`unordered_map`, `unordered_multimap`<br/>函数：`operator ==/!=`, `tstd::swap`
|[`<talgorithm.hpp>`](https://github.com/tch0/MySTL/blob/master/include/talgorithm.hpp)<br/>对应于<br/>[`<algorithm>`](https://zh.cppreference.com/w/cpp/header/algorithm)|不修改序列算法：`all_of`, `any_of`, `none_of`, `for_each`, `for_each_n`, `count`, `count_if`, `mismatch`, `find`, `find_if`, `find_if_not`, `find_end`, `find_first_of`, `adjacent_find`, `search`, `search_n`<br/>修改序列算法：`copy`, `copy_if`, `copy_n`, `copy_backward`, `move`, `move_backward`, `fill`, `fill_n`, `transform`, `generate`, `generate_n`, `remove`, `remove_if`, `remove_copy`, `remove_copy_if`, `replace`, `replace_if`, `replace_copy_if`, `swap`, `iter_swap`, `reverse`, `reverse_copy`, `rotate`, `rotate_copy`, `shift_lfet`, `shift_right`, `random_shuffle`, `shuffle`, `sample`, `unique`, `unique_copy`<br/>划分算法：`is_partitioned`, `partition`, `partition_copy`, `stable_partition`, `partition_point`<br/>排序算法：`is_sorted`, `is_sorted_until`, `sort`, `partial_sort`, `partial_sort_copy`, `stable_sort`, `stable_sort`, `nth_element`<br/>二分查找算法：`lower_bound`, `upper_bound`, `binary_search`, `equal_range`<br/>已排序范围算法：`merge`, `inplace_merge`<br/>集合算法：`includes`, `set_difference`, `set_intersection`, `set_symmetric_difference`, `set_union`<br/>堆算法：`is_heap`, `is_heap_until`, `make_heap`, `push_heap`, `pop_heap`, `sort_heap`<br/>最大最小值算法：`max`, `max_element`, `min`, `min_element`, `minmax`, `minmax_element`, `clamp`<br/>比较算法：`equal`, `lexicographical_compare`, `lexicographical_compare_three_way`<br/>排列算法：`is_permutation`, `next_permutation`, `prev_permutation`

### TODO

- `<functional>`内容实现。
- 智能指针实现。
- 容器与常用算法的效率测试。