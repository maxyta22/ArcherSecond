// Archer Prototype. All rights reserved


#include "Animations/MakeStrikeANS.h"
#include "Player/GameCharacter.h"

void UMakeStrikeANS::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	AGameCharacter* CharacterOwner;
	CharacterOwner = Cast<AGameCharacter>(MeshComp->GetOwner());
	if (!IsValid(CharacterOwner)) return;
	CharacterOwner->ClearTempInternalActors();
}

void UMakeStrikeANS::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
	AGameCharacter* CharacterOwner;
	CharacterOwner = Cast<AGameCharacter>(MeshComp->GetOwner());

	if (!IsValid(CharacterOwner)) return;

	CharacterOwner->MakeStrike(Distance, MinAngle, MaxAngle, IgnoreBlock, MakeStagger);

	if (MakeStagger && !ChargeSuccess)
	{
		CharacterOwner->ChargeSuccess();
		ChargeSuccess = true;
	}
	
	
}

void UMakeStrikeANS::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	AGameCharacter* CharacterOwner;
	CharacterOwner = Cast<AGameCharacter>(MeshComp->GetOwner());
	if (!IsValid(CharacterOwner)) return;

	if (CharacterOwner->CheckMissSuccess())
	{
		CharacterOwner->MakeMiss();
	}

	CharacterOwner->ClearTempInternalActors();

}
