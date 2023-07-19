// Archer Prototype. All rights reserved


#include "AsyncTasksManager.h"

// Sets default values
AAsyncTasksManager::AAsyncTasksManager()
{

	PrimaryActorTick.bCanEverTick = true;

}


void AAsyncTasksManager::BeginPlay()
{
	Super::BeginPlay();
	FAsyncTask<MyAsyncTask>* myTask = new FAsyncTask<MyAsyncTask>();
	myTask->StartBackgroundTask();
}

void AAsyncTasksManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

MyAsyncTask::MyAsyncTask()
{
}

void MyAsyncTask::DoWork()
{
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Green, "I Am do Work");
}

TStatId MyAsyncTask::GetStatId() const
{
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(MyAsyncTask, STATGROUP_ThreadPoolAsyncTasks);
	}
}
