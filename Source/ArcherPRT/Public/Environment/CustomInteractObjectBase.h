// Archer Prototype. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Environment/InteractObjectBase.h"
#include "Core/ArcherPRTData.h"
#include "CustomInteractObjectBase.generated.h"


UCLASS()
class ARCHERPRT_API ACustomInteractObjectBase : public AInteractObjectBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TMap<EResourcesType, int> ResoursesForInteract;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool ReturnResources;
	

protected:

	virtual void TryUseInteractObject(APlayerCharacter* Pawn) override;

private:

	bool Activate;
	
};
