// Archer Prototype. All rights reserved


#include "Animations/IverrideSpeedRotationANS.h"
#include "AI/AICharacter.h"

void UIverrideSpeedRotationANS::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	AAICharacter* AICharacterOwner;
	AICharacterOwner = Cast<AAICharacter>(MeshComp->GetOwner());
	if (!AICharacterOwner) return;

	DefaultSpeedRotation = AICharacterOwner->SpeedRotation;
}

void UIverrideSpeedRotationANS::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{

	AAICharacter* AICharacterOwner;
	AICharacterOwner = Cast<AAICharacter>(MeshComp->GetOwner());
	if (!AICharacterOwner) return;

	if (!MeshComp->GetAnimInstance()->GetCurveValue("OverrideSpeedRotation")) return;

	AICharacterOwner->SpeedRotation = MeshComp->GetAnimInstance()->GetCurveValue("OverrideSpeedRotation");
}

void UIverrideSpeedRotationANS::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	AAICharacter* AICharacterOwner;
	AICharacterOwner = Cast<AAICharacter>(MeshComp->GetOwner());
	if (!AICharacterOwner) return;

	AICharacterOwner->SpeedRotation = DefaultSpeedRotation;
}
