https://github.com/samuellab/Marc-TicToc

## 测量函数开销的快速宏

/* macros to make it easy to track a function execution time

* just put _TICTOC_TIC_FUNC at the beginning of the function call
* and put _TICTOC_TOC_FUNC right before any return statement
  */
  #define _TICTOC_TIC_FUNC TICTOC::timer().tic(__func__);
  #define _TICTOC_TOC_FUNC TICTOC::timer().toc(__func__);

## sts log显示的时间单位是us

## 原理说明

TICKS_PER_SEC的作用是将秒转换为微秒
numblowntics的作用就是一个检查tic-toc有没有配对使用的计数器

## global accessible instance

/* tictoc timer
*
* we declare a globally accessible static instance of tictoc
* TICTOC::timer for convenience.  This way, you can call tic and toc from
* multiple libraries and have all the data retained in a single structure
* without having to pass a pointer to a tictoc object around
*/
tictoc& timer();

# run log

```
jason@DESKTOP-EG66ELI:/mnt/d/ligang/2022_frs_prj/tools/Marc-TicToc/pc_build$ ./Marc-TicToc
-
  name: all
  ncalls: 1
  totaltime: 0.163924
  maxtime: 0.163924
  mintime: 0.163924
  avg time: 0.163924
  numblowntics: 0
-
  name: delayOneClockIncrement
  ncalls: 10
  totaltime: 0.060457
  maxtime: 0.016006
  mintime: 0.001738
  avg time: 0.0060457
  numblowntics: 0
-
  name: delayms
  ncalls: 100
  totaltime: 0.100988
  maxtime: 0.001024
  mintime: 0.001004
  avg time: 0.00100988
  numblowntics: 0
-
  name: inner loop
  ncalls: 100
  totaltime: 0.102212
  maxtime: 0.001049
  mintime: 0.001009
  avg time: 0.00102212
  numblowntics: 0
-
  name: outer loop
  ncalls: 10
  totaltime: 0.163798
  maxtime: 0.026238
  mintime: 0.011968
  avg time: 0.0163798
  numblowntics: 0

-
  name: timer tic
  ncalls: 111
  totaltime: 0.000716
  maxtime: 2.4e-05
  mintime: 3e-06
  avg time: 6.45045e-06
  numblowntics: 0
-
  name: timer toc
  ncalls: 111
  totaltime: 0.000678
  maxtime: 2.4e-05
  mintime: 3e-06
  avg time: 6.10811e-06
  numblowntics: 0

average time for inner loop: 0.00102212
jason@DESKTOP-EG66ELI:/mnt/d/ligang/2022_frs_prj/tools/Marc-TicToc/pc_build$
```
