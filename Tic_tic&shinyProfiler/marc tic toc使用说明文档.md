tic-toc使用说明

### 输出说明

> double et = clock() - td->starttime;starttime为tic里面的当前时钟，

1. name: 测试的函数名称

1. ncalls:调用了几次函数

1. totaltime:程序运行的总时间，把每次跑的时间进行累加，即`td->totaltime += et;`

1. maxtime:单次调用的最长时间，即`td->maxtime = et > td->maxtime ? et : td->maxtime;`

1. mintime: 单次调用的最短时间，即`td->mintime = et < td->mintime ? et : td->mintime;`

1. avg time: 平均时间，`(1.0 * p.second.totaltime) / (p.second.ncalls *tictoc::TICKS_PER_SEC)`

1. numblowntics：在没有toc的情况下连续调用tic两次，该报告将在"numblowntics"字段中记录此情况发生的次数，注意其中的numblowntics必须为0，如果不为0，则说明tic-toc没有配对使用，用户需要检查代码。

    1. numblowntics用标志位`bool ticked;`、`ticked`值默认为`false`,在连续调用`tic`时会把`ticked`值更新为`ture`;若`ticked`为`ture`就计数，计数的功能只在`tic`函数中实现；

    1. 在没有使用`tic`的情况下调用`toc`会向系统直接返回值为`-2`的警告值

1. 统计并发状态下的总程序运行时间：

