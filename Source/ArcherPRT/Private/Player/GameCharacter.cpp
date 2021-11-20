// Archer Prototype. All rights reserved.



#include "Player/GameCharacter.h"
#include "Projectile/ArcherPRTProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Components/StatsComponent.h"
#include "Components/WeaponComponent.h"
#include "Components/CustomAction.h"
#include "GameFramework/InputSettings.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"



DEFINE_LOG_CATEGORY_STATIC(LogGameCharacter, Warning, All);



AGameCharacter::AGameCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	//Init Components

	StatsComponent = CreateDefaultSubobject<UStatsComponent>("StatsComponent");
	WeaponComponent = CreateDefaultSubobject<UWeaponComponent>("WeaponComponent");
	CustomAction = CreateDefaultSubobject<UCustomAction>("CustomObjectComponent");

}

void AGameCharacter::BeginPlay()
{
	Super::BeginPlay();
	//Bind Delegate
	StatsComponent->OnHealthChanged.AddUObject(this, &AGameCharacter::OnHealChanged);
	StatsComponent->OnDeath.AddUObject(this, &AGameCharacter::OnDeath);
}

float AGameCharacter::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(Damage,DamageEvent,EventInstigator,DamageCauser);
	return Damage;
}

void AGameCharacter::OnHealChanged(float Health)
{
	
}

void AGameCharacter::OnDeath()
{


}

void AGameCharacter::OnHitReaction()
{
}

void AGameCharacter::MakeStrike(float StrikeDistance, float MinAngle, float MaxAngle)
{
	if (!GetWorld()) return;

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

		if (
			(DamagedActor)
			&& (DamagedActor != this)
			&& (!IgnoreActorsDamage.Contains(DamagedActor))
			)
		{
			const auto OneVector = GetActorForwardVector();
			const auto TwoVector = DamagedActor->GetActorLocation() - GetActorLocation();
			const auto AngleBetween = FMath::Acos(FVector::DotProduct(OneVector.GetSafeNormal(), TwoVector.GetSafeNormal()));
			const auto CrossProduct = FVector::CrossProduct(OneVector, TwoVector);
			const auto Degrees = FMath::RadiansToDegrees(AngleBetween);
			const auto Angle = FMath::RadiansToDegrees(AngleBetween) * FMath::Sign(CrossProduct.Z);


			FString AngleStr = "Angle = " + FString::SanitizeFloat(Angle);
			UE_LOG(LogGameCharacter, Display, TEXT("%s"), *AngleStr);


			if ((Angle >= MinAngle) && (Angle <= MaxAngle))
			{
				UGameplayStatics::ApplyDamage(DamagedActor, StrikeDamage, Controller, this, StrikeDamageType);
				DamagedActor->OnHitReaction();
				IgnoreActorsDamage.Add(DamagedActor);
			}
		}
	}
}

void AGameCharacter::ClearIgnoreActorsDamageStrike()
{
	IgnoreActorsDamage.Empty();
}
