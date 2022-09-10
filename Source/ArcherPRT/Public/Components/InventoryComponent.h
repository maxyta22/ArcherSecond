// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Core/ArcherPRTData.h"
#include "InventoryComponent.generated.h"


USTRUCT(BlueprintType)
struct FResoursesData
{
	GENERATED_USTRUCT_BODY()

	public:

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
struct FMaxResoursesData
{
	GENERATED_USTRUCT_BODY()

	public:

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

USTRUCT(BlueprintType)
struct FAmmoData
{
	GENERATED_USTRUCT_BODY()

	public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int ValueWoodArrow;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int ValueRockArrow;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int ValueMetalArrow;
};

USTRUCT(BlueprintType)
struct FMaxAmmoData
{
	GENERATED_USTRUCT_BODY()

	public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int MaxWoodArrow;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int MaxRockArrow;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int MaxMetalArrow;
};

UCLASS(ClassGroup = (Custom), blueprinttype, blueprintable, meta = (BlueprintSpawnableComponent))
class ARCHERPRT_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:		
	
	UInventoryComponent();

	//-----------------------------------------------------------------------------------//

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FResoursesData Resources;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FMaxResoursesData MaxResources;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FAmmoData  Ammo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FMaxAmmoData  MaxAmmo;

	//------------------------------------------------------------------------------------//

	UFUNCTION(BlueprintPure, Category = "resources")
		bool LoopOnResourcesByMap(TMap<EResourcesType, int> ResourcesMap, bool SpendResources = false, bool AddResources = false);

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
