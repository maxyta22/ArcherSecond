// Archer Prototype. All rights reserved

#include "AI/AICharacter.h"
#include "AI/PRTAIController.h"
#include "Player/PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BrainComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/StatsComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Engine/TargetPoint.h"
#include "TimerManager.h"
#include "Environment/PickupResourcesBase.h"
#include "MathUtils.h"
#include "Kismet/KismetMathLibrary.h"
#include "Particles/ParticleSystemComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Components/WeaponComponent.h"


AAICharacter::AAICharacter()
{
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	AIControllerClass = APRTAIController::StaticClass();

	//Setup Capsule Collision
	GetCapsuleComponent()->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Block);
	
	
	// Setup Smooth Rotation (Used Unreal Rotation when Movement (RotationRate)
	bUseControllerRotationYaw = false;
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->bUseControllerDesiredRotation = false;
		GetCharacterMovement()->bOrientRotationToMovement = true; //Setup true if need use Unreal RotationRate
		GetCharacterMovement()->RotationRate = FRotator(0.0f, 180.0f, 0.0f);
	}
}

void AAICharacter::BeginPlay()
{
	Super::BeginPlay();

	AIControllerRef = Cast<APRTAIController>(GetController());
}

void AAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	RotationOnTarget();
}

//Accumulate To Aiming 

void AAICharacter::StartAccumulateToAiming()
{
	if (GetWorld() == nullptr) return;
	if (StatsComponent->IsDead()) return;
	if (GetWorld()->GetTimerManager().IsTimerActive(AccumulateToAiminHandleTimer)) return;
	if (!GetAIControllerRef()) return;
	if (!GetAIControllerRef()->GetEnemy()) return;
	GetWorld()->GetTimerManager().SetTimer(AccumulateToAiminHandleTimer, this, &AAICharacter::ReactionToAiming, TimeToReactionToAiming);
}

void AAICharacter::FinishAccumulateToAiming()
{
	if (GetWorld() == nullptr) return;
	if (StatsComponent->IsDead()) return;
	if (GetWorld()->GetTimerManager().IsTimerActive(AccumulateToAiminHandleTimer))
	{
		GetWorld()->GetTimerManager().ClearTimer(AccumulateToAiminHandleTimer);
	}
	
}

void AAICharacter::ReactionToAiming()
{
	AfterReactionToAiming();
}


void AAICharacter::RotationOnTarget()
{
	if (GetWorld() == nullptr) return;
	if (StatsComponent->IsDead() && !GetCurrentMontage()) return;
	if (!AIControllerRef) return;
	if (!AIControllerRef->GetBlackboardComponent()) return;
	if (!AIControllerRef->GetBlackboardComponent()->GetValueAsObject("EnemyActor")) return;
	if (SpeedRotation == 0) return;

	const AActor* Target = Cast<AActor>(AIControllerRef->GetBlackboardComponent()->GetValueAsObject("EnemyActor"));

	if (Target)
	{
		const auto DesiredRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Target->GetActorLocation());
		const auto XYZRotation = FMath::RInterpTo(GetActorRotation(), DesiredRotation, GetWorld()->GetDeltaSeconds(), SpeedRotation);
		const auto ZRotation = FRotator(GetActorRotation().Pitch, XYZRotation.Yaw, GetActorRotation().Roll);
		SetActorRotation(ZRotation);
	}
}

FVector AAICharacter::GetNextPatrolTargetPointLocation()
{
	if (TargetsForPatrol.Num() == 0) return FVector::ZeroVector;

	if (TargetsForPatrol[CurrentPatrolIndex])
	{
		const auto TargetLocation = TargetsForPatrol[CurrentPatrolIndex]->GetActorLocation();

		CurrentPatrolIndex++;

		if (CurrentPatrolIndex >= TargetsForPatrol.Num())
		{
			CurrentPatrolIndex = 0;
		}
		return TargetLocation;
	}

	else
	{
		return FVector::ZeroVector;
	}

}

