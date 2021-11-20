// Archer Prototype. All rights reserved


#include "Animations/MakeStrikeANS.h"
#include "Player/GameCharacter.h"

void UMakeStrikeANS::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	AGameCharacter* CharacterOwner;
	CharacterOwner = Cast<AGameCharacter>(MeshComp->GetOwner());
	if (!CharacterOwner) return;
	CharacterOwner->ClearIgnoreActorsDamageStrike();
}

void UMakeStrikeANS::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);
	AGameCharacter* CharacterOwner;
	CharacterOwner = Cast<AGameCharacter>(MeshComp->GetOwner());
	if (!CharacterOwner) return;
	CharacterOwner->MakeStrike(Distance, MinAngle, MaxAngle);
}

void UMakeStrikeANS::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
	AGameCharacter* CharacterOwner;
	CharacterOwner = Cast<AGameCharacter>(MeshComp->GetOwner());
	if (!CharacterOwner) return;
	CharacterOwner->ClearIgnoreActorsDamageStrike();
}
