// Archer Prototype. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "SetWalkSpeedTask.generated.h"

UCLASS()
class ARCHERPRT_API USetWalkSpeedTask : public UBTTaskNode
{
	GENERATED_BODY()

public:

	USetWalkSpeedTask();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
		float Speed = 600;
	
};
