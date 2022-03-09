/*
The MIT License

Copyright (c) 2007-2010 Aidin Abedi http://code.google.com/p/shinyprofiler/

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#include "Shiny.h"
#include <thread>
#include <cstdio>

#ifdef _WIN32
#include <windows.h> // Sleep
#else // assume POSIX
#include <unistd.h> // usleep
#endif


/*---------------------------------------------------------------------------*/

void millisleep(unsigned int milliseconds) {
#ifdef _WIN32
	Sleep(milliseconds);
#else
    // usleep功能把进程挂起一段时间， 单位是微秒（百万分之一秒）。
	usleep(milliseconds * 1000);
#endif
}

const char* filename = "profile.txt";

/*---------------------------------------------------------------------------*/

namespace sample {

    void MainFunc1(void) {
    	PROFILE_FUNC(); // profile until end of block (only supported in c++)

    	millisleep(100);
    }

    void MainFunc2(void) {
    	PROFILE_FUNC(); // profile until end of block (only supported in c++)

    	millisleep(100);
    }

    void thread_helper() {
        PROFILE_FUNC();
        millisleep(500);
    }

    void ThreadWorker(int workerId) {
        thread_helper();
        char name[100];
        std::snprintf(name, 100, "Worker Thread #%d", workerId);

        PROFILE_SET_THREAD_NAME(name);

        millisleep(100 * workerId);
        // 刷新分析器
        PROFILE_UPDATE(); // update all profiles

        // 追加内容到文件最后一个字的末尾
        PROFILE_OUTPUT_APPEND(filename); // print to terminal
    }

}

/*---------------------------------------------------------------------------*/

void start_threads() {
    PROFILE_FUNC();

    std::thread t1(sample::ThreadWorker, 1);
    std::thread t2(sample::ThreadWorker, 2);

	sample::MainFunc1();
    sample::MainFunc2();

    t1.join();
    t2.join();
}

int main() {
    PROFILE_OUTPUT_CLEAR(filename);
    // 设置当前线程的名称为main
    PROFILE_SET_THREAD_NAME("main");

    start_threads();

	PROFILE_UPDATE(); // update all profiles

	PROFILE_OUTPUT_APPEND(filename); // print to terminal
    // PROFILE_OUTPUT(NULL); // print to terminal

    printf("Done.\n");
#ifdef _WIN32
	system("pause");
#endif
	return 0;
}
