// Archer Prototype. All rights reserved


#include "AsyncTasksManager.h"
#include "Async/AsyncWork.h"

// Sets default values
AAsyncTasksManager::AAsyncTasksManager()
{
	bAllowTickBeforeBeginPlay = true;
	PrimaryActorTick.bCanEverTick = true;

}

void AAsyncTasksManager::BeginPlay()
{

	MyTask = new FAsyncTask<MyAsyncTask>();
	MyTask->StartBackgroundTask();
}

void AAsyncTasksManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//MyTask->Cancel();

	if (MyTask->IsDone())
	{
		GEngine->AddOnScreenDebugMessage(-1, 0.1, FColor::Green, "Complete");
	}
	
}

///////////////////////////// My Async Task

MyAsyncTask::MyAsyncTask()
{
}

void MyAsyncTask::DoWork()
{
	for (int i = 0; i < 100000; i++)
	{
		FString message = FString::Printf(TEXT("intVar: %d"), i);
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Green, message);
	
	}	
}

TStatId MyAsyncTask::GetStatId() const
{
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(MyAsyncTask, STATGROUP_ThreadPoolAsyncTasks);
	}
}
