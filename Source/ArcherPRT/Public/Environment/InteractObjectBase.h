// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Net/UnrealNetwork.h"
#include "InteractObjectBase.generated.h"



class APlayerCharacter;
class USphereComponent;
class UStaticMeshComponent;
class USkeletalMeshComponent;
class UArrowComponent;

UCLASS()
class ARCHERPRT_API AInteractObjectBase : public AActor
{
	GENERATED_BODY()
	
public:	
	
	AInteractObjectBase();

	UPROPERTY(VisibleAnywhere, Category = "Components")
		UArrowComponent* PivotArrow;

	UPROPERTY(VisibleAnywhere, Category = "Components")
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

protected:

	virtual void BeginPlay() override;	

	




	
	
	
};
