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



