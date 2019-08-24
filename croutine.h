/********************************************************************************
*  croutine                                                 		            *
*  Copyright (C) 2014 lonelygray  macrosoft@outlook.com.                        *
*                                                                               *
*  This file is part of croutine.                                               *
*                                                                               *
*  This program is free software; you can redistribute it and/or modify         *
*  it under the terms of the GNU General Public License version 3 as            *
*  published by the Free Software Foundation.                                	*
*                                                                               *
*  You should have received a copy of the GNU General Public License            *
*  along with OST. If not, see <http://www.gnu.org/licenses/>.                  *
*                                                                               *
*  Unless required by applicable law or agreed to in writing, software          *
*  distributed under the License is distributed on an "AS IS" BASIS,            *
*  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.     *
*  See the License for the specific language governing permissions and          *
*  limitations under the License.                                               *
*                                                                               *
*  @file     croutine.h                                                         *
*  @brief    a fack co-routine                                                  *
*  Details.                                                            	        *
*                                                                               *
*  @author   lonegray                                                           *
*  @email    macrosoft@live.cn                                                  *
*  @version  1.0.0.1                                                            *
*  @license  GNU General Public License Version 3 (GPLv3)                       *
*                                                                               *
*  Change History :                                                             *
*  <Date>     | <Version> | <Author>       | <Description>                      *
*--------------------------------------------------------------------------     *
*  2018/04/24 | 1.0.0.1   | lonegray      | Create file                         *
*--------------------------------------------------------------------------     *
*                                                                               *
********************************************************************************/
#pragma once
#ifndef __CROUNTINE_H__
#define __CROUNTINE_H__
//#include "stdio.h"

#define get_tick_count             ulTimeGetTicks
#define TIME_ELAPSED(now, start)   (((now) >= (start)) ? ((now) - (start)) : (ULONG_MAX - (start) + now))

/***************************************
* typedef
***************************************/
typedef unsigned char       u8;
typedef unsigned int        u16;
typedef unsigned long       u32;
typedef unsigned char       BOOL;
//typedef unsigned char       bool;

//#define false               0
//#define true                1
#define FALSE               0
#define TRUE                1

#define Nop                 {}

/***************************************
* coroutine
***************************************/
#define __CO_STATE                      __LINE__ * 2
#define __CO_STATE_NEXT                 __LINE__ * 2 + 1

#define __co_var(type, var) \
    static type pdata var

#define __co_let(type, var, value) \
    static type pdata var = (value)

//typedef struct __co_context {
//    u16 state;
//    u32 time_since;
//    u32 time_for;
//} __co_context_t;
//
typedef struct __co_asleep_context {
    u32 time_since;
    u32 time_for;
} __co_asleep_context_t;

typedef bool __async_void;

#define __yield \
        do { \
            __co_state = __CO_STATE; \
            goto __CO_END__; \
    case __CO_STATE: Nop; \
        } while (0)
        
//#define __asleep(ticks) 
#define __asleep(ticks) \
        do { \
            if ((ticks) == 0) { \
                __co_state = __CO_STATE_NEXT; \
                goto __CO_END__; \
            }else { \
                __co_asleep_context.time_since = get_tick_count(); \
                __co_asleep_context.time_for = ticks; \
                __co_state = __CO_STATE; \
                goto __CO_END__; \
    case __CO_STATE: \
                if (TIME_ELAPSED(get_tick_count(), __co_asleep_context.time_since) > __co_asleep_context.time_for) \
                    break; \
                goto __CO_END__; \
            } \
    case __CO_STATE_NEXT: Nop; \
        } while (0)
       
/*
    __await(cond);
    equal==> 
    while (!cond) {
        __yield;  //每次循环都会重复设置__co_state
    }
*/
/*        
#define __await(async_func) \
        while (!(async_func)) { \
            __co_state = __CO_STATE; \ //每次循环都会重复设置__co_state
            goto __CO_END__; \
    case __CO_STATE: Nop;\
        }
*/        
#define __await(async_func) \
        do { \
            __co_state = __CO_STATE; \
    case __CO_STATE: \
            if ((async_func)) \
                break; \
            goto __CO_END__; \
        } while (0)

