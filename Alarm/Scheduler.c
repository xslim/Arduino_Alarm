//
//
//  Created by Taras Kalapun on 2/8/17.
//
//

#include "Scheduler.h"
#include <string.h>

typedef struct {
    unsigned long time;
    SchedulerTimer timers[SCHEDULER_MAX_TASKS];
} Scheduler;

Scheduler scheduler;

void scheduler_setup(unsigned long time) {
    scheduler.time = time;
}


SchedulerTimer * scheduler_get_free_timer() {
    for (int i = 0; i < SCHEDULER_MAX_TASKS; i++){
        if (!scheduler.timers[i].callback ){
            memset(&scheduler.timers[i], 0, sizeof(SchedulerTimer));
            return &scheduler.timers[i];
        }
    }
    return NULL;
}
SchedulerTimer *scheduler_add(scheduler_thread_callback_t callback_fnc, uint16_t timeout) {
    SchedulerTimer *timer = scheduler_get_free_timer();
    if (!timer) {
        return NULL;
    }
    timer->callback = callback_fnc;
    timer->timeout = timeout;
    timer->initialTimeout = timeout;
    timer->flags = SCHEDULER_TIMER_ENABLED | SCHEDULER_TIMER_AUTORESET;
    timer->time = scheduler.time;
    
    // Unset
    //timer->flags &= ~FLAG_TIMER_ENABLED;
    return timer;
}

void scheduler_reset_timeout(SchedulerTimer * timer, uint16_t timeout) {
    timer->timeout = (timeout > 0) ? timeout : timer->initialTimeout;
}

//void scheduler_update_time(unsigned long time) {
//    scheduler.time = time;
//}

void scheduler_update(unsigned long time) {
    scheduler.time = time;
    for (int i = 0; i < SCHEDULER_MAX_TASKS; i++){
        SchedulerTimer *timer = &scheduler.timers[i];
        
        if ((timer->flags & SCHEDULER_TIMER_ENABLED) && (scheduler.time - timer->time) >= timer->timeout) {
            if (timer->callback ){
                timer->callback(timer);
            }
            
            if (timer->flags & SCHEDULER_TIMER_AUTORESET) {
                timer->time = scheduler.time;
            } else {
                timer->flags = 0;
            }
        }
        
    }
}


