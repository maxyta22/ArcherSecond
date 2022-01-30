// Archer Prototype. All rights reserved


#include "Environment/PickupResourcesBase.h"
#include "Player/PlayerCharacter.h"

void APickupResourcesBase::TryTakePickup_Server(APlayerCharacter* Pawn)
{
	Super::TryTakePickup_Server(Pawn);

	if (Pawn->InventoryComponent->CheckCanTakeResources(ResourcesType))
	{
		Pawn->InventoryComponent->AddResources(ResourcesType, Value);
		TakePickup_Server();
	}
}

