// Archer Prototype. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Async/AsyncWork.h"
#include "Async/ThreadInterface.h"


class ARCHERPRT_API CommonTask : FNonAbandonableTask
{
	friend class FAsyncTask<CommonTask>;

	UObject* owner;

	CommonTask(UObject* owner);

	void DoWork();

	TStatId GetStatId() const;
};
