// Archer Prototype. All rights reserved


#include "Animations/OverrideMovementANS.h"
#include "Player/GameCharacter.h"
#include "AI/AICharacter.h"
#include "AI/PRTAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/StatsComponent.h"

void UOverrideMovementANS::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{

	AAICharacter* AICharacterOwner;
	AICharacterOwner = Cast<AAICharacter>(MeshComp->GetOwner());
	if (!AICharacterOwner) return;

	APRTAIController* AIController;
	AIController = Cast<APRTAIController>(AICharacterOwner->GetController());
	if (!AIController) return;

	UCharacterMovementComponent* MovementComponent = Cast<UCharacterMovementComponent>(AICharacterOwner->GetMovementComponent());

	if (!MovementComponent) return;

	DefaultAccelerator = MovementComponent->MaxAcceleration;
	MovementComponent->MaxAcceleration = 100000;

	DefaultMaxWalkSpeed = MovementComponent->MaxWalkSpeed;



}

void UOverrideMovementANS::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{

	AAICharacter* AICharacterOwner;
	AICharacterOwner = Cast<AAICharacter>(MeshComp->GetOwner());
	if (!AICharacterOwner) return;

	APRTAIController* AIController;
	AIController = Cast<APRTAIController>(AICharacterOwner->GetController());
	if (!AIController) return;

	UCharacterMovementComponent* MovementComponent = Cast<UCharacterMovementComponent>(AICharacterOwner->GetMovementComponent());

	if (!MovementComponent) return;

	if (!MeshComp->GetAnimInstance()->GetCurveValue("ForwardRootMotion")) return;

	MovementComponent->MaxWalkSpeed = MeshComp->GetAnimInstance()->GetCurveValue("ForwardRootMotion");

	if (MeshComp->GetAnimInstance()->GetCurveValue("ForwardRootMotion")>0 && !AICharacterOwner->StatsComponent->IsDead())
	{
		AICharacterOwner->AddMovementInput(AICharacterOwner->GetActorForwardVector(), 1);
	}

	if (MeshComp->GetAnimInstance()->GetCurveValue("ForwardRootMotion") < 0 && !AICharacterOwner->StatsComponent->IsDead())
	{
		AICharacterOwner->AddMovementInput(AICharacterOwner->GetActorForwardVector()*-1, 1);
	}
	
}

void UOverrideMovementANS::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{

	AAICharacter* AICharacterOwner;
	AICharacterOwner = Cast<AAICharacter>(MeshComp->GetOwner());
	if (!AICharacterOwner) return;

	APRTAIController* AIController;
	AIController = Cast<APRTAIController>(AICharacterOwner->GetController());
	if (!AIController) return;

	UCharacterMovementComponent* MovementComponent = Cast<UCharacterMovementComponent>(AICharacterOwner->GetMovementComponent());

	if (!MovementComponent) return;

	MovementComponent->MaxWalkSpeed = DefaultMaxWalkSpeed;
	MovementComponent->MaxAcceleration = DefaultAccelerator;
}
