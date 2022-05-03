// Archer Prototype. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Environment/PickupBase.h"
#include "Core/ArcherPRTData.h"
#include "RecipeBase.generated.h"

class APickupBase;


UCLASS(ClassGroup = (Custom), blueprinttype, blueprintable, meta = (BlueprintSpawnableComponent))
class ARCHERPRT_API URecipeBase : public UObject
{
	GENERATED_BODY()

public:

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recipe")
		TMap<EResourcesType, int> RecipeMap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Result")
		EAmmoType Ammo;

	
};
