// Archer Prototype. All rights reserved


#include "Animations/MakeShotAN.h"
#include "AI/AICharacter.h"
#include "Player/GameCharacter.h"
#include "Player/PlayerCharacter.h"
#include "Components/WeaponComponent.h"

void UMakeShotAN::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	APlayerCharacter* PlayerCharacterOwner;
	PlayerCharacterOwner = Cast<APlayerCharacter>(MeshComp->GetOwner());
	if (!PlayerCharacterOwner) return;
	PlayerCharacterOwner->WeaponComponent->MakeShot();
}
