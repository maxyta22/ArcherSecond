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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resources")
		EResourcesType ResourcesType = EResourcesType::Wood;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "!ResourcesType == EResourcesType::None"), Category = "Resources")
		int Value = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "ResourcesType == EResourcesType::None"), Category = "Resources")
		TMap<EResourcesType, int> ResourcesList;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Resources")
		bool bInfinity;

	UFUNCTION(BlueprintCallable, Category = "resources")
		void SetContainer(AActor* ActorContainer) { Container = ActorContainer; };


protected:

		virtual void TryUseInteractObject(APlayerCharacter* Pawn) override;

private:

	void TakePickup();

	AActor* Container;

};
