// Archer Prototype. All rights reserved


#include "Environment/PickupResourcesBase.h"
#include "Player/PlayerCharacter.h"

void APickupResourcesBase::TryUseInteractObject(APlayerCharacter* Pawn)
{
	Super::TryUseInteractObject(Pawn);

	if (ResourcesList.Num() > 0)
	{
		TArray<EResourcesType> KeysFromMap;
		ResourcesList.GetKeys(KeysFromMap);

		TArray<int> ValueFromMap;
		ResourcesList.GenerateValueArray(ValueFromMap);

		for (int i = 0; i < ResourcesList.Num(); i++)
		{
			if (Pawn->InventoryComponent->CheckCanTakeResources(KeysFromMap[i]))
			{
				Pawn->InventoryComponent->AddResources(KeysFromMap[i], ValueFromMap[i]);
			}
			
		}
	}

	if (Pawn->InventoryComponent->CheckCanTakeResources(ResourcesType))
	{
		Pawn->InventoryComponent->AddResources(ResourcesType, Value);
	}

	TakePickup();
}

void APickupResourcesBase::TakePickup()
{
	if (bInfinity) return;
	if (Container)
	{
		Container->Destroy();
	}

	Destroy();	
}

