//
//  UnitTests.m
//  UnitTests
//
//  Created by Taras Kalapun on 2/7/17.
//
//

#import <XCTest/XCTest.h>

#import "Scheduler.h"
#import "DST.h"

#include <mach/mach.h>
#include <mach/mach_time.h>

unsigned long millis() {
    static mach_timebase_info_data_t sTimebaseInfo;
    uint64_t machTime = mach_absolute_time();
    
    // Convert to nanoseconds - if this is the first time we've run, get the timebase.
    if (sTimebaseInfo.denom == 0 )
    {
        (void) mach_timebase_info(&sTimebaseInfo);
    }
    
    // Convert the mach time to milliseconds
    return ((machTime / 1000000) * sTimebaseInfo.numer) / sTimebaseInfo.denom;
}


@interface UnitTests : XCTestCase

@end

@implementation UnitTests

- (void)setUp {
    [super setUp];
    // Put setup code here. This method is called before the invocation of each test method in the class.
}

- (void)tearDown {
    // Put teardown code here. This method is called after the invocation of each test method in the class.
    [super tearDown];
}

- (void)testExample {
    // This is an example of a functional test case.
    // Use XCTAssert and related functions to verify your tests produce the correct results.
}

- (void)testPerformanceExample {
    // This is an example of a performance test case.
    [self measureBlock:^{
        // Put the code you want to measure the time of here.
    }];
}


- (void)testPerformanceInt {
    
    
    uint16_t v = 43210;
    uint16_t div = 1000;
    
    
    
    [self measureBlock:^{
        char buffer [50];
        sprintf (buffer, "%i.%i", v / div, v % div);
    }];
}



static uint32_t global_timer=0;



typedef struct _timeout_strct{
    uint32_t time;
    uint16_t timeout;
    uint8_t flags;
    void (*callback_fnc)(struct _timeout_strct* timer, void * echo_struct);
    void * echo_struct;
}timeout_strct;


typedef struct{
    uint16_t sensorVal;
    int read_busy;
}sensor_read;

#define SENSOR_TIMEOUT_INTERVAL (5*60*1000)


//void timer_callback(timeout_strct * timer, void * echo_struct){
//    sensor_read * sensor = (sensor_read*)echo_struct;
//    if(sensor->read_busy){
//        sensor->read_busy = 0;
//        sensor->sensorVal = 666;
//        timer->timeout = SENSOR_TIMEOUT_INTERVAL;
//    }else{
//        //start my read
//        
//        timer->timeout = 1*1000;
//    }
//}
//
//
//
//void set_timer(timeout_strct * timer, uint16_t timeout, uint8_t flags,
//               void (*callback_fnc)(struct _timeout_strct* timer, void *
//                                    echo_struct),void * echo_struct){
//                   if(timer){
//                       timer->time = global_timer;
//                       timer->timeout = timeout;
//                       timer->callback_fnc = callback_fnc;
//                       timer->flags = flags|FLAG_TIMER_ENABLED;
//                       timer->echo_struct = echo_struct;
//                   }
//               }
//
//
//void test_timer(timeout_strct * timer){
//    if((timer->flags&FLAG_TIMER_ENABLED) &&(global_timer-timer->time) >=
//       timer->timeout){
//        timer->callback_fnc(timer, timer->echo_struct);
//        if(timer->flags & FLAG_TIMER_AUTORESET){
//            timer->time = global_timer;
//        }else{
//            timer->flags = 0;
//        }
//    }
//}



void set_sensor_read_function(timeout_strct * timer, sensor_read * sensor){
    //set_timer(timer, SENSOR_TIMEOUT_INTERVAL, timer_callback, sensor);
    
}

// MARK: - Scheduler



//Scheduler scheduler;



// MARK: - DST



// MARK: - MAIN

unsigned long loop_timeout = 15000;

int8_t temperature1 = -100;
int8_t temperature2 = -100;

DST dst1;
DST dst2;



void read_temperature_sensor1(SchedulerTimer *timer) {
    uint16_t newTimeout = dst_update(&dst1, &temperature1);
    scheduler_reset_timeout(timer, newTimeout);
}

void read_temperature_sensor2(SchedulerTimer *timer) {
    uint16_t newTimeout = dst_update(&dst2, &temperature2);
    scheduler_reset_timeout(timer, newTimeout);
}

void print_results() {
    NSLog(@"t1= %i, t2= %i", temperature1, temperature2);
}


- (void)testSensor {
    // setup
    loop_timeout = loop_timeout + millis();
    
    dst_setup(&dst1, 1);
    dst_setup(&dst2, 2);
    
    scheduler_setup(millis());
    
    scheduler_add(&read_temperature_sensor1, 500);
    scheduler_add(&read_temperature_sensor2, 500);
    scheduler_add(&print_results, 2000);
    
    // loop
    while (1) {
        
        scheduler_update(millis());
        
        if (millis() >= loop_timeout) {
            NSLog(@"done");
            return;
        }
        
    }
}

@end
