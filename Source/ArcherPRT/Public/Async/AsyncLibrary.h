// Archer Prototype. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Async/AsyncWork.h"
#include "Async/CommonTask.h"
#include "AsyncLibrary.generated.h"



UCLASS()
class ARCHERPRT_API UAsyncLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
		static void InitAsyncWorker(UObject* object);
		
	
};
