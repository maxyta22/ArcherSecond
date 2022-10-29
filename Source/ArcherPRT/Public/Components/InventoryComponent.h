// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Core/ArcherPRTData.h"
#include "InventoryComponent.generated.h"


USTRUCT(BlueprintType)
struct FResourcesData
{
	GENERATED_USTRUCT_BODY()

	public:

	//Ammo
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int ValueArrow;

	//Resources
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int ValueWood;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int ValueRock;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int ValueGrass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int ValueMetal;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int ValueFood;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int ValueBattery;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int ValueLamp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int ValueMine;
};

USTRUCT(BlueprintType)
struct FMaxResourcesData
{
	GENERATED_USTRUCT_BODY()

	public:

	//Ammo
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int MaxArrow;

	//Resources
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int MaxWood;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int MaxRock;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int MaxGrass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int MaxMetal;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int MaxFood;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int MaxBattery;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int MaxLamp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int MaxMine;
};


UCLASS(ClassGroup = (Custom), blueprinttype, blueprintable, meta = (BlueprintSpawnableComponent))
class ARCHERPRT_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:		
	
	UInventoryComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FResourcesData Resources;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FMaxResourcesData MaxResources;

	UFUNCTION(BlueprintPure, Category = "resources")
		bool LoopOnResourcesByMap(TMap<EResourcesType, int> ResourcesMap, bool SpendResources = false, bool AddResources = false);

	UFUNCTION(BlueprintPure, Category = "resources")
		int GetValueResources(EResourcesType ResourcesType);

	UFUNCTION(BlueprintCallable, Category = "resources")
		void AddResources(EResourcesType ResourcesType, int Value);

	UFUNCTION(BlueprintPure, Category = "Resources")
		int GetMaxResources(EResourcesType ResourcesType);

	UFUNCTION(BlueprintCallable, Category = "Resources")
		void SetMaxResources(EResourcesType ResourcesType, int Value);

	UFUNCTION(BlueprintPure, Category = "Resources")
		bool CheckCanTakeResources(EResourcesType ResourcesType);

protected:	
	
	virtual void BeginPlay() override;

private:
	
	FResourcesData ResourcesData;
	FMaxResourcesData MaxResourcesData;

		
		
};
