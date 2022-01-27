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



