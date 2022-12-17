// Archer Prototype. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Core/ArcherPRTData.h"
#include "MathUtils.generated.h"


UCLASS()
class ARCHERPRT_API UMathUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "MathUtils") 
		static float FindAngleBetweenForwardVectorAndTarget(FVector Center, FVector Forward, FVector Target);
	
};
