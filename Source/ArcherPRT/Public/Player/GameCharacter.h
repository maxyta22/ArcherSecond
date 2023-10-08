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
#include "GameplayEffect.h"
#include "GameCharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class USceneComponent;
class UCameraComponent;
class UMotionControllerComponent;
class UAnimMontage;
class USoundBase;
class UInventoryComponent;
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

#pragma region Components

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UWeaponComponent* WeaponComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UPRTAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UPRTAttributeSet* Attributes;

#pragma endregion 

#pragma region Abilitities

public:

	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	virtual void InitializeAttributes();
	virtual void GiveAbilities();

	virtual void PossessedBy(AController* NewController) override;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "GAS")
	TSubclassOf<class UGameplayEffect> DefaultAttributeEffect;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "GAS")
	TArray<TSubclassOf<class UPRTGameplayAbility>> DefaultAbilities;

	UFUNCTION(BlueprintPure, Category = "Attributes")
	float GetHealth() { return Attributes->GetHealth();}

	UFUNCTION(BlueprintPure, Category = "Attributes")
	float GetMaxHealth() { return Attributes->GetMaxHealth();}

	UFUNCTION(BlueprintPure, Category = "Attributes")
	bool IsAlive();

	UPROPERTY()
	bool WasInitiatedAttributes;

#pragma endregion

#pragma region TakeDamage

public:

	UFUNCTION(BlueprintCallable)
	virtual void ImplementTakeDamage(FDamageData DamageData);

	UFUNCTION()
	bool IsInvulnerable();

	UFUNCTION(BlueprintImplementableEvent, Category = "AfterEvents")
	void AfterTakeDamage(FDamageData DamageData);

	UFUNCTION(BlueprintImplementableEvent, Category = "AfterEvents")
	void MissTakeDamage();

	UFUNCTION(BlueprintImplementableEvent, Category = "AfterEvents")
	void AfterOnDeath();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "AfterEvents")
	void HitOnBlockSuccess(FDamageData DamageData);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "AfterEvents")
	void FailTryUseBlock();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Internal")
	bool bInvulnerable;

#pragma endregion

#pragma region MakeDamage

public:

	virtual void MakeStrike(float StrikeDistance, float MinAngle, float MaxAngle, bool IgnoreBlock = false, bool MakeStagger = false);

	void ClearTempInternalActors();

	bool CheckHitSuccess() { return SuccessDamageCount > 0; };

	bool CheckMissSuccess() { return SuccessDamageCount == 0; };

	void MakeMiss();

	void AddSuccessDamageCount() { SuccessDamageCount++; }

	UFUNCTION(BlueprintImplementableEvent, Category = "Strike")
	void StrikeInProgress();

	UFUNCTION(BlueprintImplementableEvent, Category = "Strike")
	void ChargeSuccess();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Strike")
	TSubclassOf<UGameplayEffect> StrikeGameplayEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Strike")
	USoundBase* HitOnSuccessSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Strike")
	USoundBase* HitOnBlockSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Strike")
	UNiagaraSystem* HitOnSuccessVFX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Strike")
	UNiagaraSystem* HitOnBlockVFX;

	UPROPERTY()
	TArray<AActor*> DamageActors;

	UPROPERTY()
	TArray<AActor*> IgnoreActorsDamage;

	int SuccessDamageCount;

	virtual void OnDeath();

#pragma endregion

#pragma region Movement

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Internal")
	bool bIsUnderground;

	//Landed
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	FVector2D LandedDamageVelocity = FVector2D(900.0f, 1200.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	FVector2D LandedDamage = FVector2D(10.0f, 100.0f);

protected:

	virtual void Landed(const FHitResult& Hit);

#pragma endregion










	

	

	
	
	


};