// Copyright Epic Games, Inc. All Rights Reserved.

#include "Core/ArcherPRTGameMode.h"
#include "Core/ArcherPRTHUD.h"
#include "Player/PlayerCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "AI/AICharacter.h"
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
	

	//for (AActor* overlappedActor : OverlapActors)
	//{

	//}
	return true;
}
