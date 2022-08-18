// Archer Prototype. All rights reserved


#include "Environment/CustomInteractObjectBase.h"
#include "Core/ArcherPRTData.h"
#include "Components/InventoryComponent.h"
#include "Player/PlayerCharacter.h"

void ACustomInteractObjectBase::TryUseInteractObject(APlayerCharacter* Pawn)
{
	Super::TryUseInteractObject(Pawn);

	if (!Pawn) return;

	if (ResoursesForInteract.Num()>0)
	{
		if (!Activate)
		{
			if (Pawn->InventoryComponent->LoopOnResourcesByMap(ResoursesForInteract))
			{
				Pawn->InventoryComponent->LoopOnResourcesByMap(ResoursesForInteract, true);
				Activate = true;
				AfterWasSuccessUseInteractObject();
			}
		}
		else
		{
			if (ReturnResources)
			{
				Pawn->InventoryComponent->LoopOnResourcesByMap(ResoursesForInteract, false, true);
			}
			Activate = false;
			AfterWasSuccessUseInteractObject();
		}

	}

	else 
	{
		AfterWasSuccessUseInteractObject();
	}
	
}
