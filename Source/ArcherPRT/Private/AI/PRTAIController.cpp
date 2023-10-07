// Archer Prototype. All rights reserved


#include "AI/PRTAIController.h"
#include "AI/AICharacter.h"
#include "Player/GameCharacter.h"
#include "AI/AICharacter.h"
#include "Core/ArcherPRTData.h"
#include "Components/PRTAIPerceptionComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BrainComponent.h"

APRTAIController::APRTAIController() 
{
	PRTAIPerceptionComponent = CreateDefaultSubobject<UPRTAIPerceptionComponent>("PRTAIPerceptionComponent");
	SetPerceptionComponent(*PRTAIPerceptionComponent);
}

void APRTAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);	
}

void APRTAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	const auto ControlledChar = Cast<AAICharacter>(InPawn);
	if (ControlledChar)
	{
		RunBehaviorTree(ControlledChar->BehaviorTreeAsset);
		ControlledPawn = ControlledChar;
		const EAIStartCondition Start = ControlledChar->StartCondition;
		GetBlackboardComponent()->SetValueAsEnum("StartCondition", StaticCast<uint8>(ControlledChar->StartCondition));
	}
}

void APRTAIController::LockBehavior(bool Lock)
{
	
	if (BrainComponent)
	{
		if (Lock == true)
		{
			GetBlackboardComponent()->SetValueAsBool("LockBehavior", true);
			BrainComponent->StopLogic("Stop");

		}
		else
			GetBlackboardComponent()->SetValueAsBool("LockBehavior", false);
			BrainComponent->StartLogic();
	}
}

void APRTAIController::SetEnemy(AActor* Enemy)
{
	GetBlackboardComponent()->SetValueAsObject("EnemyActor", Enemy);
}

AActor* APRTAIController::GetEnemy()
{
	return Cast<AActor>(GetBlackboardComponent()->GetValueAsObject("EnemyActor"));
}

AActor* APRTAIController::GetFocusOnActor() const
{
	if (!GetBlackboardComponent()) return nullptr;
	return Cast<AActor>(GetBlackboardComponent()->GetValueAsObject(FocusOnKeyName));
}

