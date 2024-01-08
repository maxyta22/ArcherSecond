// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Net/UnrealNetwork.h"
#include "Interfaces/InteractInterface.h"
#include "Interfaces/TakeDamageInterface.h"
#include "Interfaces/ToggleActivateAssetInterface.h"
#include "InteractObjectBase.generated.h"

class APlayerCharacter;
class USphereComponent;
class UStaticMeshComponent;
class USkeletalMeshComponent;
class UArrowComponent;

UCLASS()
class ARCHERPRT_API AInteractObjectBase : public AActor, public IInteractInterface, public ITakeDamageInterface, public IToggleActivateAssetInterface
{
	GENERATED_BODY()
	
public:	
	
	AInteractObjectBase();

	UPROPERTY(VisibleAnywhere, Category = "Components")
		UArrowComponent* PivotArrow;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		USphereComponent* SphereCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(VisibleAnywhere, Category = "Components")
		USkeletalMeshComponent* SkeletalMeshComponent;

	// Show Info

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Description")
		void ShowInfo();	

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Description")
		void HideInfo();

	// Blueprint Implementable Events

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "After Events")
		void AfterWasTryUseInteractObject();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "After Events")
		void AfterWasSuccessUseInteractObject();

	virtual void TryUseInteractObject(APlayerCharacter* Pawn);

	//Optimization

	UPROPERTY(EditDefaultsOnly, Category = "Optimization")
	bool AllowTickIfActivated;

	//Interface

	void I_Interact_Implementation(APlayerCharacter* Pawn) override;

	void I_ShowInfo_Implementation() override;

	void I_HideInfo_Implementation() override;

	void I_TakeDamage_Implementation(FDamageData DamageData) override;

	void I_ToggleActivate_Implementation(bool Activate);

protected:

	virtual void BeginPlay() override;	

private:

	bool Activated = true;

	




	
	
	
};
