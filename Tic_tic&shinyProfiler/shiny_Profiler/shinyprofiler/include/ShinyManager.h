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

#ifndef SHINY_MANAGER_H
#define SHINY_MANAGER_H

#include "ShinyZone.h"
#include "ShinyNode.h"
#include "ShinyNodePool.h"
#include "ShinyTools.h"
#include "ShinyOutput.h"

#include <stdio.h>
#include <cstdlib>
#if defined(__ANDROID__) && defined(SHINY_VERBOSE_ANDROID_LOGGING)
#include <android/log.h>
#define ALOG(type, channel, format, ...) __android_log_print(type, channel, format, ##__VA_ARGS__)
#define ALOG_INFO(channel, format, ...) __android_log_print(ANDROID_LOG_INFO, channel, format, ##__VA_ARGS__)
#else
#define ALOG(type, channel, format, ...) //printf( format, ##__VA_ARGS__)
#define ALOG_INFO(channel, format, ...)	//printf( format, ##__VA_ARGS__)
#endif

#if SHINY_IS_COMPILED == TRUE

#ifdef __cplusplus
extern "C" {
#endif

#define SHINY_THREAD_NAME_LEN 128
/*---------------------------------------------------------------------------*/

typedef struct {
#if SHINY_HAS_ENABLED == TRUE
	bool enabled;
#endif

	shinytick_t _lastTick;	//shinytick_t 是unsigned long int

	ShinyNode* _curNode;

	uint32_t _tableMask; /* = _tableSize - 1 */

	ShinyNodeTable* _nodeTable;

#if SHINY_LOOKUP_RATE == TRUE
	uint64_t _lookupCount;
	uint64_t _lookupSuccessCount;
#endif

	uint32_t _tableSize;

	uint32_t nodeCount;
	uint32_t zoneCount;

	ShinyZone* _lastZone;

	ShinyNodePool* _lastNodePool;
	ShinyNodePool* _firstNodePool;

	ShinyNode rootNode;
	ShinyZone rootZone;

	float damping;

	int _initialized;
	int _firstUpdate;

    char threadName[SHINY_THREAD_NAME_LEN];
} ShinyManager;


/*---------------------------------------------------------------------------
extern是一个关键字，它告诉编译器存在着一个变量或者一个函数，
如果在当前编译语句的前面中没有找到相应的变量或者函数，也会在当前文件的后面或者其它文件中定义
*/

extern SHINY_TLS_C ShinyNode* _ShinyManager_dummyNodeTable[];

extern SHINY_TLS_C ShinyManager Shiny_instance;


/*---------------------------------------------------------------------------*/


SHINY_INLINE void _ShinyManager_appendTicksToCurNode(ShinyManager *self) {
	shinytick_t curTick;	//shinytick_t 是unsigned long int, 记录当前时间
	ShinyGetTicks(&curTick);

    ALOG_INFO("SHINY", "appendTicks instance: %p self: %p initialized: %d rootnode: %p curnode: %p nodecount: %d node pool: %p - %p",
        &Shiny_instance, self, self->_initialized, &self->rootNode, self->_curNode, self->nodeCount,  self->_firstNodePool, self->_lastNodePool);
    ALOG_INFO("SHINY", "appendTicks curnode name: %s curnode last: %p",
        self->_curNode->zone->name, &self->_curNode->_last);

	// 在 _ShinyManager_init 时生成一个时间戳，调用 _ShinyManager_appendTicksToCurNode 函数时又调用一次时间戳
	ShinyNode_appendTicks(self->_curNode, curTick - self->_lastTick); //当前时间-最后结束的时间，就是当前节点(节点可理解成代码块)跑过的时间
	self->_lastTick = curTick;
}

SHINY_API ShinyNode* _ShinyManager_lookupNode(ShinyManager *self, ShinyNodeCache* a_cache, ShinyZone* a_zone);

SHINY_API void _ShinyManager_createNodeTable(ShinyManager *self, uint32_t a_count);
SHINY_API void _ShinyManager_resizeNodeTable(ShinyManager *self, uint32_t a_count);

SHINY_API void _ShinyManager_createNodePool(ShinyManager *self, uint32_t a_count);
SHINY_API void _ShinyManager_resizeNodePool(ShinyManager *self, uint32_t a_count);

SHINY_API ShinyNode* _ShinyManager_createNode(ShinyManager *self, ShinyNodeCache* a_cache, ShinyZone* a_pZone);
SHINY_API void _ShinyManager_insertNode(ShinyManager *self, ShinyNode* a_pNode);

SHINY_INLINE void _ShinyManager_init(ShinyManager *self) {
	self->_initialized = TRUE;

    self->_curNode = &self->rootNode;
    self->_lastZone = &self->rootZone;

    self->rootNode.zone = &self->rootZone;
    self->rootNode.parent = &self->rootNode;

	self->rootNode._last.entryCount = 1;
	self->rootNode._last.selfTicks = 0;
	ShinyGetTicks(&self->_lastTick);
}

SHINY_INLINE void _ShinyManager_uninit(ShinyManager *self) {
	self->_initialized = FALSE;

	ShinyNode_clear(&self->rootNode);
	self->rootNode.zone = &self->rootZone;
    self->rootNode.parent = &self->rootNode;
}

#if SHINY_LOOKUP_RATE == TRUE
SHINY_INLINE void _ShinyManager_incLookup(ShinyManager *self) { self->_lookupCount++; }
SHINY_INLINE void _ShinyManager_incLookupSuccess(ShinyManager *self) { self->_lookupSuccessCount++; }
SHINY_INLINE float ShinyManager_lookupRate(const ShinyManager *self) { return ((float) self->_lookupSuccessCount) / ((float) self->_lookupCount); }

#else
SHINY_INLINE void _ShinyManager_incLookup(ShinyManager *self) {}
SHINY_INLINE void _ShinyManager_incLookupSuccess(ShinyManager *self) {}
SHINY_INLINE float ShinyManager_lookupRate(const ShinyManager *self) { return -1; }
#endif

SHINY_API void ShinyManager_setThreadName(ShinyManager *self, const char* name);

SHINY_API void ShinyManager_resetZones(ShinyManager *self);
SHINY_API void ShinyManager_destroyNodes(ShinyManager *self);

SHINY_INLINE float ShinyManager_tableUsage(const ShinyManager *self)  {
	return ((float) self->nodeCount) / ((float) self->_tableSize);
}

SHINY_INLINE uint32_t ShinyManager_allocMemInBytes(const ShinyManager *self) {
	return self->_tableSize * sizeof(ShinyNode*)
            + ((self->_firstNodePool) ? ShinyNodePool_memoryUsageChain(self->_firstNodePool) : 0);
}

SHINY_INLINE void ShinyManager_beginNode(ShinyManager *self, ShinyNode* a_node) {
    ALOG_INFO("SHINY", "beginNode start instance: %p self: %p initialized: %d curNode: %p a_node: %p name: %s",
        &Shiny_instance, self, self->_initialized, self->_curNode, a_node, a_node->zone->name);

	ShinyNode_beginEntry(a_node);

	_ShinyManager_appendTicksToCurNode(self);
	self->_curNode = a_node;
    ALOG_INFO("SHINY", "beginNode end instance: %p self: %p initialized: %d curNode: %p a_node: %p name: %s",
        &Shiny_instance, self, self->_initialized, self->_curNode, a_node, a_node->zone->name);

}

// ShinyManager_lookupAndBeginNode(&Shiny_instance, &cache, &id);	
SHINY_INLINE void ShinyManager_lookupAndBeginNode(ShinyManager *self, ShinyNodeCache* a_cache, ShinyZone* a_zone) {
#if SHINY_HAS_ENABLED == TRUE
	if (!self->enabled) return;
#endif
    ALOG_INFO("SHINY", "lookupAndBeginNode start instance: %p self: %p initialized: %d curNode: %p a_cache: %p a_zone: %p parent: %p zoneName: %s nodecount: %d node pool: %p - %p",
        &Shiny_instance, self, self->_initialized, self->_curNode, a_cache, a_zone, (*a_cache)->parent, a_zone->name, self->nodeCount,  self->_firstNodePool, self->_lastNodePool);

    // if not initialized or nodes are not in a parent/child relationship
	if (self->_initialized == FALSE || self->_curNode != (*a_cache)->parent)
    {
        // this will initialize ShinyManager if necessary
		*a_cache = _ShinyManager_lookupNode(self, a_cache, a_zone);
    }

    ALOG_INFO("SHINY", "lookupAndBeginNode mid instance: %p self: %p initialized: %d curNode: %p a_cache: %p a_zone: %p parent: %p zoneName: %s nodecount: %d node pool: %p - %p",
        &Shiny_instance, self, self->_initialized, self->_curNode, a_cache, a_zone, (*a_cache)->parent, a_zone->name, self->nodeCount,  self->_firstNodePool, self->_lastNodePool);

	ShinyManager_beginNode(self, *a_cache);

    ALOG_INFO("SHINY", "lookupAndBeginNode end instance: %p self: %p initialized: %d curNode: %p a_cache: %p a_zone: %p parent: %p zoneName: %s nodecount: %d node pool: %p - %p",
        &Shiny_instance, self, self->_initialized, self->_curNode, a_cache, a_zone, (*a_cache)->parent, a_zone->name, self->nodeCount,  self->_firstNodePool, self->_lastNodePool);

}

SHINY_INLINE void ShinyManager_endCurNode(ShinyManager *self) {
#if SHINY_HAS_ENABLED == TRUE
    if (!Shiny_instance.enabled)
        return;
#endif
    ALOG_INFO("SHINY", "endCurNode start instance: %p self: %p initialized: %d curNode: %p",
        &Shiny_instance, self, self->_initialized, self->_curNode);

	_ShinyManager_appendTicksToCurNode(self);
	self->_curNode = self->_curNode->parent;

    ALOG_INFO("SHINY", "endCurNode end instance: %p self: %p initialized: %d curNode: %p name: %s",
        &Shiny_instance, self, self->_initialized, self->_curNode, self->_curNode->zone->name);
}

/**/

SHINY_API void ShinyManager_preLoad(ShinyManager *self);

SHINY_API void ShinyManager_updateClean(ShinyManager *self);
SHINY_API void ShinyManager_update(ShinyManager *self);

SHINY_API void ShinyManager_clear(ShinyManager *self);
SHINY_API void ShinyManager_destroy(ShinyManager *self);

SHINY_INLINE void ShinyManager_sortZones(ShinyManager *self) {
	if (self->rootZone.next)
		self->_lastZone = ShinyZone_sortChain(&self->rootZone.next);
}

SHINY_API const char* ShinyManager_getOutputErrorString(ShinyManager *self);

// enum <类型名> {<枚举常量表>};
enum ShinyOutputMode {
    SHINY_OUTPUT_MODE_DEFAULT,
    SHINY_OUTPUT_MODE_APPEND
};

SHINY_API int ShinyManager_output(ShinyManager *self,
                                  const char *a_filename,
                                  ShinyOutputMode mode);

SHINY_API void ShinyManager_clearOutput(const char *a_filename);

SHINY_API void ShinyManager_outputToStream(ShinyManager *self, FILE *stream);


#ifdef __cplusplus
} /* end of extern "C" */


