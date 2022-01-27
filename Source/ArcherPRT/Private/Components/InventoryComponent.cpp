// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/InventoryComponent.h"
#include "Enum/ResourcesTypeEnum.h"
#include "Enum/AmmoTypeEnum.h"


UInventoryComponent::UInventoryComponent()
{
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
}

int UInventoryComponent::GetValueResourses(EResourcesType ResourcesType)
{
	switch (ResourcesType)
	{
	case EResourcesType::None:
			return 0;
		break;
	case EResourcesType::Wood:
			return ResoursesData.ValueWood;
		break;
	case EResourcesType::Rock:
			return ResoursesData.ValueRock;
		break;
	case EResourcesType::Grass:
			return ResoursesData.ValueGrass;
		break;
	case EResourcesType::Metal:
			return ResoursesData.ValueMetal;
		break;
	case EResourcesType::Food:
			return ResoursesData.ValueFood;
		break;
	default:
			return 0;
		break;
	}
}

void UInventoryComponent::AddResources(EResourcesType ResourcesType, int Value)
{
	switch (ResourcesType)
	{
	case EResourcesType::None:
			
		break;
	case EResourcesType::Wood:
			ResoursesData.ValueWood = ResoursesData.ValueWood + Value;
		break;
	case EResourcesType::Rock:
			ResoursesData.ValueRock = ResoursesData.ValueRock + Value;
		break;
	case EResourcesType::Grass:
			ResoursesData.ValueGrass = ResoursesData.ValueGrass + Value;
		break;
	case EResourcesType::Metal:
			ResoursesData.ValueMetal = ResoursesData.ValueMetal + Value;
		break;
	case EResourcesType::Food:
			ResoursesData.ValueFood = ResoursesData.ValueFood + Value;
		break;
	default:
			
		break;
	}
}

int UInventoryComponent::GetValueAmmo(EAmmoType AmmoType)
{
	switch (AmmoType)
	{
	case EAmmoType::None:
			return 0;
		break;
	case EAmmoType::WoodArrow:
			return AmmoData.ValueWoodArrow;
		break;
	case EAmmoType::RockArrow:
			return AmmoData.ValueRockArrow;
		break;
	case EAmmoType::MetalArrow:
			return AmmoData.ValueMetalArrow;
		break;
	default:
			return 0;
		break;
	}
}

int UInventoryComponent::GetMaxAmmo(EAmmoType AmmoType)
{
	switch (AmmoType)
	{
	case EAmmoType::None:
			return 0;
		break;
	case EAmmoType::WoodArrow:
			return MaxAmmoData.MaxWoodArrow;
		break;
	case EAmmoType::RockArrow:
			return MaxAmmoData.MaxRockArrow;
		break;
	case EAmmoType::MetalArrow:
			return MaxAmmoData.MaxMetalArrow;
		break;
	default:
			return 0;
		break;
	}
}

void UInventoryComponent::SetMaxAmmo(EAmmoType AmmoType, int Value)
{
	switch (AmmoType)
	{
	case EAmmoType::None:
		break;
	case EAmmoType::WoodArrow:
		MaxAmmoData.MaxWoodArrow = Value;
		break;
	case EAmmoType::RockArrow:
		MaxAmmoData.MaxRockArrow = Value;
		break;
	case EAmmoType::MetalArrow:
		MaxAmmoData.MaxMetalArrow = Value;
		break;
	default:
		break;
	}
}

void UInventoryComponent::AddAmmo(EAmmoType AmmoType, int Value)
{
	switch (AmmoType)
	{
	case EAmmoType::None:
		break;
	case EAmmoType::WoodArrow:
		AmmoData.ValueWoodArrow = AmmoData.ValueWoodArrow + Value;;
		break;
	case EAmmoType::RockArrow:
		AmmoData.ValueRockArrow = AmmoData.ValueRockArrow + Value;
		break;
	case EAmmoType::MetalArrow:
		AmmoData.ValueMetalArrow = AmmoData.ValueMetalArrow + Value;
		break;
	default:
		break;
	}
}





