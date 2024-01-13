// Archer Prototype. All rights reserved


#include "Animations/PerfectHitWindowANS.h"
#include "Player/PlayerCharacter.h"
#include "Player/GameCharacter.h"
#include "Components/WeaponComponent.h"



void UPerfectHitWindowANS::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	const auto LPlayerCharacter = Cast<APlayerCharacter>(MeshComp->GetOwner());

	if (!IsValid(LPlayerCharacter))
		return;

	LPlayerCharacter->OnStartPerfectWindow();
	LPlayerCharacter->WeaponComponent->bPerfectHitWindow = true;
}

void UPerfectHitWindowANS::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	const auto LPlayerCharacter = Cast<APlayerCharacter>(MeshComp->GetOwner());

	if (!IsValid(LPlayerCharacter))
		return;


	LPlayerCharacter->WeaponComponent->bPerfectHitWindow = false;
}





