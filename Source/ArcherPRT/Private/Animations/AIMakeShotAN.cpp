// Archer Prototype. All rights reserved

#include "AI/AICharacter.h"
#include "Player/GameCharacter.h"
#include "Animations/AIMakeShotAN.h"

void UAIMakeShotAN::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	AAICharacter* AICharacter;
	AICharacter = Cast<AAICharacter>(MeshComp->GetOwner());
	if (!AICharacter) return;
	AICharacter->MakeShot();
}

