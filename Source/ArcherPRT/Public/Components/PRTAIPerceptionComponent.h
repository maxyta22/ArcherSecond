// Archer Prototype. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Perception/AIPerceptionComponent.h"
#include "PRTAIPerceptionComponent.generated.h"


UCLASS()
class ARCHERPRT_API UPRTAIPerceptionComponent : public UAIPerceptionComponent
{
	GENERATED_BODY()

public:
	AActor* GetNearestEnemy() const;
	
};
