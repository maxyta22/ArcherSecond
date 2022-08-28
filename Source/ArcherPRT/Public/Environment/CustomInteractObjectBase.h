// Archer Prototype. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Environment/InteractObjectBase.h"
#include "Core/ArcherPRTData.h"
#include "Materials/Material.h"
#include "CustomInteractObjectBase.generated.h"

class UBoxComponent;


UCLASS()
class ARCHERPRT_API ACustomInteractObjectBase : public AInteractObjectBase
{
	GENERATED_BODY()

public:

	ACustomInteractObjectBase();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TMap<EResourcesType, int> ResoursesForInteract;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool ReturnResources;

	//Building

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Building")
		UMaterial* FailPreSpawnMaterial;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Building")
		UMaterial* SuccessPreSpawnMaterial;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		UBoxComponent* BoxGabaritCollision;

	APlayerCharacter* BuildingPlayer;

	bool CheckCanSpawnThisObject();

	void SetBuildingMaterial ();


	

protected:

	virtual void TryUseInteractObject(APlayerCharacter* Pawn) override;

private:

	bool Activate;

	

	
};
