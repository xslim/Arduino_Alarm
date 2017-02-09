//
//
//  Created by Taras Kalapun on 2/8/17.
//
//

#ifndef Scheduler_h
#define Scheduler_h

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

#define SCHEDULER_MAX_TASKS 10

#define SCHEDULER_TIMER_ENABLED		0x01
#define SCHEDULER_TIMER_AUTORESET	0x02

typedef struct SchedulerTimer SchedulerTimer;
typedef void(*scheduler_thread_callback_t)(SchedulerTimer *timer);

struct SchedulerTimer{
    unsigned long time;
    unsigned long timeout;
    uint8_t flags;
    uint16_t initialTimeout;
    scheduler_thread_callback_t callback;
};


void scheduler_setup(unsigned long time);
void scheduler_update(unsigned long time);
void scheduler_reset_timeout(SchedulerTimer * timer, unsigned long timeout);
SchedulerTimer *scheduler_add(scheduler_thread_callback_t callback_fnc, unsigned long timeout);

#ifdef __cplusplus
}
#endif

#endif /* Scheduler_h */
