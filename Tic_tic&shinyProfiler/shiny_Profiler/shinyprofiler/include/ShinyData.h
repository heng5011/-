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

#ifndef SHINY_DATA_H
#define SHINY_DATA_H

// #include <iostream>
#include <stdio.h>
// using namespace std;

#include "ShinyPrereqs.h"


#ifdef __cplusplus
extern "C" {
#endif


/*---------------------------------------------------------------------------*/

typedef struct {
	uint32_t entryCount;
	shinytick_t selfTicks;
} ShinyLastData;


/*---------------------------------------------------------------------------*/

typedef struct {
	shinytick_t cur;	//shinytick_t 是unsigned long int
	float avg;
} ShinyTickData;

typedef struct {
	uint32_t cur;
	float avg;
	uint32_t calls_num;	//baoer
	float max_time;	//
	float min_time;	//
	float avg_time;	//

} ShinyCountData;

typedef struct {
	ShinyCountData entryCount;
	ShinyTickData selfTicks;
	ShinyTickData childTicks;
} ShinyData;


SHINY_INLINE shinytick_t ShinyData_totalTicksCur(const ShinyData *self) {
	return self->selfTicks.cur + self->childTicks.cur;
}

SHINY_INLINE float ShinyData_totalTicksAvg(const ShinyData *self) {
	return self->selfTicks.avg + self->childTicks.avg;
}

//baoer
SHINY_INLINE float ShinyData_MaxTicksAvg(const ShinyData *self) {
	return self->selfTicks.avg > self->childTicks.avg ? self->selfTicks.avg : self->childTicks.avg;
}
SHINY_INLINE float ShinyData_MinTicksAvg(const ShinyData *self) {
	return self->selfTicks.avg < self->childTicks.avg ? self->selfTicks.avg : self->childTicks.avg; 
}
SHINY_INLINE float ShinyData_AvgTicksAvg(const ShinyData *self) {
	return 0.5 * (self->selfTicks.avg + self->childTicks.avg);
}


SHINY_INLINE void ShinyData_computeAverage(ShinyData *self, float a_damping) {
	// self->entryCount.avg = self->entryCount.cur +
	// 	a_damping * (self->entryCount.avg - self->entryCount.cur);
	self->entryCount.calls_num = self->entryCount.cur +
	a_damping * (self->entryCount.calls_num - self->entryCount.cur);	//baoer  A + (num - A)为了减少误差

	self->selfTicks.avg = self->selfTicks.cur +
		a_damping * (self->selfTicks.avg - self->selfTicks.cur);
	self->childTicks.avg = self->childTicks.cur +
		a_damping * (self->childTicks.avg - self->childTicks.cur);

}

SHINY_INLINE void ShinyData_copyAverage(ShinyData *self) {
	// self->entryCount.avg = (float) self->entryCount.cur;
	self->entryCount.calls_num = self->entryCount.cur;	//baoer
	self->entryCount.max_time = (float)self->entryCount.max_time;	//baoer
	self->entryCount.min_time = (float)self->entryCount.min_time;	//baoer
	

	self->selfTicks.avg = (float) self->selfTicks.cur;
	self->childTicks.avg = (float) self->childTicks.cur;
}

SHINY_INLINE void ShinyData_clearAll(ShinyData *self) {
	self->entryCount.cur = 0;
	// self->entryCount.avg = 0;
	self->entryCount.calls_num = 0; //baoer
	self->entryCount.max_time = 0; //baoer
	self->entryCount.min_time = 0; //baoer

	self->selfTicks.cur = 0;
	self->selfTicks.avg = 0;
	self->childTicks.cur = 0;
	self->childTicks.avg = 0;
}

SHINY_INLINE void ShinyData_clearCurrent(ShinyData *self) {
	self->entryCount.cur = 0;
	self->selfTicks.cur = 0;
	self->childTicks.cur = 0;
}


#ifdef __cplusplus
} /* end of extern "C" */
#endif

#endif /* end of include guard */
