/* 
 * File:   tictoc.h
 * Author: Marc
 *
 * Created on December 3, 2009, 12:07 PM
 * (C) Marc Gershow; licensed under the Creative Commons Attribution Share Alike 3.0 United States License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/3.0/us/ or send a letter to
 * Creative Commons, 171 Second Street, Suite 300, San Francisco, California, 94105, USA.
 */

#ifndef _TICTOC_H
#define	_TICTOC_H

/* macros to make it easy to track a function execution time
 * just put _TICTOC_TIC_FUNC at the beginning of the function call
 * and put _TICTOC_TOC_FUNC right before any return statement
 */
#define _TICTOC_TIC_FUNC TICTOC::timer().tic(__func__);
#define _TICTOC_TOC_FUNC TICTOC::timer().toc(__func__);

#include <map>
#include <cstring>
#include <string>
class Timer;


// 匿名命名空间
namespace TICTOC {
    class tictoc {
    public:

        /* error return values for toc command
            返回toc命令的值时出错
         * NOT_FOUND == you called toc on "name" without ever calling tic on "name" first
         你在“名字”上打电话给toc，而没有先在“名字”上打电话给tic
         * NOT_TICKED == although you called tic on "name", you called toc on "name" twice in a row
         虽然你在“name”上叫过tic，但你在“name”上连续叫过toc两次
         */
        static const int NOT_FOUND = -1;
        static const int NOT_TICKED = -2;
        const static constexpr double TICKS_PER_SEC = 1E6;
        /* 如果某个静态成员的应用场景仅限于编译器可以替换它的值的情况，
         * 则一个初始化的const或constexpr static不需要分别定义。
         相反，若将它用于值不能替换的场景中，则该成员必须有一条定义语句。

         即使一个常量静态数据成员在类内部被初始化了,
         通常情况下也应该在类的外部定义一些该成员
         */
        /* tictoc()
         * ~tictoc()
         * constructor takes no arguments
         * 构造函数不接受参数
         */
        tictoc();
        virtual ~tictoc();

        /* enable(true) turns logging on
        启用（true）打开日志记录
         * enable(false) turns logging off
         *
         * tic and toc will return immediately without doing anything
         * tic和toc会立即返回，无需任何操作
         * if state = false;
         *
         */
        void enable(bool state);


        /* clear()
         *
         * resets tictoc to original state, clearing all information about previously
         * 将tictoc重置为原始状态，清除以前的所有信息
         * called tics and tocs
         */
        void clear();

        /* void tic (const string &name, bool notick = false);
         * void tic (const char *name, bool notick = false);
         *
         * if notick is true, returns with no effect
         * 如果notick为true，则返回而不生效
         *
         * calling tic starts a timer running associated with "name" (case sensitive)
         * when toc("name") is called, you get the number of clock cycles (usually ms) since
         * tic("name") was last called
         *调用tic启动与“name”（区分大小写）相关联的计时器。当调用toc（“name”）时，
         * 您将获得自上次调用tic（“name”）以来的时钟周期数（通常为毫秒）
         *
         * if tic is called twice in a row without a toc, we reset the starting timer
         * generateReport will note the number of times this happens in the field "numblowntics"
         * 如果在没有toc的情况下连续调用tic两次，我们将重置启动计时器生成器报告，该报告将在"numblowntics"字段中记录此情况发生的次数
         * 注意其中的numblowntics必须为0，如果不为0，则说明tic-toc没有配对使用，用户需要检查代码
         */
        
        void tic (const std::string &name, bool notock = false);    
        void tic (const char *name, bool notock = false);

        /* long toc(const string &name, bool notock = false);
         * long toc(const char *name, bool notock = false);         *
         *
         * if notock is true, returns 0 immediately with no other effect
         * 如果NOTCK为真，则立即返回0，没有其他效果
         *
         * when toc("name") is called, we stop the timer associated with "name"
         * and return the number of clock cycles (usually ms) since tic("name") was called
         * you can convert to seconds using CLOCKS_PER_SEC
         * 当调用toc("name")时，我们停止与"name"关联的计时器，并返回时钟周期数(通常为毫秒),
         * 因为调用了tic("name")，可以使用CLOCKS_PER_SEC转换为 秒
         *
         * if toc("name") is called prior to tic("name"), return value will be NOT_FOUND
         * 如果在tic("name")之前调用toc("name")，则返回值将为NOT_FOUND
         * if tic("name") is called but then toc("name") is called two or more times in succession,
         * return value will be NOT_TICKED
         * 如果调用了toc("name")，但随后连续调用了两次或多次toc("name")，则返回值将为NOT_TICKED
         */

        double toc(const std::string &name, bool notock = false);
        double toc(const char *name, bool notock = false);

        /* double getStatistics (const std::string &name);
         * double getStatistics (const std::string &name, int &ncalls, double &totaltime);
         * double getStatistics (const std::string &name, int &ncalls, double &totaltime, double &maxtime, double &mintime, int &numblowntics);
         * returns average time per call for given name
         * 返回给定名称每次调用的平均时间
         * more stastics can be gathered by passing additional output variables
         * 通过传递额外的输出可以收集更多的静态信息
         *
         * return value less than 0 indicates an error
         * 返回值小于0说明出错
         * 
         */
        // 可变参数 Statistic 统计
        // 自检: 注意其中的numblowntics必须为0，如果不为0，则说明tic-toc没有配对使用，用户需要检查代码
        double getStatistics (const std::string &name) const;
        double getStatistics (const std::string &name, int &ncalls, double &totaltime) const;
        double getStatistics (const std::string &name, int &ncalls, double &totaltime, double &maxtime, double &mintime, int &numblowntics) const;

        std::string generateReport() const;
        char *generateReportCstr() const;
        //static tictoc *timer = new tictoc();

    private:
        double clock();

        tictoc(const tictoc& orig);
        std::map <std::string, struct _tictoc_data> tt;
        Timer *tim;
        bool enabled;
    };

    /* tictoc timer
     * 
     * we declare a globally accessible static instance of tictoc
     * 声明一个全局可访问的tictoc静态实例
     * TICTOC::timer for convenience.  This way, you can call tic and toc from 
     * multiple libraries and have all the data retained in a single structure
     * without having to pass a pointer to a tictoc object around
     * 通过这种方式，可以从多个库调用tic和toc，并将所有数据保留在一个结构中，
     * 而不必传递一个指向tictoc对象的指针。
     */
    tictoc& timer();
   
    
}

#endif	/* _TICTOC_H */

