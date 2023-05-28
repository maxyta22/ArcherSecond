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
#include "GameFramework/InputSettings.h"
#include "GameplayAbilitySystem/PRTAbilitySystemComponent.h"
#include "GameplayAbilitySystem/PRTAttributeSet.h"
#include "GameplayAbilitySystem/PRTGameplayAbility.h"
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

	AbilitySystemComponent = CreateDefaultSubobject<UPRTAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	Attributes = CreateDefaultSubobject<UPRTAttributeSet>("Attributes");

}

void AGameCharacter::BeginPlay()
{
	Super::BeginPlay();
	//Bind Delegate
	StatsComponent->OnHealthChanged.AddUObject(this, &AGameCharacter::OnHealChanged);
	StatsComponent->OnDeath.AddUObject(this, &AGameCharacter::OnDeath);
}

UAbilitySystemComponent* AGameCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AGameCharacter::InitializeAttributes()
{
	if (AbilitySystemComponent && DefaultAttributeEffect)
	{
		FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
		EffectContext.AddSourceObject(this);

		FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(DefaultAttributeEffect, 1, EffectContext);

		if (SpecHandle.IsValid())
		{
			FActiveGameplayEffectHandle GEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		}

	}
}

void AGameCharacter::GiveAbilities()
{
	if (HasAuthority() && AbilitySystemComponent)
	{
		for (TSubclassOf<UPRTGameplayAbility>& StartupAbility : DefaultAbilities)
		{
			AbilitySystemComponent->GiveAbility(
				FGameplayAbilitySpec(StartupAbility, 1, static_cast<int32>(StartupAbility.GetDefaultObject()->AbilityInputID), this));
		}
	}
}

void AGameCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	AbilitySystemComponent->InitAbilityActorInfo(this, this);

	InitializeAttributes();
	GiveAbilities();
	CanCheckAttributes = true;
}

float AGameCharacter::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	return Damage;
}

bool AGameCharacter::IsInvulnerable()
{
	return bInvulnerable;
}

void AGameCharacter::OnHealthAttributeChanged()
{
	if (CanCheckAttributes)
	{
		if (!IsAlive())
		{
			OnDeath();
		}
	}
		
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
