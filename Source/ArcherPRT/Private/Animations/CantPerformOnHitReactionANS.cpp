// Archer Prototype. All rights reserved


#include "Animations/CantPerformOnHitReactionANS.h"
#include "Player/GameCharacter.h"
#include "AI/AICharacter.h"

void UCantPerformOnHitReactionANS::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	AAICharacter* AICharacterOwner;
	AICharacterOwner = Cast<AAICharacter>(MeshComp->GetOwner());
	if (!AICharacterOwner) return;
	AICharacterOwner->CanPerformOnHitReaction = false;
}

void UCantPerformOnHitReactionANS::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	AAICharacter* AICharacterOwner;
	AICharacterOwner = Cast<AAICharacter>(MeshComp->GetOwner());
	if (!AICharacterOwner) return;
	AICharacterOwner->CanPerformOnHitReaction = true;
}