void AAICharacter::ToggleActivateHitColliders(bool Activate)
{

	const auto Components = GetComponents();

	for (UActorComponent* CurrentComponent : Components)
	{	
		const auto Collision = Activate ? ECollisionEnabled::QueryAndPhysics : ECollisionEnabled::NoCollision;

		if (CurrentComponent->ComponentHasTag("Head"))
		{
			UPrimitiveComponent* CurrentPrimitiveComponent = Cast<UPrimitiveComponent>(CurrentComponent);
			CurrentPrimitiveComponent->SetCollisionEnabled(Collision);
		}

		if (CurrentComponent->ComponentHasTag("Torso"))
		{
			UPrimitiveComponent* CurrentPrimitiveComponent = Cast<UPrimitiveComponent>(CurrentComponent);
			CurrentPrimitiveComponent->SetCollisionEnabled(Collision);
		}	

		if (CurrentComponent->ComponentHasTag("WeakPoint"))
		{
			UPrimitiveComponent* CurrentPrimitiveComponent = Cast<UPrimitiveComponent>(CurrentComponent);
			CurrentPrimitiveComponent->SetCollisionEnabled(Collision);
		}
	}
}

// Take Damage

float AAICharacter::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	AfterTakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	const auto AIController = Cast<APRTAIController>(Controller);
	if (AIController && EventInstigator)
	{
		AIController->SetEnemy(EventInstigator->GetPawn());
	}
		
	return Damage;
}

void AAICharacter::OnDeath()
{
	Super::OnDeath();
	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);

}

void AAICharacter::OnHit(FVector HitDirection, FHitResult HitResult, AActor* Causer, EWeaponType WeaponType, bool Charged)
{
	AfterOnHit(HitDirection, HitResult, Causer, WeaponType, Charged);

	if (CanPerformOnHitReaction)
	{
	 PerformOnHitReaction(HitDirection, HitResult, Causer, WeaponType, Charged);
	}
	

}

void AAICharacter::MakeStrike(float StrikeDistance, float MinAngle, float MaxAngle, bool IgnoreBlock)
{
	if (GetWorld() == nullptr) return;
	if (StatsComponent->IsDead()) return;
	StrikeInProgress();

	TArray<FOverlapResult> OverlapResult;
	FCollisionObjectQueryParams ObjectQueryParam;
	ObjectQueryParam.AllObjects;
	FQuat Rot;
	FCollisionShape CollisionShape;
	CollisionShape.SetSphere(StrikeDistance);
	AGameCharacter* DamagedActor;

	GetWorld()->OverlapMultiByObjectType(OverlapResult, GetActorLocation(), Rot, ObjectQueryParam, CollisionShape);

	for (int32 i = 0; i < OverlapResult.Num(); i++)
	{
		DamagedActor = Cast<AGameCharacter>(OverlapResult[i].GetActor());

		if ((DamagedActor) && (DamagedActor) && (DamagedActor != this) && (!IgnoreActorsDamage.Contains(DamagedActor)))
		{
			const auto Angle = UMathUtils::FindAngleBetweenForwardVectorAndTarget(GetActorLocation(), GetActorForwardVector(), DamagedActor->GetActorLocation());

			if ((Angle >= MinAngle) && (Angle <= MaxAngle))
			{
				const auto DamagedPlayerCharacter = Cast<APlayerCharacter>(DamagedActor);
				if (DamagedPlayerCharacter)
				{
					if (!DamagedPlayerCharacter->WeaponComponent->BlockInProgress() || IgnoreBlock == true)
					{
						DamagedActor->TakeDamage(StrikeDamage, FDamageEvent(), GetInstigatorController(), this);
						DamagedActor->OnHit(GetActorForwardVector(), FHitResult::FHitResult(), this, EWeaponType::None, false);
						if (HitOnSuccessSound)
						{
							UGameplayStatics::PlaySoundAtLocation(GetWorld(), HitOnSuccessSound, GetActorLocation(), 1.0, 1.0, 0.0);
						}
						if (HitOnSuccessVFX)
						{
							UNiagaraComponent* SpawnNiagaraEffect = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), HitOnSuccessVFX, GetMesh()->GetSocketLocation("VFXStrikeSocket"), GetMesh()->GetSocketRotation("VFXStrikeSocket"));
						}
					}
					else
					{
						DamagedActor->HitOnBlockSuccess(GetActorForwardVector(), FHitResult::FHitResult(), this, EWeaponType::None, false);
						PerformOnStaggerReaction();
						if (HitOnBlockSound)
						{
							UGameplayStatics::PlaySoundAtLocation(GetWorld(), HitOnBlockSound, GetActorLocation(), 1.0, 1.0, 0.0);
						}
						if (HitOnBlockVFX)
						{
							UNiagaraComponent* SpawnNiagaraEffect = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), HitOnBlockVFX, GetMesh()->GetSocketLocation("VFXStrikeSocket"), GetMesh()->GetSocketRotation("VFXStrikeSocket"));
						}
					}
					IgnoreActorsDamage.Add(DamagedActor);
				}
			}
		}
	}
}
