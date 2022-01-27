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

			if (Pawn->InventoryComponent->GetValueResourses(EResourcesType::Wood) >= CheckRecipeMap[KeysFromMap[i]])
			{
					SuccessPosition++;
					if (SpendResources)
					{
						Pawn->InventoryComponent->AddResources(EResourcesType::Wood, CheckRecipeMap[KeysFromMap[i]] * -1);
					}
			}
			break;
					
			case EResourcesType::Rock:

			if (Pawn->InventoryComponent->GetValueResourses(EResourcesType::Rock) >= CheckRecipeMap[KeysFromMap[i]])
			{
					SuccessPosition++;
					if (SpendResources)
					{
						Pawn->InventoryComponent->AddResources(EResourcesType::Rock, CheckRecipeMap[KeysFromMap[i]] * -1);
					}
			}
			break;

			case EResourcesType::Grass:

			if (Pawn->InventoryComponent->GetValueResourses(EResourcesType::Grass) >= CheckRecipeMap[KeysFromMap[i]])
			{
					SuccessPosition++;
					if (SpendResources)
					{
						Pawn->InventoryComponent->AddResources(EResourcesType::Grass, CheckRecipeMap[KeysFromMap[i]] * -1);
					}
			}
			break;

			case EResourcesType::Metal:

			if (Pawn->InventoryComponent->GetValueResourses(EResourcesType::Metal) >= CheckRecipeMap[KeysFromMap[i]])
			{
					SuccessPosition++;
					if (SpendResources)
					{
						Pawn->InventoryComponent->AddResources(EResourcesType::Metal, CheckRecipeMap[KeysFromMap[i]] * -1);
					}
			}
				break;
			case EResourcesType::Food:

			if (Pawn->InventoryComponent->GetValueResourses(EResourcesType::Food) >= CheckRecipeMap[KeysFromMap[i]])
			{
					SuccessPosition++;
					if (SpendResources)
					{
						Pawn->InventoryComponent->AddResources(EResourcesType::Food, CheckRecipeMap[KeysFromMap[i]] * -1);
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

		Pawn->InventoryComponent->AddAmmo(EAmmoType::WoodArrow, 1);

		break;

	case EAmmoType::RockArrow:

		Pawn->InventoryComponent->AddAmmo(EAmmoType::RockArrow, 1);

		break;

	case EAmmoType::MetalArrow:

		Pawn->InventoryComponent->AddAmmo(EAmmoType::MetalArrow, 1);

		break;

	default:
		break;
	}
}



