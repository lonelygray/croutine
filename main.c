#include "stdlib.h"
#include "stdio.h"
#include "croutine.h"

/*scheduler.h*/

typedef __async_void (*task_routine_t)(u32 clockticks);

typedef struct task {
    task_routine_t xRoutine;
    u32 uPriority;
    u32 uTimeCost;
} task_t;

u32 GetTaskId(void);
void StartSchedule(task_t* xTaskTable, const u32 uTaskCount);

/*scheduler.c*/

#define SCHEDULE_PERIOD     1    //毫秒

static u32 _currTaskId;

u32 GetTaskId(void)
{
    return _currTaskId;
}

void StartSchedule(task_t* xTaskTable, const u32 uTaskCount)
{
    u32 i;
    u32 ts;
    u32 lastCost;
    u32 busyLevel = 0;
    u32 _uScheduleCounter = 0;

    for (i = 0; i < uTaskCount; i++) {
        if (xTaskTable[i].uPriority == 0) {
            printf("task priority can not be zero:%u", i);
            return;
        }
        xTaskTable[i].uTimeCost = 0;
    }

    for (;;) {
        if (++_uScheduleCounter) {
            for (i = 0; i < uTaskCount; i++) {
                if (_uScheduleCounter % xTaskTable[i].uPriority == 0) {
                    _currTaskId = i;
                    ts = GetTickCount();
                    xTaskTable[i].xRoutine(ts);
                    lastCost = xTaskTable[i].uTimeCost;
                    xTaskTable[i].uTimeCost = GetTickCount() - ts;
                    if (xTaskTable[i].uTimeCost < SCHEDULE_PERIOD) {
                        if (lastCost >= SCHEDULE_PERIOD) {
                            busyLevel--;
                        }
                        if (busyLevel == 0) {
                            usleep((SCHEDULE_PERIOD - xTaskTable[i].uTimeCost) * 1000);
                        }
                    }
                    else if (lastCost < SCHEDULE_PERIOD) {
                        busyLevel++;
                    }
                }
            }
        }
    }
}


/*main.c*/

__async_void task1(u32 timeticks)
{
    __co_begin();
    printf("[%8u]task1 begin\n", timeticks);
    for (;;) {
        __asleep(1000);
        printf("[%8u]task1\n", timeticks);
    }
    __co_end();
}

__async_void do_async(void)
{
    __co_begin();
    printf("do_async:1\n");
    __asleep(1000);
    printf("do_async:2\n");
    __asleep(1000);
    printf("do_async:3\n");
    __asleep(1000);
    __co_end();
}

__async_void task2(u32 timeticks)
{
    __co_begin();
    printf("[%8u]task2 begin\n", timeticks);
    for (;;) {
        __await(do_async());
        printf("[%8u]task2\n", timeticks);
    }
    __co_end();
}

static task_t _xTaskTable[] = {
    /*
    { .xRoutine, .uPriority = [1..] }
    */
    { task1,                1 },
    { task2,                1 },
};

int main(int argc, char** argv)
{
    StartSchedule(_xTaskTable, sizeof(_xTaskTable) / sizeof(task_t));
}