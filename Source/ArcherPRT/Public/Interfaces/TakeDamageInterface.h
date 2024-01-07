// Archer Prototype. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Core/ArcherPRTData.h"
#include "TakeDamageInterface.generated.h"

UINTERFACE(MinimalAPI)
class UTakeDamageInterface : public UInterface
{
	GENERATED_BODY()
};


class ARCHERPRT_API ITakeDamageInterface
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Damage")
	void I_TakeDamage(FDamageData DamageData);

};
