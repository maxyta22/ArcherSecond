// Archer Prototype. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "CheckSpeedService.generated.h"

UCLASS()
class ARCHERPRT_API UCheckSpeedService : public UBTService
{
	GENERATED_BODY()

public :
	UCheckSpeedService();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
		FBlackboardKeySelector EnemyActorKey;

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
};
