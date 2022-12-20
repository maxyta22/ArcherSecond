// Archer Prototype. All rights reserved


#include "Animations/OverrideRotationRateANS.h"
#include "AI/AICharacter.h"
#include "AI/PRTAIController.h"
#include "GameFramework/CharacterMovementComponent.h"

void UOverrideRotationRateANS::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	AAICharacter* AICharacterOwner;
	AICharacterOwner = Cast<AAICharacter>(MeshComp->GetOwner());
	if (!AICharacterOwner) return;

	APRTAIController* AIController;
	AIController = Cast<APRTAIController>(AICharacterOwner->GetController());
	if (!AIController) return;

	UCharacterMovementComponent* MovementComponent = Cast<UCharacterMovementComponent>(AICharacterOwner->GetMovementComponent());

	if (!MovementComponent) return;

	DefaultRotationRateYaw = MovementComponent->RotationRate.Yaw;
}

void UOverrideRotationRateANS::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
	AAICharacter* AICharacterOwner;
	AICharacterOwner = Cast<AAICharacter>(MeshComp->GetOwner());
	if (!AICharacterOwner) return;

	APRTAIController* AIController;
	AIController = Cast<APRTAIController>(AICharacterOwner->GetController());
	if (!AIController) return;

	UCharacterMovementComponent* MovementComponent = Cast<UCharacterMovementComponent>(AICharacterOwner->GetMovementComponent());

	if (!MovementComponent) return;

	if (!MeshComp->GetAnimInstance()->GetCurveValue("OverrideRotationRate")) return;

	MovementComponent->RotationRate.Yaw = MeshComp->GetAnimInstance()->GetCurveValue("OverrideRotationRate");
}

void UOverrideRotationRateANS::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	AAICharacter* AICharacterOwner;
	AICharacterOwner = Cast<AAICharacter>(MeshComp->GetOwner());
	if (!AICharacterOwner) return;

	APRTAIController* AIController;
	AIController = Cast<APRTAIController>(AICharacterOwner->GetController());
	if (!AIController) return;

	UCharacterMovementComponent* MovementComponent = Cast<UCharacterMovementComponent>(AICharacterOwner->GetMovementComponent());

	if (!MovementComponent) return;

	MovementComponent->RotationRate.Yaw = DefaultRotationRateYaw;
}
