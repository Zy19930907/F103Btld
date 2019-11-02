#ifndef __TASKMANGER_H
#define __TASKMANGER_H

#include "Public.h"

typedef struct _task
{
	u64 tick;			//任务执行时钟
	u64 period;		//任务执行间隔
	u64 time;
	void(*Func)(void);			//任务具体执行函数指针
	struct _task *pre;
	struct _task *next;
}_Task;

typedef struct
{
	u64 exeTime;
	volatile u64 tick;
	_Task *curTask;
	_Task *firTask;
	_Task *lasTask;
	u64(*TaskDiff)(u64 tick);
	void(*delay_ms)(u64 nms);
}_TaskManger;

extern _TaskManger TaskManger;

extern _Task* CreateTask(u64 day,u64 hour,u64 min,u64 sec,u64 ms,void(*Func)(void));
extern void SetTaskPeriod(u64 day, u64 hour, u64 min, u64 sec, u64 ms, _Task *Task);
extern void DelTask(_Task *delTask);
extern void ExecTask(void);

#endif
