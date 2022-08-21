// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/InventoryComponent.h"
#include "Core/ArcherPRTData.h"
#include "Player/PlayerCharacter.h"
#include "Craft/RecipeBase.h"


UInventoryComponent::UInventoryComponent()
{
}

bool UInventoryComponent::LoopOnResourcesByMap(TMap<EResourcesType, int> ResourcesMap, bool SpendResources, bool AddResources)
{
	const auto Pawn = Cast<APlayerCharacter>(GetOwner());

	TArray<EResourcesType> KeysFromMap;
	ResourcesMap.GetKeys(KeysFromMap);

	TArray<int> ValueFromMap;
	ResourcesMap.GenerateValueArray(ValueFromMap);

	int SuccessPosition = 0;

	for (int i = 0; i < ResourcesMap.Num(); i++)
	{
		switch (KeysFromMap[i])
		{
		case EResourcesType::Wood:

			if (Pawn->InventoryComponent->GetValueResourses(EResourcesType::Wood) >= ResourcesMap[KeysFromMap[i]])
			{
				SuccessPosition++;
			}

			if (SpendResources)
			{
				Pawn->InventoryComponent->AddResources(EResourcesType::Wood, ResourcesMap[KeysFromMap[i]] * -1);
			}

			if (AddResources)
			{
				Pawn->InventoryComponent->AddResources(EResourcesType::Wood, ResourcesMap[KeysFromMap[i]]);
			}

			break;

		case EResourcesType::Rock:

			if (Pawn->InventoryComponent->GetValueResourses(EResourcesType::Rock) >= ResourcesMap[KeysFromMap[i]])
			{
				SuccessPosition++;
			}

			if (SpendResources)
			{
				Pawn->InventoryComponent->AddResources(EResourcesType::Rock, ResourcesMap[KeysFromMap[i]] * -1);
			}

			if (AddResources)
			{
				Pawn->InventoryComponent->AddResources(EResourcesType::Rock, ResourcesMap[KeysFromMap[i]]);
			}

			break;

		case EResourcesType::Grass:

			if (Pawn->InventoryComponent->GetValueResourses(EResourcesType::Grass) >= ResourcesMap[KeysFromMap[i]])
			{
				SuccessPosition++;
			}

			if (SpendResources)
			{
				Pawn->InventoryComponent->AddResources(EResourcesType::Grass, ResourcesMap[KeysFromMap[i]] * -1);
			}

			if (AddResources)
			{
				Pawn->InventoryComponent->AddResources(EResourcesType::Grass, ResourcesMap[KeysFromMap[i]]);
			}

			break;

		case EResourcesType::Metal:

			if (Pawn->InventoryComponent->GetValueResourses(EResourcesType::Metal) >= ResourcesMap[KeysFromMap[i]])
			{
				SuccessPosition++;
			}

			if (SpendResources)
			{
				Pawn->InventoryComponent->AddResources(EResourcesType::Metal, ResourcesMap[KeysFromMap[i]] * -1);
			}

			if (AddResources)
			{
				Pawn->InventoryComponent->AddResources(EResourcesType::Metal, ResourcesMap[KeysFromMap[i]]);
			}

			break;

		case EResourcesType::Food:

			if (Pawn->InventoryComponent->GetValueResourses(EResourcesType::Food) >= ResourcesMap[KeysFromMap[i]])
			{
				SuccessPosition++;
			}

			if (SpendResources)
			{
				Pawn->InventoryComponent->AddResources(EResourcesType::Food, ResourcesMap[KeysFromMap[i]] * -1);
			}

			if (AddResources)
			{
				Pawn->InventoryComponent->AddResources(EResourcesType::Food, ResourcesMap[KeysFromMap[i]]);
			}

			break;
		}
	}

	if (SuccessPosition == ResourcesMap.Num())
	{
		return true;
	}

	return false;
}

void UInventoryComponent::BeginPlay()
{
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

int UInventoryComponent::GetMaxResourses(EResourcesType ResourcesType)
{
	switch (ResourcesType)
	{
	case EResourcesType::None:
		return 0;
		break;
	case EResourcesType::Wood:
		return MaxResoursesData.MaxWood;
		break;
	case EResourcesType::Rock:
		return MaxResoursesData.MaxRock;
		break;
	case EResourcesType::Grass:
		return MaxResoursesData.MaxGrass;
		break;
	case EResourcesType::Metal:
		return MaxResoursesData.MaxMetal;
		break;
	case EResourcesType::Food:
		return MaxResoursesData.MaxFood;
		break;
	default:
		return 0;
		break;
	}
}

void UInventoryComponent::SetMaxResourses(EResourcesType ResourcesType, int Value)
{
	switch (ResourcesType)
	{
	case EResourcesType::None:
		break;
	case EResourcesType::Wood:
		MaxResoursesData.MaxWood = Value;
		break;
	case EResourcesType::Rock:
		MaxResoursesData.MaxRock = Value;
		break;
	case EResourcesType::Grass:
		MaxResoursesData.MaxGrass = Value;
		break;
	case EResourcesType::Metal:
		MaxResoursesData.MaxMetal = Value;
		break;
	case EResourcesType::Food:
		MaxResoursesData.MaxFood = Value;
		break;
	default:
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

void UInventoryComponent::SetAmmo(EAmmoType AmmoType, int Value)
{
	switch (AmmoType)
	{
	case EAmmoType::None:
		break;
	case EAmmoType::WoodArrow:
		AmmoData.ValueWoodArrow = Value;;
		break;
	case EAmmoType::RockArrow:
		AmmoData.ValueRockArrow = Value;
		break;
	case EAmmoType::MetalArrow:
		AmmoData.ValueMetalArrow = Value;
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

bool UInventoryComponent::CheckCanTakeResources(EResourcesType ResourcesType)
{
	switch (ResourcesType)
	{
	case EResourcesType::None: 
		return false;
		break;
	case EResourcesType::Wood:
		return ResoursesData.ValueWood < MaxResoursesData.MaxWood;
		break;
	case EResourcesType::Rock:
		return ResoursesData.ValueRock < MaxResoursesData.MaxRock;
		break;
	case EResourcesType::Grass:
		return ResoursesData.ValueGrass < MaxResoursesData.MaxGrass;
		break;
	case EResourcesType::Metal:
		return ResoursesData.ValueMetal < MaxResoursesData.MaxMetal;
		break;
	case EResourcesType::Food:
		return ResoursesData.ValueFood < MaxResoursesData.MaxFood;
		break;
	default:
		return false;
		break;
	}
}

bool UInventoryComponent::CheckCanTakeAmmo(EAmmoType AmmoType)
{
	switch (AmmoType)
	{
	case EAmmoType::None:
		return false;
		break;
	case EAmmoType::WoodArrow:
		return AmmoData.ValueWoodArrow < MaxAmmoData.MaxWoodArrow;
		break;
	case EAmmoType::RockArrow:
		return AmmoData.ValueRockArrow < MaxAmmoData.MaxRockArrow;
		break;
	case EAmmoType::MetalArrow:
		return AmmoData.ValueMetalArrow < MaxAmmoData.MaxMetalArrow;
		break;
	default:
		return false;
		break;
	}
}





