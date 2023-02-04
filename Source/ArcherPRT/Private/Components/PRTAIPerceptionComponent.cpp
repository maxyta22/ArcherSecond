// Archer Prototype. All rights reserved


#include "Components/PRTAIPerceptionComponent.h"
#include "AIController.h"
#include "AI/AICharacter.h"
#include "GameFramework/Actor.h"
#include "Components/StatsComponent.h"
#include "Player/GameCharacter.h"
#include "Player/PlayerCharacter.h"
#include "Perception/AISense_Sight.h"

AActor* UPRTAIPerceptionComponent::GetNearestEnemy() const
{
	const auto Controller = Cast<AAIController>(GetOwner());
	if (!Controller) return nullptr;

	AAICharacter* Pawn = Cast<AAICharacter>(Controller->GetPawn());
	if (!Pawn) return nullptr;

	TArray<AActor*> PerceivedActors;
	GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(), PerceivedActors);

	if (PerceivedActors.IsEmpty()) return nullptr;

	float BestDistance = MAX_FLT;
	AActor* BestPawn = nullptr;
	float MaxDistanceForDetection = Pawn->MaxDistanceDetection;
	
	for (const auto PercieveActor : PerceivedActors)
	{
		const auto StatsComponent = PercieveActor->FindComponentByClass<UStatsComponent>();

		AGameCharacter* CurrentPercieveCharacter = Cast<AGameCharacter>(PercieveActor);

		if (CurrentPercieveCharacter)
		{
			if (OnlySightOnPlayer)
			{
				if (CurrentPercieveCharacter->IsA(APlayerCharacter::StaticClass()))
				{
					if (StatsComponent && !StatsComponent->IsDead())
					{
						const auto CurrentDistance = Pawn->GetDistanceTo(PercieveActor);
						if ((CurrentDistance < BestDistance) && (CurrentDistance < MaxDistanceForDetection))
						{
							BestDistance = CurrentDistance;
							BestPawn = PercieveActor;
						}
					}
				}
			}
			else
			{
				if (StatsComponent && !StatsComponent->IsDead())
				{
					const auto CurrentDistance = (PercieveActor->GetActorLocation() - Pawn->GetActorLocation()).Size();
					if ((CurrentDistance < BestDistance) && (CurrentDistance < MaxDistanceForDetection))
					{
						BestDistance = CurrentDistance;
						BestPawn = PercieveActor;
					}
				}
			}
		}

	}
	
	return BestPawn;
}