SHINY_INLINE std::string ShinyManager_outputTreeToString(ShinyManager *self) {
	const char* error = ShinyManager_getOutputErrorString(self);
	if (error) return error;
	else return ShinyNodesToString(&self->rootNode, self->nodeCount, self->threadName);
}

SHINY_INLINE std::string ShinyManager_outputFlatToString(ShinyManager *self) {
	const char* error = ShinyManager_getOutputErrorString(self);
	if (error) return error;

	ShinyManager_sortZones(self);
	return ShinyZonesToString(&self->rootZone, self->zoneCount, self->threadName);
}


extern "C" { /* end of c++ */
#endif


SHINY_INLINE int ShinyManager_isZoneSelfTimeBelow(ShinyManager *self, ShinyZone* a_zone, float a_percentage) {
	return a_percentage * (float) self->rootZone.data.childTicks.cur
		<= (float) a_zone->data.selfTicks.cur;
}

SHINY_INLINE int ShinyManager_isZoneTotalTimeBelow(ShinyManager *self, ShinyZone* a_zone, float a_percentage) {
	return a_percentage * (float) self->rootZone.data.childTicks.cur
		<= (float) ShinyData_totalTicksCur(&a_zone->data);
}

/**/

SHINY_INLINE void ShinyManager_enumerateNodes(ShinyManager *self, void (*a_func)(const ShinyNode*)) {
	ShinyNode_enumerateNodes(&self->rootNode, a_func);
}

SHINY_INLINE void ShinyManager_enumerateZones(ShinyManager *self, void (*a_func)(const ShinyZone*)) {
	ShinyZone_enumerateZones(&self->rootZone, a_func);
}


#ifdef __cplusplus
} /* end of extern "C" */


