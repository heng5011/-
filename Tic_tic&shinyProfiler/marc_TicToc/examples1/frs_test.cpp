#include <stdlib.h>
#include <iostream>
#include "tictoc.h"
#include "Timer.h"

#include <thread>

#ifdef _WIN32
#include <windows.h> // Sleep
#else // assume POSIX
#include <unistd.h> // usleep
#endif

using namespace TICTOC;
using namespace std;



void millisleep(unsigned int milliseconds) {
#ifdef _WIN32
	Sleep(milliseconds);
#else
	usleep(milliseconds * 1000);
#endif
}

void cal(int val)
{
    _TICTOC_TIC_FUNC
    millisleep(10 * val);
    _TICTOC_TOC_FUNC
}


void ThreadWorker(int val)
{
    _TICTOC_TIC_FUNC
    // _TICTOC_TIC_CODE("ThreadWorker1111")

    printf("ThreadWorker called,input=%d\n", val);
    millisleep(10 * val);
    // _TICTOC_TOC_CODE("ThreadWorker1111")
    _TICTOC_TOC_FUNC
}

void start_threads() {

    _TICTOC_TIC_FUNC //统计的是最长线程运行的时间
    
    // 线程使用函数指针作为可调用参数
    // std::thread t1(ThreadWorker, 1);   
    std::thread t2(ThreadWorker, 2);
    std::thread t3(ThreadWorker, 3);
    // t3.join();
    // _TICTOC_TOC_FUNC //测的是否是thread这个类 ?

    // _TICTOC_TIC_FUNC
    // std::thread t4(ThreadWorker, 4);
 
    // t1.join();
    t2.join();
    t3.join();
    // t4.join();
    // t3.detach();
    // t4.detach();
   
    _TICTOC_TOC_FUNC
}

void ThreadWorker_new(int val)
{
    tictoc t;
    t.tic("ThreadWorker");
    // _TICTOC_TIC_FUNC
    // _TICTOC_TIC_CODE("ThreadWorker1111")

    printf("ThreadWorker called,input=%d\n", val);
    millisleep(10 * val);
    // _TICTOC_TOC_CODE("ThreadWorker1111")
    // _TICTOC_TOC_FUNC
    t.toc("ThreadWorker");
    cout << t.generateReportCstr();
    // t.getStatistics();
}

void t1()
{
    _TICTOC_TIC_FUNC
    std::thread t2(ThreadWorker_new, 2);
    std::thread t3(ThreadWorker_new, 3);
    t2.join();
    t3.join();
    _TICTOC_TOC_FUNC
}

int main() {    

    _TICTOC_TIC_FUNC

    start_threads();    
    // t1();
	
    // cal
    for(int i = 0; i < 10; i++)
        cal(i);
    
    
    printf("Done.\n");

    _TICTOC_TOC_FUNC

    cout << timer().generateReport();
    cout << "\n average time for start_threads: " << timer().getStatistics("start_threads") << "\n";

	return 0;
}