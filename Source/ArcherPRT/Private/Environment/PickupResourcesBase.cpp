// Archer Prototype. All rights reserved


#include "Environment/PickupResourcesBase.h"
#include "Player/PlayerCharacter.h"

 void APickupResourcesBase::TryTakePickup_Server(APlayerCharacter* Pawn)
{
	 Super::TryTakePickup_Server(Pawn);
	 switch (ResourcesType)
	 {
	 case EResourcesType::Wood:  Pawn->InventoryComponent->AddResources(EResourcesType::Wood, Value);
		 break;
	 case EResourcesType::Rock:  Pawn->InventoryComponent->AddResources(EResourcesType::Rock, Value);
		 break;
	 case EResourcesType::Grass: Pawn->InventoryComponent->AddResources(EResourcesType::Grass, Value);
		 break;
	 case EResourcesType::Metal: Pawn->InventoryComponent->AddResources(EResourcesType::Metal, Value);
		 break;
	 case EResourcesType::Food: Pawn->InventoryComponent->AddResources(EResourcesType::Food, Value);
		 break;
	 default:
		 break;
	 }
	 TakePickup_Server();
}

