// Archer Prototype. All rights reserved


#include "AI/Services/FindEnemyService.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "AI/AICharacter.h"
#include "AI/PRTAIController.h"
#include "Components/PRTAIPerceptionComponent.h"

UFindEnemyService::UFindEnemyService()
{
	NodeName = "Find Enemy";
}


void UFindEnemyService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{

	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	const auto Blackboard = OwnerComp.GetBlackboardComponent();

	if (Blackboard)
	{
		const auto OwnerAIController = Cast<APRTAIController>(OwnerComp.GetAIOwner());
		if (!OwnerAIController) return;
		const auto OwnerAICharacter = Cast<AAICharacter>(OwnerAIController->GetCharacter());
		if (!OwnerAICharacter) return;
		const auto PerceptionComponent = OwnerAIController->FindComponentByClass<UPRTAIPerceptionComponent>();
		if (!PerceptionComponent) return;


		if (!OwnerAIController->GetEnemy()) 
		{
			OwnerAIController->SetEnemy(PerceptionComponent->GetNearestEnemy());
			if (OwnerAIController->GetEnemy())
			{
				OwnerAICharacter->AfterEnemyFound();
			}			
		}
	}
	
	
}
