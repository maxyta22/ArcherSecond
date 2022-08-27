// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/CraftComponent.h"
#include "Components/WeaponComponent.h"
#include "Craft/RecipeBase.h"
#include "Core/ArcherPRTData.h"
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

	const TSubclassOf<URecipeBase>  CurrentRecipe = RecipeDataBase[SelectedIndex];
	const EAmmoType Result = CurrentRecipe.GetDefaultObject()->Ammo;
	const TMap<EResourcesType, int> NeededResources = CurrentRecipe.GetDefaultObject()->RecipeMap;
	
	if (Pawn->InventoryComponent->CheckCanTakeAmmo(Result)
		&& Pawn->InventoryComponent->LoopOnResourcesByMap(NeededResources)
		&& !Pawn->WeaponComponent->AimingInProgress() 
		&& !CraftInProgress())
	{
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Green, TEXT("Start Craft"));
		GetWorld()->GetTimerManager().SetTimer(CraftInProgressTimer, this, &UCraftComponent::CraftSucceess, TimeCraft, false);
		Pawn->AfterBeginCraft();
	}
}

void UCraftComponent::AbortCraftProcess()
{
	GetWorld()->GetTimerManager().ClearTimer(CraftInProgressTimer);
}

void UCraftComponent::CraftSucceess()
{
	const auto Pawn = Cast<APlayerCharacter>(GetOwner());

	if (!Pawn) return;

	GetWorld()->GetTimerManager().ClearTimer(CraftInProgressTimer);
	Pawn->InventoryComponent->LoopOnResourcesByMap(RecipeDataBase[SelectedIndex].GetDefaultObject()->RecipeMap, true, false);
	GetRecipeResult(RecipeDataBase[SelectedIndex]);
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Green, TEXT("CraftSuccess"));
}

float UCraftComponent::GetCraftTimeRemaining()
{
	return GetWorld()->GetTimerManager().GetTimerRemaining(CraftInProgressTimer);
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



