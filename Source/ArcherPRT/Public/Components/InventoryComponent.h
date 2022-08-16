// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Core/ArcherPRTData.h"
#include "InventoryComponent.generated.h"


USTRUCT()
	struct FResoursesData
	{
		GENERATED_USTRUCT_BODY()
		int ValueWood;
		int ValueRock;
		int ValueGrass;
		int ValueMetal;
		int ValueFood;
	};

USTRUCT()
	struct FMaxResoursesData
	{
		GENERATED_USTRUCT_BODY()
		int MaxWood = 5;
		int MaxRock = 5;
		int MaxGrass = 5;
		int MaxMetal = 5;
		int MaxFood  =5;
};

USTRUCT()
	struct FAmmoData
	{
		GENERATED_USTRUCT_BODY()
		int ValueWoodArrow = 5;
		int ValueRockArrow = 0;
		int ValueMetalArrow = 0;
	};

USTRUCT()
struct FMaxAmmoData
{
	GENERATED_USTRUCT_BODY()
		int MaxWoodArrow = 2;
		int MaxRockArrow = 5;
		int MaxMetalArrow = 3;
};


UCLASS(ClassGroup = (Custom), blueprinttype, blueprintable, meta = (BlueprintSpawnableComponent))
class ARCHERPRT_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:		
	
	UInventoryComponent();

	UFUNCTION(BlueprintPure, Category = "resources")
		bool CheckHasResources(TMap<EResourcesType, int> ResourcesMap);

	// Get Inventory Contents resources
	UFUNCTION(BlueprintPure, Category = "resources")
		int GetValueResourses(EResourcesType ResourcesType);

	// Add Inventory Contents resources
	UFUNCTION(BlueprintCallable, Category = "resources")
		void AddResources(EResourcesType ResourcesType, int Value);

	// GetMaxResourses
	UFUNCTION(BlueprintPure, Category = "Resources")
		int GetMaxResourses(EResourcesType ResourcesType);

	// SetMaxResourses
	UFUNCTION(BlueprintCallable, Category = "Resources")
		void SetMaxResourses(EResourcesType ResourcesType, int Value);

	// Get Inventory Contents Ammo
	UFUNCTION(BlueprintPure, Category = "Ammo")
		int GetValueAmmo(EAmmoType AmmoType);

	//Get Max Ammo
	UFUNCTION(BlueprintPure, Category = "Ammo")
		int GetMaxAmmo(EAmmoType AmmoType);

	//Set Max Ammo
	UFUNCTION(BlueprintCallable, Category = "Ammo")
		void SetMaxAmmo(EAmmoType AmmoType, int Value);

	UFUNCTION(BlueprintCallable, Category = "Ammo")
		void SetAmmo(EAmmoType AmmoType, int Value);

	// Add Inventory Contents Ammo
	UFUNCTION(BlueprintCallable, Category = "Ammo")
		void AddAmmo(EAmmoType AmmoType, int Value);

	// Check Can Take Resources
	UFUNCTION(BlueprintPure, Category = "Resources")
		bool CheckCanTakeResources(EResourcesType ResourcesType);

	UFUNCTION(BlueprintPure, Category = "Resources")
		bool CheckCanTakeAmmo(EAmmoType AmmoType);

	
	

protected:	
	
	virtual void BeginPlay() override;

private:
	
	FResoursesData ResoursesData;
	FMaxResoursesData MaxResoursesData;
	FAmmoData AmmoData;
	FMaxAmmoData MaxAmmoData;

		
		
};
