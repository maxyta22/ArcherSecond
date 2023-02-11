// Copyright Epic Games, Inc. All Rights Reserved.

#include "Core/ArcherPRTGameMode.h"
#include "Core/ArcherPRTHUD.h"
#include "Player/PlayerCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "AI/AICharacter.h"
#include "AI/PRTAIController.h"
#include "Components/StatsComponent.h"
#include "UObject/ConstructorHelpers.h"

AArcherPRTGameMode::AArcherPRTGameMode()
	: Super()
{
	HUDClass = AArcherPRTHUD::StaticClass();
}

bool AArcherPRTGameMode::InBattleMode()
{
	if (!GetWorld()) return false;

	TArray<AActor*> OverlapActors;
	TArray<AActor*> IgnoreActors;
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));
	UClass* FilterClass = AAICharacter::StaticClass();
	const auto Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

	UKismetSystemLibrary::SphereOverlapActors(GetWorld(), Player->GetActorLocation(), 1000, ObjectTypes, FilterClass, IgnoreActors, OverlapActors);

	if (OverlapActors.IsEmpty())
	{
		return false;
	}
	

	for (AActor* OverlappedActor : OverlapActors)
	{
		const auto AICharacter = Cast<AAICharacter>(OverlappedActor);

		if (AICharacter && !AICharacter->StatsComponent->IsDead())
		{
			const auto AIController = Cast<APRTAIController>(AICharacter->GetController());
			
			if (AIController && AIController->GetEnemy())
			{
				float ZPlayer = Player->GetActorLocation().Z;
				float ZEnemy = AICharacter->GetActorLocation().Z;	
				const auto ZDistance = UKismetMathLibrary::Vector_Distance(FVector(0, 0, ZPlayer), FVector(0, 0, ZEnemy));

				if (ZDistance < Z_ErrorBattleMode)
				{
					return true;
				}
				
			}
			
		}
	}

	return false;
}
