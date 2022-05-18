// Archer Prototype. All rights reserved


#include "Environment/PickupResourcesBase.h"
#include "Player/PlayerCharacter.h"

void APickupResourcesBase::TryTakePickup(APlayerCharacter* Pawn)
{
	Super::TryTakePickup(Pawn);

	if (Pawn->InventoryComponent->CheckCanTakeResources(ResourcesType))
	{
		Pawn->InventoryComponent->AddResources(ResourcesType, Value);
		TakePickup();
	}
}

