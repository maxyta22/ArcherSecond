// Archer Prototype. All rights reserved


#include "Components/StatsComponent.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "TimerManager.h"

DEFINE_LOG_CATEGORY_STATIC(LogHealthComponent, All, All)


UStatsComponent::UStatsComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UStatsComponent::BeginPlay()
{
	Super::BeginPlay();

	SetHealth(MaxHealth);

	// Bind Delegate Take Damage
	AActor* ComponentOwner = GetOwner();
	if (ComponentOwner)
	{
		ComponentOwner->OnTakeAnyDamage.AddDynamic(this, &UStatsComponent::OnTakeAnyDamage);
	}
}

void UStatsComponent::OnTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatorBy, AActor* DamageCauser)
{
	if (Damage <= 0.0 || IsDead() || !GetWorld())return;

	SetHealth(Health - Damage);
	GetWorld()->GetTimerManager().ClearTimer(HealTimerHandle);


	if (IsDead())
	{
		OnDeath.Broadcast();
		UE_LOG(LogHealthComponent, Display, TEXT("Death"));
	}
	else
		if (AutoHeal)
		{
			GetWorld()->GetTimerManager().SetTimer(HealTimerHandle, this, &UStatsComponent::RegenHeals, HealUpdateTime, true, HealDelay);
		}

}

void UStatsComponent::RegenHeals()
{
	SetHealth(Health + HealModifire);
	if (FMath::IsNearlyEqual(Health, MaxHealth) && GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(HealTimerHandle);
	}
}

void UStatsComponent::SetHealth(float NewHealth)
{
	Health = FMath::Clamp(NewHealth, 0.0f, MaxHealth);
	OnHealthChanged.Broadcast(Health);
}

