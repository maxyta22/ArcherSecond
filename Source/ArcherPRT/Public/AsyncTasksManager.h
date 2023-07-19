// Archer Prototype. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Async/AsyncWork.h"
#include "AsyncTasksManager.generated.h"


class MyAsyncTask : FNonAbandonableTask {

	friend class FAsyncTask<MyAsyncTask>;

	MyAsyncTask();

	void DoWork();

	TStatId GetStatId() const;

};

UCLASS()
class ARCHERPRT_API AAsyncTasksManager : public AActor
{
	GENERATED_BODY()
	
public:	

	FAsyncTask<MyAsyncTask>* MyTask;

	AAsyncTasksManager();


protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};

