
#include "taskexec.h"

//const char task0[] = "Running task 0\n ";
//const char task1[] = "Running task 1\n ";
//const char task2[] = "Running task 2\n ";
//
//
//void HandlerTaskExecutive(void){
//	while(1)
//	{
//		if (_task->ReadyTaskToExec[ID_USER_TASK_0] == TRUE){
//			_task->TaskInExec[ID_USER_TASK_0] == TRUE;
//			HandlerUserTask0();
//			_task->ReadyTaskToExec[ID_USER_TASK_0] = FALSE;
//			_task->TaskCounter[ID_USER_TASK_0] = 0;
//			_task->TimeToExec[ID_USER_TASK_0] = ReadCoreTimer() - MaxCoreTimerTaskCounter;
//		}
//		if (_task->ReadyTaskToExec[ID_USER_TASK_1] == TRUE){
//			_task->TaskInExec[ID_USER_TASK_1] == TRUE;
//			HandlerUserTask1();
//			_task->ReadyTaskToExec[ID_USER_TASK_1] = FALSE;
//			_task->TaskCounter[ID_USER_TASK_1] = 0;
//			_task->TimeToExec[ID_USER_TASK_1] = ReadCoreTimer() - MaxCoreTimerTaskCounter;
//		}
//		if (_task->ReadyTaskToExec[ID_USER_TASK_2] == TRUE){
//			_task->TaskInExec[ID_USER_TASK_2] == TRUE;
//			HandlerUserTask2();
//			_task->ReadyTaskToExec[ID_USER_TASK_2] = FALSE;
//			_task->TaskCounter[ID_USER_TASK_2] = 0;
//			_task->TimeToExec[ID_USER_TASK_2] = ReadCoreTimer() - MaxCoreTimerTaskCounter;
//		}
//
//	}
//}
//void HandlerUserTask0(void){
//}
//
//void HandlerUserTask1(){
//	PORTToggleBits(IOPORT_D , BIT_2);
//	SendDataBuffer(task1,strlen(task1));
//}
//
//void HandlerUserTask2(){
//	PORTToggleBits(IOPORT_D , BIT_4);
//	SendDataBuffer(task2,strlen(task2));
//}
