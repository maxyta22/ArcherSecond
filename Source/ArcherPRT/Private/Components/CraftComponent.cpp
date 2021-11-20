// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/CraftComponent.h"
#include "Craft/RecipeBase.h"
#include "Enum/ResourcesTypeEnum.h"
#include "Player/PlayerCharacter.h"


UCraftComponent::UCraftComponent()
{
}

void UCraftComponent::TryCraftItem()
{
	if (CheckRecipe(RecipeDataBase[SelectedIndex], false))
	{
		CheckRecipe(RecipeDataBase[SelectedIndex], true);
		GetRecipeResult(RecipeDataBase[SelectedIndex]);

		//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, TEXT("CraftSuccess"));
	}
	
}

void UCraftComponent::BeginPlay()
{
	Super::BeginPlay();	
}

bool UCraftComponent::CheckRecipe(TSubclassOf<URecipeBase> Recipe, bool SpendResources)
{	
	const auto Pawn = Cast<APlayerCharacter>(GetOwner());

	const auto CheckRecipeMap = Recipe.GetDefaultObject()->RecipeMap;

	TArray<EResourcesType> KeysFromMap;
	CheckRecipeMap.GetKeys(KeysFromMap);

	TArray<int> ValueFromMap;
	CheckRecipeMap.GenerateValueArray(ValueFromMap);

	int SuccessPosition = 0;

	//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, TEXT("CheckResourcesLoopIteration"));

	for (int  i = 0; i < CheckRecipeMap.Num(); i++)	
	{
		
		switch (KeysFromMap[i])
		{

			case EResourcesType::Wood:

			if (Pawn->InventoryComponent->GetWood() >= CheckRecipeMap[KeysFromMap[i]])
			{
					SuccessPosition++;
					if (SpendResources)
					{
						Pawn->InventoryComponent->AddWood(CheckRecipeMap[KeysFromMap[i]] * -1);
					}
			}
			break;
					
			case EResourcesType::Rock:

			if (Pawn->InventoryComponent->GetRock() >= CheckRecipeMap[KeysFromMap[i]])
			{
					SuccessPosition++;
					if (SpendResources)
					{
						Pawn->InventoryComponent->AddRock(CheckRecipeMap[KeysFromMap[i]] * -1);
					}
			}
			break;

			case EResourcesType::Grass:

			if (Pawn->InventoryComponent->GetGrass() >= CheckRecipeMap[KeysFromMap[i]])
			{
					SuccessPosition++;
					if (SpendResources)
					{
						Pawn->InventoryComponent->AddGrass(CheckRecipeMap[KeysFromMap[i]] * -1);
					}
			}
			break;

			case EResourcesType::Metal:

			if (Pawn->InventoryComponent->GetMetal() >= CheckRecipeMap[KeysFromMap[i]])
			{
					SuccessPosition++;
					if (SpendResources)
					{
						Pawn->InventoryComponent->AddMetal(CheckRecipeMap[KeysFromMap[i]] * -1);
					}
			}
				break;
			case EResourcesType::Food:

			if (Pawn->InventoryComponent->GetFood() >= CheckRecipeMap[KeysFromMap[i]])
			{
					SuccessPosition++;
					if (SpendResources)
					{
						Pawn->InventoryComponent->AddFood(CheckRecipeMap[KeysFromMap[i]] * -1);
					}
			}
				break;
			
			default:

				break;
			}	
	}

	if (SuccessPosition == CheckRecipeMap.Num())
	{
		return true;
	}

	return false;
}

void UCraftComponent::GetRecipeResult(TSubclassOf<URecipeBase> Recipe)
{
	const auto Pawn = Cast<APlayerCharacter>(GetOwner());

	const auto Ammo = Recipe.GetDefaultObject()->Ammo;

	switch (Ammo)
	{
	case EAmmoType::WoodArrow:

		Pawn->InventoryComponent->AddWoodArrow(1);

		break;

	case EAmmoType::RockArrow:

		Pawn->InventoryComponent->AddRockArrow(1);

		break;

	case EAmmoType::MetalArrow:

		Pawn->InventoryComponent->AddMetalArrow(1);

		break;

	default:
		break;
	}
}



