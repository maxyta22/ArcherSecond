// Archer Prototype. All rights reserved

#include "Animations/AIMakeShotAN.h"
#include "AI/AICharacter.h"
#include "Player/GameCharacter.h"

void UAIMakeShotAN::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	AAICharacter* AICharacter;
	AICharacter = Cast<AAICharacter>(MeshComp->GetOwner());
	if (!AICharacter) return;
	AICharacter->MakeShot();
}

