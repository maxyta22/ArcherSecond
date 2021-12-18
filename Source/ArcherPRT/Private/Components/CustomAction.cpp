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

void UCustomAction::TryPerformPlayAnimMontage_Server_Implementation(UAnimMontage* Montage , bool CanInterruptCurrentMontage)
{
	TryPerformPlayAnimMontage_Multicast(Montage, CanInterruptCurrentMontage);
}

void UCustomAction::TryPerformPlayAnimMontage_Multicast_Implementation(UAnimMontage* Montage, bool CanInterruptCurrentMontage)
{
	if (!CanInterruptCurrentMontage && bCustomActionInProgress) return;
	if (!Montage) return;
	const auto Pawn = Cast<AAICharacter>(GetOwner());
	if (!Pawn) return;
	const auto AIController = Cast<APRTAIController>(Pawn->GetController());
	if (!AIController) return;
	//Abort Current Custom Action
	if (CanInterruptCurrentMontage)
	{
		FinishCustomActionTimer.Invalidate();
		FinishCustomAction();
	}
	AIController->LockBehavior(true);

	Pawn->PlayAnimMontage(Montage);

	
	
	LastAnimMontage = Montage;
	bCustomActionInProgress = true;

	GetWorld()->GetTimerManager().SetTimer(FinishCustomActionTimer, this, &UCustomAction::FinishCustomAction, Montage->GetPlayLength(), false);
}

void UCustomAction::FinishCustomAction()
{
	const auto Pawn = Cast<AAICharacter>(GetOwner());
	if (!Pawn) return;
	const auto AIController = Cast<APRTAIController>(Pawn->GetController());
	if (!AIController) return;
	
	if (!Pawn->StatsComponent->IsDead())
	{
		AIController->LockBehavior(false);
	}
	
	bCustomActionInProgress = false;

	Pawn->AfterCustomAction();
	
}



