// Archer Prototype. All rights reserved


#include "Animations/RotationOnTargetANS.h"
#include "Player/GameCharacter.h"
#include "AI/AICharacter.h"
#include "AI/PRTAIController.h"

void URotationOnTargetANS::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
	AAICharacter* AICharacterOwner;
	AICharacterOwner = Cast<AAICharacter>(MeshComp->GetOwner());
	if (!AICharacterOwner) return;

	APRTAIController* AIController;
	AIController = Cast<APRTAIController>(AICharacterOwner->GetController());
	if (!AIController) return;

	AICharacterOwner->RotationOnTarget();
}
