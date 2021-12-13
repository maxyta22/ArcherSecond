// Archer Prototype. All rights reserved


#include "Environment/PickupResourcesBase.h"
#include "Player/PlayerCharacter.h"

 void APickupResourcesBase::TryTakePickup_Server(APlayerCharacter* Pawn)
{
	 Super::TryTakePickup_Server(Pawn);
	 switch (ResourcesType)
	 {
	 case EResourcesType::Wood:  Pawn->InventoryComponent->AddWood(Value);
		 break;
	 case EResourcesType::Rock:  Pawn->InventoryComponent->AddRock(Value);
		 break;
	 case EResourcesType::Grass: Pawn->InventoryComponent->AddGrass(Value);
		 break;
	 case EResourcesType::Metal: Pawn->InventoryComponent->AddMetal(Value);
		 break;
	 case EResourcesType::Food: Pawn->InventoryComponent->AddFood(Value);
		 break;
	 default:
		 break;
	 }
	 TakePickup_Server();
}

