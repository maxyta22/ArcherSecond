// Archer Prototype. All rights reserved


#include "GameplayAbilitySystem/PRTAttributeSet.h"
#include "Net/UnrealNetwork.h"

UPRTAttributeSet::UPRTAttributeSet()
{
}

void UPRTAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION_NOTIFY(UPRTAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPRTAttributeSet, Stamina, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPRTAttributeSet, Power, COND_None, REPNOTIFY_Always);
}

void UPRTAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPRTAttributeSet, Health, OldHealth)
}

void UPRTAttributeSet::OnRep_Stamina(const FGameplayAttributeData& OldStamina)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPRTAttributeSet, Stamina, OldStamina)
}

void UPRTAttributeSet::OnRep_Power(const FGameplayAttributeData& OldPower)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPRTAttributeSet, Power, OldPower)
}
