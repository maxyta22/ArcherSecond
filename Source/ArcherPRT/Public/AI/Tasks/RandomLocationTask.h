// Archer Prototype. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "RandomLocationTask.generated.h"


UCLASS()
class ARCHERPRT_API URandomLocationTask : public UBTTaskNode
{
	GENERATED_BODY()

public:

	URandomLocationTask();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
		float Radius = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
		FBlackboardKeySelector TargetLocationKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
		bool SelfCenter = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (EditCondition = "!SelfCenter"))
		FBlackboardKeySelector CenterActorKey;


};
