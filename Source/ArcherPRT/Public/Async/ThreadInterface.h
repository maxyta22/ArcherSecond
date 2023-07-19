// Archer Prototype. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ThreadInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UThreadInterface : public UInterface
{
	GENERATED_BODY()
};


class ARCHERPRT_API IThreadInterface
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void StartAsyncWork();
};
