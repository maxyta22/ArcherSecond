// Archer Prototype. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "GameplayAbilitySystem/PRTAttributeSet.h"
#include "LandedDamageCalculation.generated.h"

struct AttStruct
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Health);
	DECLARE_ATTRIBUTE_CAPTUREDEF(DamageMod);

	AttStruct()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UPRTAttributeSet, Health, Target, false);
		//DEFINE_ATTRIBUTE_CAPTUREDEF(UPRTAttributeSet, DamageMod, Source, true);
	}

};

UCLASS()
class ARCHERPRT_API ULandedDamageCalculation : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:

	ULandedDamageCalculation();

	//UProperty* HealthProperty;
	//FGameplayEffectAttributeCaptureDefinition HealthDef;

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
	
};
