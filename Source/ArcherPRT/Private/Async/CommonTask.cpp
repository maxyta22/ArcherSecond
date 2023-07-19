// Archer Prototype. All rights reserved


#include "Async/CommonTask.h"


CommonTask::CommonTask(UObject* owner) : owner(owner)
{
}

void CommonTask::DoWork()
{
	IThreadInterface::Execute_StartAsyncWork(owner);
}

TStatId CommonTask::GetStatId() const
{
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(CommonTask, STATGROUP_ThreadPoolAsyncTasks);
	}
}