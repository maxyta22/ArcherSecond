// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Enum/ResourcesTypeEnum.h"
#include "Enum/AmmoTypeEnum.h"
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
		int MaxWood;
		int MaxRock;
		int MaxGrass;
		int MaxMetal;
		int MaxFood;
};

USTRUCT()
	struct FAmmoData
	{
		GENERATED_USTRUCT_BODY()
		int ValueWoodArrow = 0;
		int ValueRockArrow = 5;
		int ValueMetalArrow = 3;
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

	// Get Inventory Contents resources
	UFUNCTION(BlueprintPure, Category = "resources")
		int GetValueResourses(EResourcesType ResourcesType);

	// Get Inventory Contents Ammo
	UFUNCTION(BlueprintPure, Category = "Ammo")
		int GetValueAmmo(EAmmoType AmmoType);

	//Get Max Ammo
	UFUNCTION(BlueprintPure, Category = "Ammo")
		int GetMaxAmmo(EAmmoType AmmoType);


	//Set Max Ammo
	UFUNCTION(BlueprintCallable, Category = "Ammo")
		void SetMaxWoodArrow(int value)  {MaxAmmoData.MaxWoodArrow = value; }
	UFUNCTION(BlueprintCallable, Category = "Ammo")
		void SetMaxRockArrow(int value)  {MaxAmmoData.MaxRockArrow = value; }
	UFUNCTION(BlueprintCallable, Category = "Ammo")
		void SetMaxMetalArrow(int value)  {MaxAmmoData.MaxMetalArrow = value; }

	// Add Inventory Contents resources
	UFUNCTION(BlueprintCallable, Category = "resources")
		void AddWood(int value)  {ResoursesData.ValueWood = ResoursesData.ValueWood + value;}
	UFUNCTION(BlueprintCallable, Category = "resources")
		void AddRock(int value)  {ResoursesData.ValueRock = ResoursesData.ValueRock + value;}
	UFUNCTION(BlueprintCallable, Category = "resources")
		void AddGrass(int value) {ResoursesData.ValueGrass = ResoursesData.ValueGrass + value;}
	UFUNCTION(BlueprintCallable, Category = "resources")
		void AddMetal(int value) {ResoursesData.ValueMetal = ResoursesData.ValueMetal + value;}
	UFUNCTION(BlueprintCallable, Category = "resources")
		void AddFood(int value)	 {ResoursesData.ValueFood = ResoursesData.ValueFood + value;}
	
	// Add Inventory Contents Ammo
	UFUNCTION(BlueprintCallable, Category = "Ammo")
		void AddWoodArrow(int value) { AmmoData.ValueWoodArrow = AmmoData.ValueWoodArrow+ value;}
	UFUNCTION(BlueprintCallable, Category = "Ammo")
		void AddRockArrow(int value) { AmmoData.ValueRockArrow = AmmoData.ValueRockArrow + value;}
	UFUNCTION(BlueprintCallable, Category = "Ammo")
		void AddMetalArrow(int value) { AmmoData.ValueMetalArrow = AmmoData.ValueMetalArrow + value;}


protected:	
	
	virtual void BeginPlay() override;

private:
	
	FResoursesData ResoursesData;
	FMaxResoursesData MaxResoursesData;
	FAmmoData AmmoData;
	FMaxAmmoData MaxAmmoData;

		
		
};
