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

#define get_tick_count             GetTickCount
#define TIME_ELAPSED(now, start)   (((now) >= (start)) ? ((now) - (start)) : (ULONG_MAX - (start) + now))

/***************************************
* typedef
***************************************/
typedef unsigned char       u8;
typedef unsigned int        u16;
typedef unsigned long       u32;
typedef unsigned char       BOOL;
typedef unsigned char       bool;

#define false               0
#define true                1
#define FALSE               0
#define TRUE                1

#define Nop                 {}

/***************************************
* coroutine
***************************************/
#define __CO_STATE                      __LINE__ * 2
#define __CO_STATE_NEXT                 __LINE__ * 2 + 1

#define __co_var(type, var) \
    static type var

#define __co_let(type, var, value) \
    static type var = (value)

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
    static short __co_state = 0; \
    static __co_asleep_context_t __co_asleep_context; \
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

#endif
