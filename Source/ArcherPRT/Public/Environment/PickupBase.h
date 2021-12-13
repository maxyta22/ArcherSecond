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

	


	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Description")
		void ShowInfo();
		void ShowInfo_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Description")
		void HideInfo();
		void HideInfo_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Take")
		void AfterWasTaken();

	UFUNCTION(Server, Reliable)
		virtual void TryTakePickup_Server(APlayerCharacter* Pawn);


protected:

	virtual void BeginPlay() override;	

	UFUNCTION(Server, Reliable)
	void TakePickup_Server();

	//UFUNCTION(NetMulticast, Reliable)
	//void TakePickupMulticast();
	//void TakePickupMulticast_Implementation();


	
	
	
};
