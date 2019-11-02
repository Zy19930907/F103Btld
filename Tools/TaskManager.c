#include "TaskManager.h"
#include "malloc.h"

static inline void StartTask(void);
static inline void EndTask(void);
u64 TaskTickDiff(u64 tick);
static inline void DelayMs(u64 nms);

extern _Task sTask;
_Task eTask = {0, 0, 0, EndTask, &sTask, &sTask};
_Task sTask = {0, 0, 0, StartTask, &eTask, &eTask};
_TaskManger TaskManger = {0, 0, &sTask, &sTask, &eTask, TaskTickDiff, DelayMs};

//计算任务执行时钟
u64 TaskTickDiff(u64 tick)
{
	if (tick > TaskManger.tick)
		return 0xFFFFFFFFFFFFFFFF - tick + TaskManger.tick;
	else
		return TaskManger.tick - tick;
}

//默认创建任务，位于任务链表开始节点
static inline void StartTask(void)
{

}

//默认创建任务，位于任务链表结束节点
static inline void EndTask(void)
{

}

//创建新任务，添加到任务链表结束节点的上一节点
_Task *CreateTask(u64 day, u64 hour, u64 min, u64 sec, u64 ms, void(*Func)(void))
{
	_Task *Task = 0;
	Task = mymalloc(sizeof(_Task));
	if (Task != 0)
	{
		Task->Func = Func;
		Task->tick = TaskManger.tick;
		Task->period = ((day * 86400000) + (hour * 3600000) + (min * 60000) + (sec * 1000) +ms);        //计算任务执行周期
		Task->pre = TaskManger.lasTask->pre;
		Task->next = TaskManger.lasTask;
		TaskManger.lasTask->pre->next = Task;
		TaskManger.lasTask->pre = Task;
	}
	return Task;
}

//从任务链表中删除任务
void DelTask(_Task *delTask)
{
	if (delTask != 0)
	{
		delTask->pre->next = delTask->next;
		delTask->next->pre = delTask->pre;
		myfree(delTask);
	}
}

//修改任务执行周期
void SetTaskPeriod(u64 day, u64 hour, u64 min, u64 sec, u64 ms, _Task *Task)
{
	Task->period = ((day * 86400000) + (hour * 3600000) + (min * 60000) + (sec * 1000) + ms);        //计算任务执行周期
}

volatile u64 delayTick;
void DelayMs(u64 nms)
{
	delayTick = TaskManger.tick;
	while (TaskTickDiff(delayTick) < nms);
}

//循环执行任务链表中的任务
void ExecTask(void)
{
	_Task *RunTask = &sTask;
	for (;;)
	{
		if (TaskTickDiff(RunTask->tick) >= RunTask->period)
		{
			RunTask->tick = TaskManger.tick;
			RunTask->Func();//调用任务处理函数
		}
		RunTask = RunTask->next;
	}
}

