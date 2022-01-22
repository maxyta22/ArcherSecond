// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
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
	UFUNCTION(BlueprintCallable, Category = "resources")
		int GetWood() const {return ResoursesData.ValueWood;}
	UFUNCTION(BlueprintCallable, Category = "resources")
		int GetRock() const {return ResoursesData.ValueRock;}
	UFUNCTION(BlueprintCallable, Category = "resources")
		int GetGrass() const {return ResoursesData.ValueGrass;}
	UFUNCTION(BlueprintCallable, Category = "resources")
		int GetMetal() const { return ResoursesData.ValueMetal; }
	UFUNCTION(BlueprintCallable, Category = "resources")
		int GetFood() const { return ResoursesData.ValueFood; }


	// Get Inventory Contents Ammo
	UFUNCTION(BlueprintCallable, Category = "Ammo")
		int GetWoodArrow() const { return AmmoData.ValueWoodArrow; }
	UFUNCTION(BlueprintCallable, Category = "Ammo")
		int GetRockArrow() const { return AmmoData.ValueRockArrow; }
	UFUNCTION(BlueprintCallable, Category = "Ammo")
		int GetMetalArrow() const { return AmmoData.ValueMetalArrow; }
	
	//Get Max Ammo
	UFUNCTION(BlueprintCallable, Category = "Ammo")
		int GetMaxWoodArrow() const { return MaxAmmoData.MaxWoodArrow; }
	UFUNCTION(BlueprintCallable, Category = "Ammo")
		int GetMaxRockArrow() const { return MaxAmmoData.MaxRockArrow; }
	UFUNCTION(BlueprintCallable, Category = "Ammo")
		int GetMaxMetalArrow() const { return MaxAmmoData.MaxMetalArrow; }

	// Add Inventory Contents resources
	void AddWood(int value)  {ResoursesData.ValueWood = ResoursesData.ValueWood + value;}
	void AddRock(int value)  {ResoursesData.ValueRock = ResoursesData.ValueRock + value;}
	void AddGrass(int value) {ResoursesData.ValueGrass = ResoursesData.ValueGrass + value;}
	void AddMetal(int value) { ResoursesData.ValueMetal = ResoursesData.ValueMetal + value;}
	void AddFood(int value) { ResoursesData.ValueFood = ResoursesData.ValueFood + value;}
	
	// Add Inventory Contents Ammo
	void AddWoodArrow(int value) { AmmoData.ValueWoodArrow = AmmoData.ValueWoodArrow+ value;}
	void AddRockArrow(int value) { AmmoData.ValueRockArrow = AmmoData.ValueRockArrow + value;}
	void AddMetalArrow(int value) { AmmoData.ValueMetalArrow = AmmoData.ValueMetalArrow + value;}


protected:	
	
	virtual void BeginPlay() override;

private:
	
	FResoursesData ResoursesData;
	FAmmoData AmmoData;
	FMaxAmmoData MaxAmmoData;

		
		
};
