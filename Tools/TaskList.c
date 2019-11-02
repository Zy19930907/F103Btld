#include "TaskList.h"
#include "Led.h"

_Task *RunLedTaskTcb;
_Task *IapTaskTcb;

void TasksInit(void)
{
	RunLedTaskTcb = CreateTask(0, 0, 0, 0, 50, RunLedTask);                       		 //运行指示灯
	IapTaskTcb = CreateTask(0,0,0,0,200,IapTask);
}
