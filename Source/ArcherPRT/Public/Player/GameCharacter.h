// Archer Prototype. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include <GameplayEffectTypes.h>
#include "AbilitySystemComponent.h"
#include "GameplayAbilitySystem/PRTAttributeSet.h"
#include "Core/ArcherPRTData.h"
#include "NiagaraComponent.h"
#include "GameCharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class USceneComponent;
class UCameraComponent;
class UMotionControllerComponent;
class UAnimMontage;
class USoundBase;
class UInventoryComponent;
class UStatsComponent;
class UWeaponComponent;
class UPRTAbilitySystemComponent;
class UPRTAttributeSet;
class UBoxComponent;

UCLASS(config = Game)
class AGameCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:

	AGameCharacter();

	virtual void BeginPlay() override;

	//Components

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
		UStatsComponent* StatsComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
		UWeaponComponent* WeaponComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
		UPRTAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
		UPRTAttributeSet* Attributes;

	// Gameplay Ability System
	
	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	virtual void InitializeAttributes();
	virtual void GiveAbilities();

	virtual void PossessedBy(AController* NewController) override;


	//Default Effect for attributes
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "GAS")
		TSubclassOf<class UGameplayEffect> DefaultAttributeEffect;

	//Default Abilities
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "GAS")
		TArray<TSubclassOf<class UPRTGameplayAbility>> DefaultAbilities;

	//Attributes

	UFUNCTION(BlueprintPure, Category = "Attributes")
		float GetHealth() { return Attributes->GetHealth();}

	UFUNCTION(BlueprintPure, Category = "Attributes")
		float GetMaxHealth() { return Attributes->GetMaxHealth();}

	UFUNCTION(BlueprintPure, Category = "Attributes")
		bool IsAlive() { return Attributes->GetHealth() > 0; }

	UFUNCTION()
		void OnHealthAttributeChanged();



	//Take Damage
	
	UFUNCTION()
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION()
		bool IsInvulnerable();

	UFUNCTION(BlueprintImplementableEvent, Category = "AfterEvents")
		void AfterTakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser);

	UFUNCTION(BlueprintImplementableEvent, Category = "AfterEvents")
		void AfterOnDeath();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "AfterEvents")
		void HitOnBlockSuccess(FVector HitDirection, FHitResult HitResult, AActor* Causer, EWeaponType WeaponType, bool Charged);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "AfterEvents")
		void FailTryUseBlock();

	//Hit Reaction
	UFUNCTION(BlueprintCallable)
	virtual void OnHit(FVector HitDirection, FHitResult HitResult, AActor* Causer, EWeaponType WeaponType, bool Charged);

	//Landed
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		FVector2D LandedDamageVelocity = FVector2D(900.0f, 1200.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		FVector2D LandedDamage = FVector2D(10.0f, 100.0f);

	//Make Strike

	virtual void MakeStrike(float StrikeDistance, float MinAngle, float MaxAngle);

	void ClearIgnoreActorsDamageStrike();

	UFUNCTION(BlueprintImplementableEvent, Category = "Strike")
	void StrikeInProgress();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Strike")
		float StrikeDamage = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Strike")
		USoundBase* HitOnSuccessSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Strike")
		USoundBase* HitOnBlockSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Strike")
		UNiagaraSystem* HitOnSuccessVFX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Strike")
		UNiagaraSystem* HitOnBlockVFX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Strike")
		TSubclassOf<UDamageType> StrikeDamageType;

	// Internal
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Internal")
		bool bInvulnerable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Internal")
		bool bIsUnderground;

	
protected:

	virtual void OnDeath();

	virtual void Landed(const FHitResult& Hit);

	TArray<AActor*> IgnoreActorsDamage;

private:

	bool CanCheckAttributes;

	void OnHealChanged(float Health);

	

	

	
	
	


};