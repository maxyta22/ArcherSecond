// Archer Prototype. All rights reserved


#include "GameplayAbilitySystem/LandedDamageCalculation.h"
#include "GameplayAbilitySystem/PRTAttributeSet.h"
#include "GameplayEffectAggregator.h"
#include "Player/GameCharacter.h"

ULandedDamageCalculation::ULandedDamageCalculation()
{	
	AttStruct Attributes;
	RelevantAttributesToCapture.Add(Attributes.HealthDef);
	//InvalidScopedModifierAttributes.Add(Attributes.HealthDef);

}



void ULandedDamageCalculation::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	AttStruct Attributes;

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();
	UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();

	AActor* TargetActor = TargetASC ? TargetASC->GetAvatarActor() : nullptr;
	AActor* SourceActor = SourceASC ? SourceASC->GetAvatarActor() : nullptr;

	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();

	//FAggregatorEvaluateMetaData EvalutionParameters;
	//EvalutionParameters.TargetTags = TargetTags;
	//EvalutionParameters.SourceTags = SourceTags;

	//float Health = 0.f;
	//ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(Attributes.HealthDef, EvalutionParameters, Health);
	
	AGameCharacter* AvatarGameChar = Cast<AGameCharacter>(ExecutionParams.GetTargetAbilitySystemComponent()->GetAvatarActor());

	if (IsValid(AvatarGameChar))
	{
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(Attributes.HealthProperty, EGameplayModOp::Additive, -AvatarGameChar->LastDamageFromLanded));
	}
}

