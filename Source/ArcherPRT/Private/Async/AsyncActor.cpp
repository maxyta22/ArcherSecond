// Archer Prototype. All rights reserved


#include "Async/AsyncActor.h"


AAsyncActor::AAsyncActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AAsyncActor::BeginPlay()
{
	Super::BeginPlay();

	FAsyncTask<MyAsyncTask>* myTask = new FAsyncTask<MyAsyncTask>();
	myTask->StartBackgroundTask();
	
}

void AAsyncActor::Tick(float DeltaTime)
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
