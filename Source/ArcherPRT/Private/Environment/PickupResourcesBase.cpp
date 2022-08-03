// Archer Prototype. All rights reserved


#include "Environment/PickupResourcesBase.h"
#include "Player/PlayerCharacter.h"

void APickupResourcesBase::TryUseInteractObject(APlayerCharacter* Pawn)
{
	Super::TryUseInteractObject(Pawn);

	if (Pawn->InventoryComponent->CheckCanTakeResources(ResourcesType))
	{
		Pawn->InventoryComponent->AddResources(ResourcesType, Value);
		TakePickup();
	}
}

void APickupResourcesBase::TakePickup()
{
	if (bInfinity) return;
	Destroy();
}

