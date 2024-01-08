// Archer Prototype. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ToggleActivateAssetInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UToggleActivateAssetInterface : public UInterface
{
	GENERATED_BODY()
};


class ARCHERPRT_API IToggleActivateAssetInterface
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Optimization")
	void I_ToggleActivate(bool Activate);
};
