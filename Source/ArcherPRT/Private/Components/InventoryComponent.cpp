// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/InventoryComponent.h"
#include "Core/ArcherPRTData.h"
#include "Player/PlayerCharacter.h"
#include "GameFramework/Actor.h"
#include "Craft/RecipeBase.h"
#include "Math/UnrealMathUtility.h"


UInventoryComponent::UInventoryComponent()
{
}

void UInventoryComponent::BeginPlay()
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

		case EResourcesType::Arrow:

			if (Pawn->InventoryComponent->GetValueResources(EResourcesType::Arrow) >= ResourcesMap[KeysFromMap[i]])
			{
				SuccessPosition++;
			}

			if (SpendResources)
			{
				Pawn->InventoryComponent->AddResources(EResourcesType::Arrow, ResourcesMap[KeysFromMap[i]] * -1);
			}

			if (AddResources)
			{
				Pawn->InventoryComponent->AddResources(EResourcesType::Arrow, ResourcesMap[KeysFromMap[i]]);
			}

			break;

		case EResourcesType::Wood:

			if (Pawn->InventoryComponent->GetValueResources(EResourcesType::Wood) >= ResourcesMap[KeysFromMap[i]])
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

			if (Pawn->InventoryComponent->GetValueResources(EResourcesType::Rock) >= ResourcesMap[KeysFromMap[i]])
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

			if (Pawn->InventoryComponent->GetValueResources(EResourcesType::Grass) >= ResourcesMap[KeysFromMap[i]])
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

			if (Pawn->InventoryComponent->GetValueResources(EResourcesType::Metal) >= ResourcesMap[KeysFromMap[i]])
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

			if (Pawn->InventoryComponent->GetValueResources(EResourcesType::Food) >= ResourcesMap[KeysFromMap[i]])
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

		case EResourcesType::Battery:

			if (Pawn->InventoryComponent->GetValueResources(EResourcesType::Battery) >= ResourcesMap[KeysFromMap[i]])
			{
				SuccessPosition++;
			}

			if (SpendResources)
			{
				Pawn->InventoryComponent->AddResources(EResourcesType::Battery, ResourcesMap[KeysFromMap[i]] * -1);
			}

			if (AddResources)
			{
				Pawn->InventoryComponent->AddResources(EResourcesType::Battery, ResourcesMap[KeysFromMap[i]]);
			}

			break;

		case EResourcesType::Lamp:

			if (Pawn->InventoryComponent->GetValueResources(EResourcesType::Lamp) >= ResourcesMap[KeysFromMap[i]])
			{
				SuccessPosition++;
			}

			if (SpendResources)
			{
				Pawn->InventoryComponent->AddResources(EResourcesType::Lamp, ResourcesMap[KeysFromMap[i]] * -1);
			}

			if (AddResources)
			{
				Pawn->InventoryComponent->AddResources(EResourcesType::Lamp, ResourcesMap[KeysFromMap[i]]);
			}

			break;


		case EResourcesType::Mine:

			if (Pawn->InventoryComponent->GetValueResources(EResourcesType::Mine) >= ResourcesMap[KeysFromMap[i]])
			{
				SuccessPosition++;
			}

			if (SpendResources)
			{
				Pawn->InventoryComponent->AddResources(EResourcesType::Mine, ResourcesMap[KeysFromMap[i]] * -1);
			}

			if (AddResources)
			{
				Pawn->InventoryComponent->AddResources(EResourcesType::Mine, ResourcesMap[KeysFromMap[i]]);
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

int UInventoryComponent::GetValueResources(EResourcesType ResourcesType)
{
	switch (ResourcesType)
	{
	case EResourcesType::None:
			return 0;
		break;
	case EResourcesType::Arrow:
		return ResourcesData.ValueArrow;
		break;
	case EResourcesType::Wood:
			return ResourcesData.ValueWood;
		break;
	case EResourcesType::Rock:
			return ResourcesData.ValueRock;
		break;
	case EResourcesType::Grass:
			return ResourcesData.ValueGrass;
		break;
	case EResourcesType::Metal:
			return ResourcesData.ValueMetal;
		break;
	case EResourcesType::Food:
			return ResourcesData.ValueFood;
		break;
	case EResourcesType::Battery:
		return ResourcesData.ValueBattery;
		break;
	case EResourcesType::Lamp:
		return ResourcesData.ValueLamp;
		break;
	case EResourcesType::Mine:
		return ResourcesData.ValueMine;
		break;
	default:
			return 0;
		break;
	}
}

int UInventoryComponent::GetMaxResources(EResourcesType ResourcesType)
{
	switch (ResourcesType)
	{
	case EResourcesType::None:
		return 0;
		break;
	case EResourcesType::Arrow:
		return MaxResourcesData.MaxArrow;
		break;
	case EResourcesType::Wood:
		return MaxResourcesData.MaxWood;
		break;
	case EResourcesType::Rock:
		return MaxResourcesData.MaxRock;
		break;
	case EResourcesType::Grass:
		return MaxResourcesData.MaxGrass;
		break;
	case EResourcesType::Metal:
		return MaxResourcesData.MaxMetal;
		break;
	case EResourcesType::Food:
		return MaxResourcesData.MaxFood;
		break;
	case EResourcesType::Battery:
		return MaxResourcesData.MaxBattery;
		break;
	case EResourcesType::Lamp:
		return MaxResourcesData.MaxLamp;
		break;
	case EResourcesType::Mine:
		return MaxResourcesData.MaxMine;
		break;
	default:
		return 0;
		break;
	}
}

void UInventoryComponent::SetMaxResources(EResourcesType ResourcesType, int Value)
{
	switch (ResourcesType)
	{
	case EResourcesType::None:
		break;
	case EResourcesType::Arrow:
		MaxResourcesData.MaxArrow = Value;
		break;
	case EResourcesType::Wood:
		MaxResourcesData.MaxWood = Value;
		break;
	case EResourcesType::Rock:
		MaxResourcesData.MaxRock = Value;
		break;
	case EResourcesType::Grass:
		MaxResourcesData.MaxGrass = Value;
		break;
	case EResourcesType::Metal:
		MaxResourcesData.MaxMetal = Value;
		break;
	case EResourcesType::Food:
		MaxResourcesData.MaxFood = Value;
		break;
	case EResourcesType::Battery:
		MaxResourcesData.MaxBattery = Value;
		break;
	case EResourcesType::Lamp:
		MaxResourcesData.MaxLamp = Value;
		break;
	case EResourcesType::Mine:
		MaxResourcesData.MaxMine = Value;
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
	case EResourcesType::Arrow:
		ResourcesData.ValueArrow = FMath::Clamp(ResourcesData.ValueArrow + Value, 0, MaxResourcesData.MaxArrow);
		break;
	case EResourcesType::Wood:
			ResourcesData.ValueWood = FMath::Clamp(ResourcesData.ValueWood + Value, 0, MaxResourcesData.MaxWood);
		break;
	case EResourcesType::Rock:
			ResourcesData.ValueRock = FMath::Clamp(ResourcesData.ValueRock + Value, 0, MaxResourcesData.MaxRock);
		break;
	case EResourcesType::Grass:
			ResourcesData.ValueGrass = FMath::Clamp(ResourcesData.ValueGrass + Value, 0, MaxResourcesData.MaxGrass);
		break;
	case EResourcesType::Metal:
			ResourcesData.ValueMetal = FMath::Clamp(ResourcesData.ValueMetal + Value, 0, MaxResourcesData.MaxMetal);
		break;
	case EResourcesType::Food:
			ResourcesData.ValueFood = FMath::Clamp(ResourcesData.ValueFood + Value, 0, MaxResourcesData.MaxFood);
		break;
	case EResourcesType::Battery:
		ResourcesData.ValueBattery = FMath::Clamp(ResourcesData.ValueBattery + Value, 0, MaxResourcesData.MaxBattery);
		break;
	case EResourcesType::Lamp:
		ResourcesData.ValueLamp = FMath::Clamp(ResourcesData.ValueLamp + Value, 0, MaxResourcesData.MaxLamp);
		break;
	case EResourcesType::Mine:
		ResourcesData.ValueMine = FMath::Clamp(ResourcesData.ValueMine + Value, 0, MaxResourcesData.MaxMine);
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
	case EResourcesType::Arrow:
		return ResourcesData.ValueArrow < MaxResourcesData.MaxArrow;
		break;
	case EResourcesType::Wood:
		return ResourcesData.ValueWood < MaxResourcesData.MaxWood;
		break;
	case EResourcesType::Rock:
		return ResourcesData.ValueRock < MaxResourcesData.MaxRock;
		break;
	case EResourcesType::Grass:
		return ResourcesData.ValueGrass < MaxResourcesData.MaxGrass;
		break;
	case EResourcesType::Metal:
		return ResourcesData.ValueMetal < MaxResourcesData.MaxMetal;
		break;
	case EResourcesType::Food:
		return ResourcesData.ValueFood < MaxResourcesData.MaxFood;
		break;
	case EResourcesType::Battery:
		return ResourcesData.ValueBattery < MaxResourcesData.MaxBattery;
		break;
	case EResourcesType::Lamp:
		return ResourcesData.ValueLamp < MaxResourcesData.MaxLamp;
		break;
	case EResourcesType::Mine:
		return ResourcesData.ValueMine < MaxResourcesData.MaxMine;
		break;
	default:
		return false;
		break;
	}
}






