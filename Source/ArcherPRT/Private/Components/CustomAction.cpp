// Archer Prototype. All rights reserved


#include "Components/CustomAction.h"
#include "Player/GameCharacter.h"
#include "AI/AICharacter.h"
#include "AI/PRTAIController.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Components/StatsComponent.h"

UCustomAction::UCustomAction()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UCustomAction::BeginPlay()
{
	Super::BeginPlay();
}

void UCustomAction::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UCustomAction::FinishCustomAction_Server_Implementation()
{
	//Server Logic

	const auto Pawn = Cast<AAICharacter>(GetOwner());
	if (!Pawn) return;
	const auto AIController = Cast<APRTAIController>(Pawn->GetController());
	if (!AIController) return;

	if (!Pawn->StatsComponent->IsDead())
	{
		AIController->LockBehavior(false);
	}

	FinishCustomAction_Multicast();
}

void UCustomAction::FinishCustomAction_Multicast_Implementation()
{
	const auto Pawn = Cast<AAICharacter>(GetOwner());
	if (!Pawn) return;

	bCustomActionInProgress = false;

	Pawn->AfterCustomAction();

}

void UCustomAction::TryPerformPlayAnimMontage_Server_Implementation(UAnimMontage* Montage , bool CanInterruptCurrentMontage)
{
	//Server Logic

	if (!CanInterruptCurrentMontage && bCustomActionInProgress) return;
	if (!Montage) return;

	const auto Pawn = Cast<AAICharacter>(GetOwner());
	if (!Pawn) return;

	const auto AIController = Cast<APRTAIController>(Pawn->GetController());
	if (!AIController) return;

	AIController->LockBehavior(true);

	TryPerformPlayAnimMontage_Multicast(Montage, CanInterruptCurrentMontage);
}

void UCustomAction::TryPerformPlayAnimMontage_Multicast_Implementation(UAnimMontage* Montage, bool CanInterruptCurrentMontage)
{
	if (!CanInterruptCurrentMontage && bCustomActionInProgress) return;
	if (!Montage) return;
	const auto Pawn = Cast<AAICharacter>(GetOwner());
	if (!Pawn) return;

	//Abort Current Custom Action
	if (CanInterruptCurrentMontage)
	{
		FinishCustomActionTimer.Invalidate();
		FinishCustomAction_Server();
	}
	
	Pawn->PlayAnimMontage(Montage);

	LastAnimMontage = Montage;
	bCustomActionInProgress = true;

	GetWorld()->GetTimerManager().SetTimer(FinishCustomActionTimer, this, &UCustomAction::FinishCustomAction_Server, Montage->GetPlayLength(), false);
}




