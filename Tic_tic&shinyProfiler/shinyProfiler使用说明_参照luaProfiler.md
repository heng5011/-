***

### shinyProfiler 使用说明

```text
   251  cd build
   252  mkdir build
   253  cd build/
   254  cmake ../
   255  make -j4
   256  ls
   257  cd bin/
   258  ls
   259  ./ShinySimpleSample 
```



***

先搞清 simple，第一个参数为什么大于100%？

- Self time 自己调用时间，函数的本次调用运行的时间，仅仅包含该函数本身的运行时间，不包含它所调用的子函数的时间。

- Total time (总调用时间，包括子项调用时间)函数的本次调用运行的**总**时间，包含它所调用的子函数的运行时间。

> “% Disk Time”计数器只不过是“Avg. Disk Queue Length”计数器乘以100，它是以不同比例显示的相同值。 如果“Avg. Disk Queue Length”为1，“% Disk Time”将为100； 如果“Avg. Disk Queue Length”为0.37，“% Disk Time”将为37。 这就是为什么你会看到“% Disk Time”大于100%的原因：只需要“Avg. Disk Queue Length”大于1。 相同的逻辑适用于“% Disk Read Time”和“% Disk Write Time”。 他们的数据分别来自“Avg. Disk Read Queue Length”和“Avg. Disk Write Queue Length”。PS：perfmon上对“Avg. Disk Queue Length”的中文描述为指读取和写入请求(为所选磁盘在实例间隔中列队的)的平均数。这么说来，“% Disk Time”和“Avg. Disk Queue Length”其实是一个东西，都是指读取和写入请求(为所选磁盘在实例间隔中列队的)的平均数（个人感觉根本没必要弄2个功能相同的计数器）。

输出说明

1. TotalTIme(ms)

- 函数从Call到Return总共消耗的时间，单位为毫秒

- 该值会受Profiler本身开销影响，主要是Profiler取Time的开销（因为Profiler取Time必须是同步的，但比如自增函数的开销比取Time开销还小，势必会造成误差，可以控制ProfileDepth来消除）

1. TotalTime(%)

- 该函数调用时间开销占父节点的百分比

- TotalTime / Parent.TotalTime

1. SelfTime(ms)

- 代表该函数自身的开销

- TotalTime减去各子节点的TotalTime

1. SelfTime(%)

- 自身开销占父节点的比重

- SelfTime / Parent.SelfTime

1. Average(ms)

- 平均每次调用耗时

- TotalTime / Calls

1. Alloc(kb)

- 函数调用期间开辟的内存，单位为kb

1. Alloc(%)

- 该函数开辟的内存占父节点的比重

- Alloc / Parent.Alloc

1. GC(kb)

- 函数调用期间释放的内存，因为Lua的GC为步进式，所以不能准确形容当前函数释放的内存。

- 但一级节点的Alloc - GC可以作为整个Lua的内存增量，可以用来观察代码是否有明显泄露，但具体定位还应该使用ELuaMemAnalyzer。

1. GC(%)

- 该函数调用期间发生的GC占父节点的比重

- GC / Parent.GC

1. Calls

- 该函数被调用次数

默认按照TotalTime(%)降序排列，也可按照调用树排列

