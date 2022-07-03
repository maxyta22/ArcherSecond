// Archer Prototype. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Environment/InteractObjectBase.h"
#include "Core/ArcherPRTData.h"
#include "PickupResourcesBase.generated.h"


UCLASS()
class ARCHERPRT_API APickupResourcesBase : public AInteractObjectBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ResoursesType")
		EResourcesType ResourcesType = EResourcesType::Wood;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ResoursesValue")
		int Value = 1;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
		bool bInfinity;


protected:

		virtual void TryUseInteractObject(APlayerCharacter* Pawn) override;

private:

	void TakePickup();
};
