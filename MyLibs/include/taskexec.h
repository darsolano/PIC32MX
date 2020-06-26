/* 
 * File:   taskexec.h
 * Author: dsolano
 *
 * Created on July 15, 2014, 8:12 PM
 */

#ifndef TASKEXEC_H
#define	TASKEXEC_H

#ifdef	__cplusplus
extern "C" {
#endif
#include <xc.h>
#include <plib.h>
#include "utilities.h"
#include "UART_app.h"

#define CORE_TIMER_FCLK         (SYS_CLOCK/2)
#define TASK_INTERVAL_TIME      0x00000000
#define CORE_TICK_RATE          (GetSystemClock() / 2 / 10000)   // for 100us

// Definition, Constants, Enum, Structs for the application board - UNIVERSAL DEVELOPMENT BOARD
#define SYS_FREQ                    (80000000uL)
#define SYS_CLOCK                   (80000000uL)
#define	GetSystemClock()             (SYS_CLOCK)
#define	GetPeripheralClock()         (GetSystemClock()/(1 << OSCCONbits.PBDIV))
#define	GetInstructionClock()        (GetSystemClock())

typedef enum{
    ID_USER_TASK_0,
    ID_USER_TASK_1,
    ID_USER_TASK_2,
    MAX_TASK
}USER_DEF_TASK_t;


typedef enum{
    TASK_SUCCESS,
    TASK_TIMEOUT,
    TASK_COOPERATE,
    TASK_RESOURCE_BUSY,
    TASK_INTERNAL_ERROR
}TASK_ERRORS_t;

typedef enum{
    MAX_TASK0_COUNTER = 10,
    MAX_TASK1_COUNTER = 30,
    MAX_TASK2_COUNTER = 60,
}_TASK_MAX_EXECUTE_COUNTER;

typedef struct {
    BOOL    ReadyTaskToExec[MAX_TASK];
    BOOL    TaskInExec[MAX_TASK];
    UINT8   TaskLastError[MAX_TASK];
    UINT32  TimeToExec[MAX_TASK];
    UINT32  TaskCounter[MAX_TASK];
    UINT32  GlobalMaxTaskCounter;
}_TASK;
_TASK *_task = {0};

UINT32  MaxCoreTimerTaskCounter;
UCHAR8  PrintBuffer[48];


void    HandlerTaskExecutive(void);
void    HandlerUserTask0(void);
void    HandlerUserTask1(void);
void    HandlerUserTask2(void);

#ifdef	__cplusplus
}
#endif

#endif	/* TASKEXEC_H */

