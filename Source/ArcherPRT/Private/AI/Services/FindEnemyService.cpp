// Archer Prototype. All rights reserved


#include "AI/Services/FindEnemyService.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Components/PRTAIPerceptionComponent.h"

UFindEnemyService::UFindEnemyService()
{
	NodeName = "Find Enemy";
}


void UFindEnemyService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	const auto Blackboard = OwnerComp.GetBlackboardComponent();
	if (Blackboard)
	{
		if (Blackboard->GetValueAsObject(EnemyActorKey.SelectedKeyName)) return;

		const auto Controller = OwnerComp.GetAIOwner();
		const auto PerceptionComponent = Controller->FindComponentByClass<UPRTAIPerceptionComponent>();
		if (PerceptionComponent)
		{
			Blackboard->SetValueAsObject(EnemyActorKey.SelectedKeyName, PerceptionComponent->GetNearestEnemy());
		}
	}
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}