template <class T> void ShinyManager_enumerateNodes(ShinyManager *self, T* a_this, void (T::*a_func)(const ShinyNode*)) {
	ShinyNode_enumerateNodes(&self->rootNode, a_this, a_func);
}

template <class T> void ShinyManager_enumerateZones(ShinyManager *self, T* a_this, void (T::*a_func)(const ShinyZone*)) {
	ShinyZone_enumerateZones(&self->rootZone, a_this, a_func);
}


extern "C" { /* end of c++ */
#endif


/*---------------------------------------------------------------------------*/


#ifdef __cplusplus
} /* end of extern "C" */


class ShinyEndNodeOnDestruction {
public:
    ShinyEndNodeOnDestruction(ShinyManager* self) : self_(self)
    {
        ALOG_INFO("SHINY", "ShinyEndNodeOnDestruction ctor instance: %p self: %p this: %p",
            &Shiny_instance, self_, this);
    }

	~ShinyEndNodeOnDestruction() {
        ALOG_INFO("SHINY", "ShinyEndNodeOnDestruction dtor instance: %p self: %p this: %p",
            &Shiny_instance, self_, this);
		ShinyManager_endCurNode(self_);
        self_ = nullptr;
	}
private:
    ShinyManager* self_;
};


#endif /* end of c++ */

#endif /* if SHINY_IS_COMPILED == TRUE */

#endif /* end of include guard */
