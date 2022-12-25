// Archer Prototype. All rights reserved.



#include "Player/GameCharacter.h"
#include "Projectile/ArcherPRTProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Player/PlayerCharacter.h"
#include "AI/AICharacter.h"
#include "Components/InputComponent.h"
#include "Components/StatsComponent.h"
#include "Components/WeaponComponent.h"
#include "Components/CustomAction.h"
#include "GameFramework/InputSettings.h"
#include "Net/UnrealNetwork.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "MathUtils.h"
#include "GameFramework/CharacterMovementComponent.h"



DEFINE_LOG_CATEGORY_STATIC(LogGameCharacter, Warning, All);



AGameCharacter::AGameCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	//Init Components

	StatsComponent = CreateDefaultSubobject<UStatsComponent>("StatsComponent");
	StatsComponent->SetIsReplicated(true);

	WeaponComponent = CreateDefaultSubobject<UWeaponComponent>("WeaponComponent");
	WeaponComponent->SetIsReplicated(true);

	CustomAction = CreateDefaultSubobject<UCustomAction>("CustomObjectComponent");
	CustomAction->SetIsReplicated(true);

	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>("AbilitySystemComponent");

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
	Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	return Damage;
}

void AGameCharacter::OnHealChanged(float Health)
{
	
}

void AGameCharacter::OnDeath()
{
	AfterOnDeath();
}

void AGameCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);
	const auto FallVelocityZ = -GetCharacterMovement()->Velocity.Z;
	if (FallVelocityZ < LandedDamageVelocity.X) return;

	const auto FinalDamage = FMath::GetMappedRangeValueClamped(LandedDamageVelocity, LandedDamage, FallVelocityZ);
	TakeDamage(FinalDamage, FDamageEvent{}, nullptr, nullptr);

}

void AGameCharacter::OnHit(FVector HitDirection, FHitResult HitResult, AActor* Causer, EWeaponType WeaponType, bool Charged)
{
}

void AGameCharacter::MakeStrike(float StrikeDistance, float MinAngle, float MaxAngle)
{
}

void AGameCharacter::ClearIgnoreActorsDamageStrike()
{
	IgnoreActorsDamage.Empty();
}
