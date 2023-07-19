// Archer Prototype. All rights reserved


#include "Async/AsyncLibrary.h"

 void UAsyncLibrary::InitAsyncWorker(UObject* object)
{
	FAsyncTask<CommonTask>* newWorker = new FAsyncTask<CommonTask>(object);
	newWorker->StartBackgroundTask();
}