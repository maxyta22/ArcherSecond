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
	const EResourcesType Result = CurrentRecipe.GetDefaultObject()->Resources;
	const TMap<EResourcesType, int> NeededResources = CurrentRecipe.GetDefaultObject()->RecipeMap;
	
	if (Pawn->InventoryComponent->CheckCanTakeResources(Result)
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

	const auto Ammo = Recipe.GetDefaultObject()->Resources;

	switch (Ammo)
	{
	case EResourcesType::Arrow:

		Pawn->InventoryComponent->AddResources(EResourcesType::Arrow, 3);
		break;
	}
}



