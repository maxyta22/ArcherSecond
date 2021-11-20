// Archer Prototype. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StatsComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnDeath)
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHealthChanged, float)


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ARCHERPRT_API UStatsComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UStatsComponent();

	UFUNCTION( BlueprintCallable)
		float GetHealth() const { return Health; }

	float GetMaxHealh() const { return MaxHealth; }

	void AddHealth(float Value) { SetHealth(GetHealth() + 50); }

	UFUNCTION(BlueprintPure)
		bool IsDead() { bool Result; return Result = (GetHealth() <= 0) ? true : false; }


	// Delegate
	FOnDeath OnDeath;
	FOnHealthChanged OnHealthChanged;

protected:

	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health")
		float MaxHealth = 100.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal")
		bool AutoHeal = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal", meta = (EditCondition = "AutoHeal"))
		float HealDelay = 3.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal", meta = (EditCondition = "AutoHeal"))
		float HealUpdateTime = 0.3f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal", meta = (EditCondition = "AutoHeal"))
		float HealModifire = 1.0f;

private:

	float  Health = 0.0f;
	FTimerHandle HealTimerHandle;

	UFUNCTION()
		void OnTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatorBy, AActor* DamageCauser);


	void RegenHeals();
	void SetHealth(float NewHealth);


};
