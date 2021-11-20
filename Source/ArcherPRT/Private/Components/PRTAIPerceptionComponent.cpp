// Archer Prototype. All rights reserved


#include "Components/PRTAIPerceptionComponent.h"
#include "AIController.h"
#include "GameFramework/Actor.h"
#include "Components/StatsComponent.h"
#include "Player/GameCharacter.h"
#include "Perception/AISense_Sight.h"

AActor* UPRTAIPerceptionComponent::GetNearestEnemy() const
{
	TArray<AActor*> PerceivedActors;
	GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(), PerceivedActors);
	if (PerceivedActors.Num() == 0) return nullptr;
	
	const auto Controller = Cast<AAIController>(GetOwner());
	if (!Controller) return nullptr;
	
	const auto Pawn = Controller->GetPawn();
	if (!Pawn) return nullptr;

	float BestDistance = MAX_FLT;
	AActor* BestPawn = nullptr;
	
	for (const auto PercieveActor: PerceivedActors)
	{
		AGameCharacter* CurrentPercieveCharacter = Cast<AGameCharacter>(PercieveActor);
		const auto StatsComponent = PercieveActor->FindComponentByClass<UStatsComponent>();
	
		//if (CurrentPercieveCharacter && !CurrentPercieveCharacter->StatsComponent->IsDead())
		if (StatsComponent && !StatsComponent->IsDead())
		{
			const auto CurrentDistance = (PercieveActor->GetActorLocation() - Pawn->GetActorLocation()).Size();
			if (CurrentDistance < BestDistance)
			{
				BestDistance = CurrentDistance;
				BestPawn = PercieveActor;
			}
		}
	}
	
	return BestPawn;
}