![](https://tb.orbbec.com:8080/storage/012db17de4588b37ef29b8daa1be42bc8a07?Signature=eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJBcHBJRCI6IjU5Mzc3MGZmODM5NjMyMDAyZTAzNThmMSIsIl9hcHBJZCI6IjU5Mzc3MGZmODM5NjMyMDAyZTAzNThmMSIsIl9vcmdhbml6YXRpb25JZCI6IiIsImV4cCI6MTY0MjU4NDYwNCwiaWF0IjoxNjQxOTc5ODA0LCJyZXNvdXJjZSI6Ii9zdG9yYWdlLzAxMmRiMTdkZTQ1ODhiMzdlZjI5YjhkYWExYmU0MmJjOGEwNyJ9.ijnqxlY1n2zu_M4QHLP4snyGbsyBibco5vcPiBe82WA&download=image.png "")

1. 统计并发状态下每个线程运行的时间

![](https://tb.orbbec.com:8080/storage/012d6baf82d2b69fc5a913ab62db35c9d144?Signature=eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJBcHBJRCI6IjU5Mzc3MGZmODM5NjMyMDAyZTAzNThmMSIsIl9hcHBJZCI6IjU5Mzc3MGZmODM5NjMyMDAyZTAzNThmMSIsIl9vcmdhbml6YXRpb25JZCI6IiIsImV4cCI6MTY0MjU4NDYwNCwiaWF0IjoxNjQxOTc5ODA0LCJyZXNvdXJjZSI6Ii9zdG9yYWdlLzAxMmQ2YmFmODJkMmI2OWZjNWE5MTNhYjYyZGIzNWM5ZDE0NCJ9.s7jhAi_nab2cKXuZr6IO1F_ZiK3Ontqx5MyX7MabWxg&download=image.png "")

1. tic-toc统计不同函数的机制：

/* 使用字典存放tic时间戳，每次tic的时候都会初始化一次;

 * 字典的键由函数名组成，针对每一个函数都会有一个自己的计时器;

 * 之后运行toc的时候，也会检查当前的函数名，从而与对应的tic时间戳相减

 */

![](https://tb.orbbec.com:8080/storage/012d4de8721e477e506649d55653e3f49cd9?Signature=eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJBcHBJRCI6IjU5Mzc3MGZmODM5NjMyMDAyZTAzNThmMSIsIl9hcHBJZCI6IjU5Mzc3MGZmODM5NjMyMDAyZTAzNThmMSIsIl9vcmdhbml6YXRpb25JZCI6IiIsImV4cCI6MTY0MjU4NDYwNCwiaWF0IjoxNjQxOTc5ODA0LCJyZXNvdXJjZSI6Ii9zdG9yYWdlLzAxMmQ0ZGU4NzIxZTQ3N2U1MDY2NDlkNTU2NTNlM2Y0OWNkOSJ9.3SDlxPwsmw2N91P92Fqk31UdZebwgNObdxV3TTUCFZg&download=image.png "")

1. C++中定义的一些宏

```text
__FILE__ : 宏所在的文件名
__FUNCTION__ : 宏所在的函数名
__LINE__ : 当前行号
__DATE__, __TIME__ : 最后一次编译的时间
__TIMESTAMP__ : 文件最后的修改时间
```

1. 使用匿名命名空间namespace的原因

不用担心不同的库之间的冲突。所有的库想依赖该文件，都会直接include它，而include操作其实就是简单的copy文件的内容，所以这段代码就会进入每个文件自身中，成为其源码的一部分。如此，只要我们把单例维护的代码放在匿名命名空间中，就可以保证其在每个文件中有且只有一个。

![](https://tb.orbbec.com:8080/storage/012d84a4f1d67655ac93c62ff590626716fe?Signature=eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJBcHBJRCI6IjU5Mzc3MGZmODM5NjMyMDAyZTAzNThmMSIsIl9hcHBJZCI6IjU5Mzc3MGZmODM5NjMyMDAyZTAzNThmMSIsIl9vcmdhbml6YXRpb25JZCI6IiIsImV4cCI6MTY0MjU4NDYwNCwiaWF0IjoxNjQxOTc5ODA0LCJyZXNvdXJjZSI6Ii9zdG9yYWdlLzAxMmQ4NGE0ZjFkNjc2NTVhYzkzYzYyZmY1OTA2MjY3MTZmZSJ9.mpM4KEI9ERaGg5Vr7EJV71C1lKG-10GrJEbfEa6lV60&download=image.png "")

```text
 -
   name: ThreadWorker
   ncalls: 1
   totaltime: 0.030157
   maxtime: 0.030157
   mintime: 0.030157
   avg time: 0.030157
   numblowntics: 1
 -
   name: cal
   ncalls: 10
   totaltime: 0.4511
   maxtime: 0.090144
   mintime: 6.4e-05
   avg time: 0.04511
   numblowntics: 0
 -
   name: start_threads
   ncalls: 1
   totaltime: 0.040432
   maxtime: 0.040432
   mintime: 0.040432
   avg time: 0.040432
   numblowntics: 0
 ​
  average time for start_threads: 0.040432
```

```text
 -
   name: ThreadWorker
   ncalls: 2
   totaltime: 0.070274
   maxtime: 0.040147
   mintime: 0.030127
   avg time: 0.035137
   numblowntics: 0
 -
   name: start_threads
   ncalls: 2
   totaltime: 0.070709
   maxtime: 0.040336
   mintime: 0.030373
   avg time: 0.0353545
   numblowntics: 0
 ​
  average time for start_threads: 0.0353545
```

![](https://tb.orbbec.com:8080/storage/012dca33a26bc9a52db08d2cf187872c150e?Signature=eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJBcHBJRCI6IjU5Mzc3MGZmODM5NjMyMDAyZTAzNThmMSIsIl9hcHBJZCI6IjU5Mzc3MGZmODM5NjMyMDAyZTAzNThmMSIsIl9vcmdhbml6YXRpb25JZCI6IiIsImV4cCI6MTY0MjU4NDYwNCwiaWF0IjoxNjQxOTc5ODA0LCJyZXNvdXJjZSI6Ii9zdG9yYWdlLzAxMmRjYTMzYTI2YmM5YTUyZGIwOGQyY2YxODc4NzJjMTUwZSJ9.6pMuli7W2P6mKekl6ScUQiBdfXP5n_jvs2j_ZCfgSGA&download=image.png "")



### 和hpc的区别

1. 统计时把微秒换成毫秒

1. 修复内存泄漏的bug

1. 增加定义 `_TICTOC_TOC_CODE(str) TICTOC::timer().toc(str);`

1. 增加互斥锁：`std::recursive_mutex mTicTocMutex;`

***

设计思路

计时器，定义两个宏TIC和TOC，如果插入TIC，则记录为起始时间，当插入TOC的时候，则计算与上一次TIC之间的时间，并打印出来。

在全局定义一个时间的变量，但这样会带来另一个问题，就是所有函数都共享这个变量，如果函数内部再运行一次TIC，会覆盖掉这个时间戳，但是其他的TOC的结果不直观。

所以，就使用了一个字典，来存放TIC的时间戳。这个字典本身是使用单例模式去生成和维护的。每次TIC的时候都会初始化一次它，但是由于是单例，所以只有第一次会耗时。而字典的键是个字符串，由函数名组成。针对每个函数，都会有自己的一个计时器，就不用担心冲突了。之后运行TOC的时候，也会检查当前的函数名，从而与对应的TIC时间戳相减。

采用匿名命名空间解决动态库和主程序使用相同字典的问题，在每个函数中都生成自己的单例。

### 一、使用不同的宏控制程序

因为我们不希望在Deliver的时候，再修改代码，所以有没有办法，使用不同的宏来控制我们的程序呢？当然是可以的。C/C++最常用到的预处理语句：

eg:

```text
 #ifndef _TICTOC_H
 #define _TICTOC_H
 ​
 #if 1
     /* #if true
      * 执行
      * #endif
      * macros to make it easy to track a function execution time
      * just put _TICTOC_TIC_FUNC at the beginning of the function call
      * and put _TICTOC_TOC_FUNC right before any return statement
      */
     #define _TICTOC_TIC_FUNC TICTOC::timer().tic(__func__);
     #define _TICTOC_TOC_FUNC TICTOC::timer().toc(__func__);
 ​
     //add by hpc fengrusong
     #define _TICTOC_TIC_CODE(str) TICTOC::timer().tic(str);
     #define _TICTOC_TOC_CODE(str) TICTOC::timer().toc(str);
 ​
 #else
     //if you want to disable marc tic-toc,just enalbe the following macro defines
     #define _TICTOC_TIC_FUNC
     #define _TICTOC_TOC_FUNC
 ​
     //add by hpc fengrusong
     #define _TICTOC_TIC_CODE(str)
     #define _TICTOC_TOC_CODE(str)
 #endif
```

### 二、多种精度

在`Time.h`文件中定义多种精度的函数，在`tictoc.cpp`中直接调用

![](https://tb.orbbec.com:8080/storage/012de4fcffdd2eb1f36d6f0612a13e878606?Signature=eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJBcHBJRCI6IjU5Mzc3MGZmODM5NjMyMDAyZTAzNThmMSIsIl9hcHBJZCI6IjU5Mzc3MGZmODM5NjMyMDAyZTAzNThmMSIsIl9vcmdhbml6YXRpb25JZCI6IiIsImV4cCI6MTY0MjU4NDYwNCwiaWF0IjoxNjQxOTc5ODA0LCJyZXNvdXJjZSI6Ii9zdG9yYWdlLzAxMmRlNGZjZmZkZDJlYjFmMzZkNmYwNjEyYTEzZTg3ODYwNiJ9.s4Oz4Hg99tj60rIl3PYAzOLxzDgG2Phz42tiMawW_X8&download=image.png "")

### 三、打印完整的信息

这个时间戳所在的位置，包括：函数名

```text
 __FILE__ : 宏所在的文件名
 __FUNCTION__ : 宏所在的函数名
 __LINE__ : 当前行号
 __DATE__, __TIME__ : 最后一次编译的时间
 __TIMESTAMP__ : 文件最后的修改时间
```

### 四、使用匿名命名空间namespace的原因

不用担心不同的库之间的冲突。所有的库想依赖该文件，都会直接include它，而include操作其实就是简单的copy文件的内容，所以这段代码就会进入每个文件自身中，成为其源码的一部分。如此，只要我们把单例维护的代码放在匿名命名空间中，就可以保证其在每个文件中有且只有一个。



***

### 单例模式

- 单例类只能有一个实例。

- 单例类必须自己创建自己的唯一实例。

- 单例类必须给所有其他对象提供这一实例。



***

