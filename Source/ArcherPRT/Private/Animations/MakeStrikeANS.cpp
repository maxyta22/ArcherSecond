// Archer Prototype. All rights reserved


#include "Animations/MakeStrikeANS.h"
#include "Player/GameCharacter.h"

void UMakeStrikeANS::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	AGameCharacter* CharacterOwner;
	CharacterOwner = Cast<AGameCharacter>(MeshComp->GetOwner());
	if (!CharacterOwner) return;
	CharacterOwner->ClearTempInrenalActors();
}

void UMakeStrikeANS::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
	AGameCharacter* CharacterOwner;
	CharacterOwner = Cast<AGameCharacter>(MeshComp->GetOwner());
	if (!CharacterOwner) return;
	CharacterOwner->MakeStrike(Distance, MinAngle, MaxAngle);
}

void UMakeStrikeANS::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	AGameCharacter* CharacterOwner;
	CharacterOwner = Cast<AGameCharacter>(MeshComp->GetOwner());
	if (!CharacterOwner) return;

	if (CharacterOwner->CheckMiss())
	{
		CharacterOwner->MakeMiss();
	}

	CharacterOwner->ClearTempInrenalActors();

}
