// Archer Prototype. All rights reserved


#include "Animations/LockFocusANS.h"
#include "Player/GameCharacter.h"
#include "AI/AICharacter.h"
#include "AI/PRTAIController.h"

void ULockFocusANS::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	AAICharacter* AICharacterOwner;
	AICharacterOwner = Cast<AAICharacter>(MeshComp->GetOwner());
	if (!AICharacterOwner) return;

	APRTAIController* AIController;
	AIController = Cast<APRTAIController>(AICharacterOwner->GetController());
	if (!AIController) return;

	AIController->FocusActive = false;

	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Green, TEXT("False"));
	
}

void ULockFocusANS::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{

}

void ULockFocusANS::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

	AAICharacter* AICharacterOwner;
	AICharacterOwner = Cast<AAICharacter>(MeshComp->GetOwner());
	if (!AICharacterOwner) return;

	APRTAIController* AIController;
	AIController = Cast<APRTAIController>(AICharacterOwner->GetController());
	if (!AIController) return;

	AIController->FocusActive = true;

	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Green, TEXT("True"));

}
