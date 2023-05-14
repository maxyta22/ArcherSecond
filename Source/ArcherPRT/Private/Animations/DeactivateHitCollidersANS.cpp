// Archer Prototype. All rights reserved


#include "Animations/DeactivateHitCollidersANS.h"
#include "AI/AICharacter.h"
#include "AI/PRTAIController.h"

void UDeactivateHitCollidersANS::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	AAICharacter* AICharacterOwner;
	AICharacterOwner = Cast<AAICharacter>(MeshComp->GetOwner());
	if (!AICharacterOwner) return;

	AICharacterOwner->ToggleActivateHitColliders(false);
}

void UDeactivateHitCollidersANS::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	AAICharacter* AICharacterOwner;
	AICharacterOwner = Cast<AAICharacter>(MeshComp->GetOwner());
	if (!AICharacterOwner) return;

	AICharacterOwner->ToggleActivateHitColliders(true);
}
