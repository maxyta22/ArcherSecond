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

bool UCraftComponent::TryCraftItem(TSubclassOf<URecipeBase> Recipe)
{
	const auto Pawn = Cast<APlayerCharacter>(GetOwner());

	if (Pawn == nullptr) return false;;
	if (Recipe == nullptr) return false;

	const EResourcesType Result = Recipe.GetDefaultObject()->Resources;
	const TMap<EResourcesType, int> NeededResources = Recipe.GetDefaultObject()->RecipeMap;
	
	if (Pawn->InventoryComponent->CheckCanTakeResources(Result)
		&& Pawn->InventoryComponent->LoopOnResourcesByMap(NeededResources)
		&& !Pawn->WeaponComponent->AimingInProgress() 
		&& !CraftInProgress())
	{

		// Use this code when craft in pause
		Pawn->InventoryComponent->LoopOnResourcesByMap(Recipe.GetDefaultObject()->RecipeMap, true, false);
		GetRecipeResult(Recipe);
		return true;

		//Use this when craft in gameplay time
		/* 
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Green, TEXT("Start Craft"));
		GetWorld()->GetTimerManager().SetTimer(CraftInProgressTimer, this, &UCraftComponent::CraftSucceess, TimeCraft, false);
		Pawn->AfterBeginCraft();
		*/
	}
	return false;
}

bool UCraftComponent::CheckCanTryCraftItem(TSubclassOf<URecipeBase> Recipe)
{
	const auto Pawn = Cast<APlayerCharacter>(GetOwner());

	if (Pawn == nullptr) return false;;
	if (Recipe == nullptr) return false;

	const EResourcesType Result = Recipe.GetDefaultObject()->Resources;
	const TMap<EResourcesType, int> NeededResources = Recipe.GetDefaultObject()->RecipeMap;

	return (Pawn->InventoryComponent->CheckCanTakeResources(Result)
		&& Pawn->InventoryComponent->LoopOnResourcesByMap(NeededResources)
		&& !Pawn->WeaponComponent->AimingInProgress()
		&& !CraftInProgress());
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

	//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Green, TEXT("CraftSuccess"));
}

float UCraftComponent::GetCraftTimeRemaining()
{
	return GetWorld()->GetTimerManager().GetTimerRemaining(CraftInProgressTimer);
}

void UCraftComponent::GetRecipeResult(TSubclassOf<URecipeBase> Recipe)
{
	if (Recipe == nullptr) return;

	const auto Pawn = Cast<APlayerCharacter>(GetOwner());

	const auto Ammo = Recipe.GetDefaultObject()->Resources;

	switch (Ammo)
	{
	case EResourcesType::Arrow:

		Pawn->InventoryComponent->AddResources(EResourcesType::Arrow, 3);
		break;
	}
}



