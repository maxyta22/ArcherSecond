// Archer Prototype. All rights reserved

#include "AI/AICharacter.h"
#include "AI/PRTAIController.h"
#include "Player/PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BrainComponent.h"
#include "Components/CapsuleComponent.h"
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

#pragma region ReactionToAiming

void AAICharacter::StartAccumulateToAiming()
{
	if (GetWorld() == nullptr) return;
	if (!IsAlive()) return;
	if (GetWorld()->GetTimerManager().IsTimerActive(AccumulateToAiminHandleTimer)) return;
	if (!GetAIControllerRef()) return;
	if (!GetAIControllerRef()->GetEnemy()) return;
	GetWorld()->GetTimerManager().SetTimer(AccumulateToAiminHandleTimer, this, &AAICharacter::ReactionToAiming, TimeToReactionToAiming);
}

void AAICharacter::FinishAccumulateToAiming()
{
	if (GetWorld() == nullptr) return;
	if (!IsAlive()) return;
	if (GetWorld()->GetTimerManager().IsTimerActive(AccumulateToAiminHandleTimer))
	{
		GetWorld()->GetTimerManager().ClearTimer(AccumulateToAiminHandleTimer);
	}
	
}

void AAICharacter::ReactionToAiming()
{
	AfterReactionToAiming();
}

#pragma endregion

#pragma region Behavior

void AAICharacter::RotationOnTarget()
{
	if (GetWorld() == nullptr) return;
	if (!IsAlive() && !GetCurrentMontage()) return;
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

#pragma endregion

#pragma region TakeDamage

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

void AAICharacter::ImplementTakeDamage(FDamageData DamageData)
{
	Super::ImplementTakeDamage(DamageData);
	const auto hitDirection = DamageData.DamageDirection;
	const auto hitResult = DamageData.DamagePoint;
	const auto weaponType = DamageData.DamageWeaponType;
	const auto damageCauser = DamageData.DamageCauser;
	const auto damageInstigator = DamageData.DamageInstigator;
	bool charged = DamageData.DamageCharged;

	const auto AIController = Cast<APRTAIController>(Controller);

	if (AIController && damageInstigator)
	{
		AIController->SetEnemy(damageInstigator->GetPawn());
	}
	
	AfterOnHit(DamageData);

	if (CanPerformOnHitReaction)
	{
		PerformOnHitReaction(DamageData);
	}
}



#pragma endregion 

#pragma region MakeDamage

void AAICharacter::MakeStrike(float StrikeDistance, float MinAngle, float MaxAngle, bool IgnoreBlock, bool MakeStagger)
{
	if (GetWorld() == nullptr) return;
	if (!IsAlive()) return;
	StrikeInProgress();

	TArray<FOverlapResult> OverlapResults;
	FCollisionObjectQueryParams ObjectQueryParam;
	ObjectQueryParam.AllObjects;
	FQuat Rot;
	FCollisionShape CollisionShape;
	CollisionShape.SetSphere(StrikeDistance);
	AGameCharacter* DamagedActor;

	FDamageData damageData;
	damageData.DamageDirection = GetActorForwardVector();
	damageData.DamageInstigator = GetInstigatorController();
	damageData.DamageCauser = this;
	damageData.DamageGameplayEffect = StrikeGameplayEffect;
	
	GetWorld()->OverlapMultiByObjectType(OverlapResults, GetActorLocation(), Rot, ObjectQueryParam, CollisionShape);

	for (FOverlapResult& OverlapResult : OverlapResults)
	{
		DamagedActor = Cast<AGameCharacter>(OverlapResult.GetActor());

		if (IsValid(DamagedActor) && (DamagedActor != this) && (!IgnoreActorsDamage.Contains(DamagedActor)))
		{
			const auto Angle = UMathUtils::FindAngleBetweenForwardVectorAndTarget(GetActorLocation(), GetActorForwardVector(), DamagedActor->GetActorLocation());

			if ((Angle >= MinAngle) && (Angle <= MaxAngle))
			{
				const auto DamagedPlayerCharacter = Cast<APlayerCharacter>(DamagedActor);
				if (DamagedPlayerCharacter)
				{
					if (!DamagedPlayerCharacter->WeaponComponent->BlockInProgress() || IgnoreBlock == true)
					{
						DamagedActor->ImplementTakeDamage(damageData);
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
						DamagedActor->HitOnBlockSuccess(damageData);
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

#pragma endregion 

#pragma region Death

void AAICharacter::OnDeath()
{
	Super::OnDeath();
	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
	GetWorld()->GetTimerManager().SetTimer(DestroyCorpseTimer, this, &AAICharacter::DestroyCorpse, TimeToDestroyCorpse);
}

void AAICharacter::DestroyCorpse()
{
	Destroy();	
}

#pragma endregion 

