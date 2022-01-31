// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/CraftComponent.h"
#include "Craft/RecipeBase.h"
#include "Enum/AmmoTypeEnum.h"
#include "Enum/ResourcesTypeEnum.h"
#include "Craft/RecipeBase.h"
#include "Player/PlayerCharacter.h"


UCraftComponent::UCraftComponent()
{
}

void UCraftComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UCraftComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
}

void UCraftComponent::TryCraftItem()
{
	const auto Pawn = Cast<APlayerCharacter>(GetOwner());

	if (!Pawn) return;
	if (!RecipeDataBase[SelectedIndex]) return;

	TSubclassOf<URecipeBase>  CurrentRecipe = RecipeDataBase[SelectedIndex];
	EAmmoType Result = CurrentRecipe.GetDefaultObject()->Ammo;
	
	if (Pawn->InventoryComponent->CheckCanTakeAmmo(Result)&& CheckRecipe(RecipeDataBase[SelectedIndex], false))
	{
		GetWorld()->GetTimerManager().SetTimer(CraftInProgressTimer, this, &UCraftComponent::CraftSucceess, TimeCraft, false);
	}
}

void UCraftComponent::AbortCraftProcess()
{
	GetWorld()->GetTimerManager().ClearTimer(CraftInProgressTimer);
}

void UCraftComponent::CraftSucceess()
{
	GetWorld()->GetTimerManager().ClearTimer(CraftInProgressTimer);
	CheckRecipe(RecipeDataBase[SelectedIndex], true);
	GetRecipeResult(RecipeDataBase[SelectedIndex]);
	//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, TEXT("CraftSuccess"));
}

float UCraftComponent::GetCraftTimeRemaining()
{
	return GetWorld()->GetTimerManager().GetTimerRemaining(CraftInProgressTimer);
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