#define __timed_await(async_func, wait_time_ticks) \
        do { \
            if ((wait_time_ticks) > 0) { \
                __co_asleep_context.time_since = get_tick_count(); \
                __co_asleep_context.time_for = wait_time_ticks; \
                __co_state = __CO_STATE; \
    case __CO_STATE: \
                if ((async_func) || TIME_ELAPSED(get_tick_count(), __co_asleep_context.time_since) > __co_asleep_context.time_for) \
                    break; \
                goto __CO_END__; \
            }else { \
                if ((async_func)) \
                    break; \
                __co_state = __CO_STATE_NEXT; \
                goto __CO_END__; \
            } \
    case __CO_STATE_NEXT: Nop; \
        } while (0)
    
#define __co_begin() \
    static short pdata __co_state = 0; \
    static __co_asleep_context_t pdata __co_asleep_context; \
    __co_asleep_context = __co_asleep_context; \
    switch (__co_state) { \
    case 0:

#define __co_end() \
        __co_state = 0; \
        return true; \
    default: \
        break; \
    } \
__CO_END__: \
    return false

#if 0
/// #if 0 [
#define __task__    

typedef const void AwaitContext_t;

typedef struct DelayUntilContext {
    u32 ulTimeSince;
    u32 ulTimeFor;
} DelayUntilContext_t;

typedef struct WaitForContext {
    void * pvCookie;
    bool bWaitResult;
} WaitForContext_t;

typedef bool(code *AwaitCondition_t)(const AwaitContext_t *) large reentrant;

bool crDelayUntil(const DelayUntilContext_t * ctx) large reentrant;

#define crVar(type, var) \
    static type pdata var

#define crLet(type, var, value) \
    static type pdata var = (value)

/*
        if (__await_cond__(__await_ctx__)) { \
            __state__ = __resume_state__; \
            if (__await_cond__ != crDelayUntil) \
                ((WaitForContext_t*)__await_ctx__)->bWaitResult = true; \
        } \
        else if (__await_cond__ != crDelayUntil && crDelayUntil(__await_ctx__)) { \
            ((WaitForContext_t*)__await_ctx__)->bWaitResult = false; \
            __state__ = __resume_state__; \
        } \
*/
#define crStart() \
    static DelayUntilContext_t pdata __delay_until_ctx__; \
    static AwaitContext_t * pdata __await_ctx__ = NULL; \
    static AwaitCondition_t pdata __await_cond__ = NULL; \
    static short pdata __state__ = 1; \
    static short pdata __resume_state__ = 1; \
    switch (__state__) { \
    case 0: \
        if (__await_cond__ != crDelayUntil && __await_cond__(__await_ctx__)) { \
            __state__ = __resume_state__; \
            ((WaitForContext_t *)__await_ctx__)->bWaitResult = true; \
        } \
        else if (crDelayUntil(&__delay_until_ctx__)) { \
            __state__ = __resume_state__; \
            if (__await_cond__ != crDelayUntil) \
                ((WaitForContext_t *)__await_ctx__)->bWaitResult = false; \
        } \
        break; \
    case 1:

#define crEnd() \
        __state__ = -1; \
        break; \
    default: \
        break; \
    } \
    __LABEL_CREND___:

#define crDelay(ticks) \
        do { \
            if ((ticks) == 0) { \
                __state__ = __LINE__; \
            } \
            else { \
                __delay_until_ctx__.ulTimeSince = ulTimeGetTickCount(); \
                __delay_until_ctx__.ulTimeFor = (ticks); \
                __await_ctx__ = &__delay_until_ctx__; \
                __await_cond__ = (AwaitCondition_t)crDelayUntil; \
                __resume_state__ = __LINE__; \
                __state__ = 0; \
            } \
            goto __LABEL_CREND___; \
    case __LINE__: Nop; \
        } while (0)

#define crWaitFor(cond, ctx, timeout, cookie) \
        do { \
            __delay_until_ctx__.ulTimeSince = ulTimeGetTickCount(); \
            __delay_until_ctx__.ulTimeFor = (timeout); \
            (ctx).pvCookie = (cookie); \
            (ctx).bWaitResult = false; \
            __await_ctx__ = &(ctx); \
            __await_cond__ = (AwaitCondition_t)(cond); \
            __resume_state__ = __LINE__; \
            __state__ = 0; \
    case __LINE__: Nop; \
        } while (0) 
/// #endif ]
#endif

#endif
