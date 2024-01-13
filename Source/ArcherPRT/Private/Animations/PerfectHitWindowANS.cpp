// Archer Prototype. All rights reserved


#include "Animations/PerfectHitWindowANS.h"
#include "Player/PlayerCharacter.h"
#include "Player/GameCharacter.h"
#include "Components/WeaponComponent.h"



void UPerfectHitWindowANS::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
	const auto LPlayerCharacter = Cast<APlayerCharacter>(MeshComp->GetOwner());

	if (!IsValid(LPlayerCharacter))
		return;

	LPlayerCharacter->WeaponComponent->bPerfectHitWindow = true;
	LPlayerCharacter->WeaponComponent->bPerfectDamage = true;
}



