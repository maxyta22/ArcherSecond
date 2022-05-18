// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Net/UnrealNetwork.h"
#include "PickupBase.generated.h"



class APlayerCharacter;
class USphereComponent;
class UStaticMeshComponent;
class USkeletalMeshComponent;



UCLASS()
class ARCHERPRT_API APickupBase : public AActor
{
	GENERATED_BODY()
	
public:	
	
	APickupBase();

	UPROPERTY(VisibleAnywhere, Category = "Components")
		USphereComponent* SphereCollision;

	UPROPERTY(VisibleAnywhere, Category = "Components")
		UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(VisibleAnywhere, Category = "Components")
		USkeletalMeshComponent* SkeletalMeshComponent;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Description")
		void ShowInfo();	

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Description")
		void HideInfo();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Take")
		void AfterWasTaken();

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
		bool bInfinity;

	virtual void TryTakePickup(APlayerCharacter* Pawn);

protected:

	virtual void BeginPlay() override;	

	void TakePickup();




	
	
	
};
