// Archer Prototype. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "SetPatrolPointTask.generated.h"


UCLASS()
class ARCHERPRT_API USetPatrolPointTask : public UBTTaskNode
{
	GENERATED_BODY()

public:

	USetPatrolPointTask();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
		FBlackboardKeySelector TargetLocationKey;
	
};
