// Archer Prototype. All rights reserved


#include "Animations/MakeReloadWeaponAN.h"
#include "Player/PlayerCharacter.h"
#include "Player/GameCharacter.h"
#include "Components/WeaponComponent.h"

void UMakeReloadWeaponAN::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	const auto LPlayerCharacter = Cast<APlayerCharacter>(MeshComp->GetOwner());

	if (!IsValid(LPlayerCharacter))
		return;

	LPlayerCharacter->WeaponComponent->FinishReloadWeapon();
}