[https://zhuanlan.zhihu.com/p/352419024](https://zhuanlan.zhihu.com/p/352419024)

### 函数调用关系图

基于gcc编译器输出RTL文本功能，解析其输出的RTL文件，从而解析出函数调用关系图

使用步骤

1. 首先在CmakeList.txt中设置gcc/g++的编译选项：

![](https://tb.orbbec.com:8080/storage/012dbbacd6c554c5728a8f36387c5e8977d4?Signature=eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJBcHBJRCI6IjU5Mzc3MGZmODM5NjMyMDAyZTAzNThmMSIsIl9hcHBJZCI6IjU5Mzc3MGZmODM5NjMyMDAyZTAzNThmMSIsIl9vcmdhbml6YXRpb25JZCI6IiIsImV4cCI6MTY0MjU4MTY5MSwiaWF0IjoxNjQxOTc2ODkxLCJyZXNvdXJjZSI6Ii9zdG9yYWdlLzAxMmRiYmFjZDZjNTU0YzU3MjhhOGYzNjM4N2M1ZTg5NzdkNCJ9.ubVQcen7LN1rwHeY7VQwKMuYKu3wifxvTRdl5uaiads&download=1.jpg "")

1. 执行以下命令即可生成对应图像

```text
 cd build/
 cmake ../
 make -j4
python funtrace.py  Main.cpp.229r.expand Advanced.cpp.229r.expand MainLoop.cpp.229r.expand Simple.cpp.229r.expand ThreadSample.cpp.229r.expand
 ​
 dot -Tsvg -Grankdir=LR -o funtrace.svg funtrace.dot
```

1. funtrace.py 源代码

```python
#!/usr/bin/env python

import sys
import os
import string
import re

funcs = []

ignor_calls = '__stack_chk_fail,__strcat_chk,__strcpy_chk,__sprintf_chk,__strrchr_chk,__memcpy_chk'\
'strlen,__strlen_chk,printf,__errno,__strchr_chk,__strncpy_chk'

class funcNode:
	def __init__(self, name, aname):
		self.__mName = name
		self.__altName = aname
		self.__mCallee = set()
		self.mCallers = 0;

	def addCallee(self, callee):
		self.__mCallee.add(callee)

	def getCalleeList(self):
		return self.__mCallee.copy()

	def getName(self):
		return [self.__mName, self.__altName]

	def incCaller(self):
		self.mCallers += 1;

	def callers(self):
		return self.mCallers;

'''
example of function declaration
;; Function const uint8_t* getCharbitmap480(char, uint32_t*, uint32_t*) (_Z16getCharbitmap480cPjS_, funcdef_no=23, decl_uid=6855, cgraph_uid=23)
'''
def addFunc(line):
	'''''found a new funcation.'''
	fs = 12
	fe = line.index(')') + 1
	fun = line[fs:fe]

	ans = line.rindex('(') + 1
	ane = line.find(',', ans)
	altname = line[ans:ane]
	return [fun, altname]

def getNodeByName(nodeList, name):
	for node in nodeList:
		if cmp(name, node.getName()[0]) == 0:
			return node
	return 0

def getNodeByaltName(nodeList, name):
	for node in nodeList:
		if cmp(name, node.getName()[1]) == 0:
			return node
	return 0

def getFunList(files):
	filep = open(files)
	fundef = ';; Function'
	print 'Functions List at ' + files
	for line in filep:
		if line.find(fundef) == 0:
			name = addFunc(line)
			print '\t' + name[0] + '\t' + name[1]
			n = funcNode(name[0], name[1])
			funcs.append(n)
	filep.close()

'''
example format of call_insn
(call_insn 22 21 23 4 (parallel [
            (set (reg:SI 0 r0)
                (call (mem:SI (symbol_ref:SI ("uevent_kernel_multicast_recv") [flags 0x41]  <function_decl 0x2b003cf7d500 uevent_kernel_multicast_recv>) [0 uevent_kernel_multicast_recv S4 A32])
                    (const_int 0 [0])))
            (use (const_int 0 [0]))
            (clobber (reg:SI 14 lr))
        ]) frameworks/av/cmds/camerad/cameraserver.cpp:590 -1
     (nil)
    (expr_list:REG_CFA_WINDOW_SAVE (use (reg:SI 2 r2))
        (expr_list:REG_CFA_WINDOW_SAVE (use (reg:SI 1 r1))
            (expr_list:REG_CFA_WINDOW_SAVE (use (reg:SI 0 r0))
                (nil)))))
'''
def parafile(files):
	'''''paras input file.'''

	print '\nFunctions call map at ' + files
	insn='(call_insn'
	fundef = ';; Function'
	filep = open(files)
	incall = 0;
	parten1 = re.compile('.*\("(\S+)"\)')
	parten2 = re.compile('.*function_decl\s+\w+\s+(.+)>')
	iline=0
	funcnod=0
	for line in filep:
		iline+=1
		if line.find(fundef) == 0:
			name = addFunc(line)
			funcnod = getNodeByName(funcs, name[0])
			print '\t Function::' + name[0]

		incall = line.find(insn)

		if (line.find('function_decl') > 0) and (line.find('symbol_ref') > 0):
			m = parten1.match(line)
			fname = m.group(1)
			calleenod = getNodeByaltName(funcs, fname)
			if calleenod == 0:
#				m = parten2.match(line)
#				fname = m.group(1)
				if ignor_calls.find(fname) < 0:
					calleenod = funcNode(fname, fname)
					print  '\t\t --> ' + str(iline) + '::N::' + fname
					# funcnod.addCallee(calleenod)
			else:
				print  '\t\t --> ' + str(iline) + '::Y::' + calleenod.getName()[0]
				funcnod.addCallee(calleenod)
				calleenod.incCaller()

	filep.close()

def buildDotFile(func_list):
	header = 'digraph callgraph {\n'
	ender  = '}\n'
	color = '[style = "filled",color="red"]\n'

	output = open('./funtrace.dot', 'w')
	output.write(header)
	lines = 0;
	for node in func_list:
		if node.callers() == 0 and len(node.getCalleeList()) > 0:
			output.write('"' + node.getName()[0] + '"' + color)

		for callee in node.getCalleeList():
			print node.getName()[0] + '->' + callee.getName()[0]
			output.write('"' + node.getName()[0] + '" -> "' + callee.getName()[0] + '"\n')
			lines += 1
	print "have written (%d) lines" % (lines)
	output.write(ender)
	output.close()

def main(argv):
	for filename in sys.argv[1:]:
		getFunList(filename)

	print '\nFunctions found ' + str(len(funcs))

	for filename in sys.argv[1:]:
		parafile(filename);

	buildDotFile(funcs)

if __name__ == '__main__':
	main(sys.argv)




```

参考:[https://blog.csdn.net/aaab01/article/details/61917368](https://blog.csdn.net/aaab01/article/details/61917368)

1. 整个框架

	

![](https://tb.orbbec.com:8080/storage/002dfb6dc0e499e6ab7989aa21e4df51b1a5?Signature=eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJBcHBJRCI6IjU5Mzc3MGZmODM5NjMyMDAyZTAzNThmMSIsIl9hcHBJZCI6IjU5Mzc3MGZmODM5NjMyMDAyZTAzNThmMSIsIl9vcmdhbml6YXRpb25JZCI6IiIsImV4cCI6MTY0MjU4MTY5MSwiaWF0IjoxNjQxOTc2ODkxLCJyZXNvdXJjZSI6Ii9zdG9yYWdlLzAwMmRmYjZkYzBlNDk5ZTZhYjc5ODlhYTIxZTRkZjUxYjFhNSJ9.WAKrR1wbeuT7Wqp1w915bYr3cXBJjBGCSXd1RNmj2Nw&download=funtrace.svg "")

1.  Advanced.cpp

![](https://tb.orbbec.com:8080/storage/002de2ebd7ab92bff56485ed0cacff4c2c5c?Signature=eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJBcHBJRCI6IjU5Mzc3MGZmODM5NjMyMDAyZTAzNThmMSIsIl9hcHBJZCI6IjU5Mzc3MGZmODM5NjMyMDAyZTAzNThmMSIsIl9vcmdhbml6YXRpb25JZCI6IiIsImV4cCI6MTY0MjU4MTY5MSwiaWF0IjoxNjQxOTc2ODkxLCJyZXNvdXJjZSI6Ii9zdG9yYWdlLzAwMmRlMmViZDdhYjkyYmZmNTY0ODVlZDBjYWNmZjRjMmM1YyJ9.u3jxUYNK3_dKnM02-V0YLrY4BNFQSfVffB7eDsnOnH8&download=Advanced.svg "")

1. Simple.cpp

	

![](https://tb.orbbec.com:8080/storage/002d776b471464f9327e8d58a9dbd96c9c59?Signature=eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJBcHBJRCI6IjU5Mzc3MGZmODM5NjMyMDAyZTAzNThmMSIsIl9hcHBJZCI6IjU5Mzc3MGZmODM5NjMyMDAyZTAzNThmMSIsIl9vcmdhbml6YXRpb25JZCI6IiIsImV4cCI6MTY0MjU4MTY5MSwiaWF0IjoxNjQxOTc2ODkxLCJyZXNvdXJjZSI6Ii9zdG9yYWdlLzAwMmQ3NzZiNDcxNDY0ZjkzMjdlOGQ1OGE5ZGJkOTZjOWM1OSJ9.WmKhT7FQ3fdNgesH9u1Gx26IH8KxNNSgcaGeSjMvMq4&download=Simple.svg "")

1. MainLoop.cpp

![](https://tb.orbbec.com:8080/storage/002d680267cfabc5cb9051e6862c97c9083f?Signature=eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJBcHBJRCI6IjU5Mzc3MGZmODM5NjMyMDAyZTAzNThmMSIsIl9hcHBJZCI6IjU5Mzc3MGZmODM5NjMyMDAyZTAzNThmMSIsIl9vcmdhbml6YXRpb25JZCI6IiIsImV4cCI6MTY0MjU4MTY5MSwiaWF0IjoxNjQxOTc2ODkxLCJyZXNvdXJjZSI6Ii9zdG9yYWdlLzAwMmQ2ODAyNjdjZmFiYzVjYjkwNTFlNjg2MmM5N2M5MDgzZiJ9.jbTd6bdjgQhdUUo6SSV51fbybDLWdJ9vw1VTvfNmhQw&download=MainLoop.svg "")

1. ThreadSample.cpp

![](https://tb.orbbec.com:8080/storage/002ddb90c3eba298f216227a255cc457b80b?Signature=eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJBcHBJRCI6IjU5Mzc3MGZmODM5NjMyMDAyZTAzNThmMSIsIl9hcHBJZCI6IjU5Mzc3MGZmODM5NjMyMDAyZTAzNThmMSIsIl9vcmdhbml6YXRpb25JZCI6IiIsImV4cCI6MTY0MjU4MTY5MSwiaWF0IjoxNjQxOTc2ODkxLCJyZXNvdXJjZSI6Ii9zdG9yYWdlLzAwMmRkYjkwYzNlYmEyOThmMjE2MjI3YTI1NWNjNDU3YjgwYiJ9.zRYiwqccC7PQTzfdL8y3Mf2AclRB5p44jSiwvEwKNvw&download=ThreadSample.svg "")



***

### 基础知识

make j* make -j4 make j8区别

转载：

在双处理器上make -j4，难道是让每个处理器跑两个作业？那和make -j2效率相比 难道不是一样的？

1. 两个处理器的话，一般 -j2 能达到最高效率。 不过也有些进程会花时间在IO上，并不能利用完单个cpu的时间。这样 -j4 可以更快。

1. jobs=4 同时最多跑4个作业 make自己会协调，如果CPU等资源不够可能小于4个。不过一般瓶颈都在硬盘，所以一般能看到4个进程一起跑。 

1. 启用4个cpu去编译。提高编译速度。 

1. 感觉指的是逻辑线程数，和CPU无关，逻辑线程具体跑在哪个逻辑CPU上应该是由内核决定的。make说白了也只是个程序而已。

[https://www.cnblogs.com/baiduboy/p/6568289.html](https://www.cnblogs.com/baiduboy/p/6568289.html)

Physics

> Kernel在在管理内存时将物理内存从逻辑上划分为节点(node)，内存管理区(zone)，页框(frame page)三级结构。我们都知道frame page是管理内存单元的最小单位，这个frame page在代码中就是struct page。而node是与cpu数量相关的！pg_data_t node_data[MAX_NUMNODES] 实际上是有多少个内存节点才会有多少个struct pglist_data 结构实例,而一个struct pglist_data下又把当前管理的内存区域划分为3部分：这个就是由zone定义的。zone将内存区域划分为三种类型：1)DMA 2)NORMAL 3)HIGHEM引入这种node管理方式的根本原因是为了兼容UMA架构的计算机，Kernel对于内存的管理主要存在NUMA/UMA两种形式

Graphics

Linux Graphics 主要是指 Linux 是如何在屏幕上显示内容的，可以分为两部分：

- 一部分是显示服务器，比如X11或者Wayland，它们负责定义系统的窗口，允许第三方开发者使用它们提供的client库来开发Linux GUI程序，也就是负责程序的窗口部分；

- 另一部分是这些GUI程序内容的渲染，也就是负责窗口中的内容部分。

do {...} while(0) 在宏定义中的妙用

> 在Linux内核和其它一些著名的C库中有许多使用do{...}while(0)的宏定义。这种宏的用途是什么？有什么好处？Google的Robert Love（先前从事Linux内核开发）给我们解答如下：do{...}while(0)在C中是唯一的构造程序，让你定义的宏总是以相同的方式工作，这样不管怎么使用宏（尤其在没有用大括号包围调用宏的语句），宏后面的分号也是相同的效果。这句话听起来可能有些拗口，其实用一句话概括就是：使用do{...}while(0)构造后的宏定义不会受到大括号、分号等的影响，总是会按你期望的方式调用运行。

unsleep

usleep功能把进程挂起一段时间， 单位是微秒（百万分之一秒）。

system()函数

为了更好的理解system()函数返回值，需要了解其执行过程，实际上system()函数执行了三步操作：

1. fork一个子进程；

1. 在子进程中调用exec函数去执行command；

1. 在父进程中调用wait去等待子进程结束。

对于fork失败，system()函数返回-1。

如果exec执行成功，也即command顺利执行完毕，则返回command通过exit或return返回的值。 （注意，command顺利执行不代表执行成功，比如command："rm debuglog.txt"，不管文件存不存在该command都顺利执行了）

如果exec执行失败，也即command没有顺利执行，比如被信号中断，或者command命令根本不存在，system()函数返回127.

如果command为NULL，则system()函数返回非0值，一般为1.



***

extern 关键字

extern是一个关键字，它告诉编译器存在着一个变量或者一个函数，如果在当前编译语句的前面中没有找到相应的变量或者函数，也会在当前文件的后面或者其它文件中定义

extern “C”

> extern "C"的主要作用就是为了能够正确实现C++代码调用其他C语言代码。加上extern "C"后，会指示编译器这部分代码按C语言的进行编译，而不是C++的。由于C++支持函数重载，因此编译器编译函数的过程中会将函数的参数类型也加到编译后的代码中，而不仅仅是函数名;而C语言并不支持函数重载，因此编译C语言代码的函数时不会带上函数的参数类型，一般只包括函数名。

```text
 extern int a;
 extern double b;
 extern const struct box *box_ptr 
 extern double box_volume(box box_ptr)
```

thread_local

thread_local 是 C++ 11 新引入的一种存储类型，它会影响变量的存储周期。

C++ 中有 4 种存储周期：

```text
 automatic
 static
 dynamic
 thread
```

有且只有 thread_local 关键字修饰的变量具有线程（thread）周期，这些变量在线程开始的时候被生成，在线程结束的时候被销毁，并且每一个线程都拥有一个独立的变量实例。

thread_local 一般用于需要保证线程安全的函数中。

需要注意的一点是，如果类的成员函数内定义了 thread_local 变量，则对于同一个线程内的该类的多个对象都会共享一个变量实例，并且只会在第一次执行这个成员函数时初始化这个变量实例，这一点是跟类的静态成员变量类似的。



***

## shinyProfiler新增功能

使用方法和原来一样

### 已解决:

1. 解决calls统计出现小数的bug，原因:float 是32位，unsigned int也是32位；unsigned int和“float”都使用32位来存储值。但由于浮点数的范围更大,所以它必然会牺牲一些精度。所以有一些无符号的int值不能用浮点数精确表示。解决方法：将folat型改为unsigned int型；

1. 支持min/max/avg/call_num/total call time统计功能；

1. 支持call tree分析

![](https://tb.orbbec.com:8080/storage/012dffbbbdf78db24a50f0841c1bf6d64fc9?Signature=eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJBcHBJRCI6IjU5Mzc3MGZmODM5NjMyMDAyZTAzNThmMSIsIl9hcHBJZCI6IjU5Mzc3MGZmODM5NjMyMDAyZTAzNThmMSIsIl9vcmdhbml6YXRpb25JZCI6IjVlMTU0MmQ5OWNiMDhmMDAwMWZjNDJkNCIsImV4cCI6MTY0MjU4MTg0NSwiaWF0IjoxNjQxOTc3MDQ1LCJyZXNvdXJjZSI6Ii9zdG9yYWdlLzAxMmRmZmJiYmRmNzhkYjI0YTUwZjA4NDFjMWJmNmQ2NGZjOSJ9.Im4qTSShvQ8d_qfxOUr4XBKB8P7I7H4nJbB9yen3Ixg&download=image.png "")

### 存在的问题

1. 多线程输出文件还有一点bug，如下图所示，新增的min/max/avg功能还不能同步到离线文件中

![](https://tb.orbbec.com:8080/storage/012d22fbd63573a147cf58ccc380a34c4423?Signature=eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJBcHBJRCI6IjU5Mzc3MGZmODM5NjMyMDAyZTAzNThmMSIsIl9hcHBJZCI6IjU5Mzc3MGZmODM5NjMyMDAyZTAzNThmMSIsIl9vcmdhbml6YXRpb25JZCI6IjVlMTU0MmQ5OWNiMDhmMDAwMWZjNDJkNCIsImV4cCI6MTY0MjU4MTkzMywiaWF0IjoxNjQxOTc3MTMzLCJyZXNvdXJjZSI6Ii9zdG9yYWdlLzAxMmQyMmZiZDYzNTczYTE0N2NmNThjY2MzODBhMzRjNDQyMyJ9.86fBe5n7RdW2klFKG1FoWWZXBnbAB2lfiXUCIW4I6aM&download=image.png "")

1. 不能解决tic-toc的自检功能，原因：

    1. tic-toc统计时需要添加两个宏函数，较为麻烦，而shinyProfiler只需一个func函数即可

    1. shinyProfiler原项目代码风格混乱，增加tic-toc自检功能非常困难；单独把call-tree拎出来设计太多源文件，在较短的时间内完成不可取。原因：虽然源文件中的类没几个，但里面的数据结构涉及哈希表(再哈希法)，遍历森林，拎出来非常棘手

    1. 相同的typedef太多，比如一个unsigned long int 类型一下子定义了5次，猜想是因为几个人分工写项目最后合并时用typedef进行拼合

    1. 原文档没注释，也没开发文档，数据流图，晦涩难懂

