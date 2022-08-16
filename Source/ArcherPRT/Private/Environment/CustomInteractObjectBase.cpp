// Archer Prototype. All rights reserved


#include "Environment/CustomInteractObjectBase.h"
#include "Core/ArcherPRTData.h"

void ACustomInteractObjectBase::TryUseInteractObject(APlayerCharacter* Pawn)
{
	Super::TryUseInteractObject(Pawn);

	if (ResoursesForInteract.Num()>0)
	{
		
	}
	else 
	{
		AfterWasSuccessUseInteractObject();
	}
	
}
