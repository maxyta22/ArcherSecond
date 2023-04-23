// Archer Prototype. All rights reserved


#include "Animations/OverrideSpeedRotationANS.h"
#include "AI/AICharacter.h"

void UOverrideSpeedRotationANS::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	AAICharacter* AICharacterOwner;
	AICharacterOwner = Cast<AAICharacter>(MeshComp->GetOwner());
	if (!AICharacterOwner) return;

	DefaultSpeedRotation = AICharacterOwner->SpeedRotation;
}

void UOverrideSpeedRotationANS::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{

	AAICharacter* AICharacterOwner;
	AICharacterOwner = Cast<AAICharacter>(MeshComp->GetOwner());
	if (!AICharacterOwner) return;

	if (!MeshComp->GetAnimInstance()->GetCurveValue("OverrideSpeedRotation")) return;

	AICharacterOwner->SpeedRotation = MeshComp->GetAnimInstance()->GetCurveValue("OverrideSpeedRotation");
}

void UOverrideSpeedRotationANS::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	AAICharacter* AICharacterOwner;
	AICharacterOwner = Cast<AAICharacter>(MeshComp->GetOwner());
	if (!AICharacterOwner) return;

	AICharacterOwner->SpeedRotation = DefaultSpeedRotation;
}
