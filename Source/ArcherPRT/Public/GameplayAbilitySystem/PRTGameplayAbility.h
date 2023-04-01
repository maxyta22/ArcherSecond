// Archer Prototype. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Core/ArcherPRTData.h"
#include "PRTGameplayAbility.generated.h"


UCLASS()
class ARCHERPRT_API UPRTGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:

	UPRTGameplayAbility();

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
		EAbilityInputID AbilityInputID = EAbilityInputID::None;

	
};
