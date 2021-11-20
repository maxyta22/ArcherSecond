// Archer Prototype. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Environment/PickupBase.h"
#include "Enum/ResourcesTypeEnum.h"
#include "PickupResourcesBase.generated.h"


UCLASS()
class ARCHERPRT_API APickupResourcesBase : public APickupBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ResoursesType")
		EResourcesType ResourcesType = EResourcesType::Wood;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ResoursesValue")
		int Value = 1;

		
protected:

		virtual void TryTakePickup(APlayerCharacter* Pawn) override;
};
