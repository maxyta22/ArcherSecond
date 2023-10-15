// Archer Prototype. All rights reserved.

#include "Player/GameCharacter.h"
#include "Projectile/ArcherPRTProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Player/PlayerCharacter.h"
#include "AI/AICharacter.h"
#include "Components/InputComponent.h"
#include "Components/WeaponComponent.h"
#include "GameFramework/InputSettings.h"
#include "GameplayAbilitySystem/PRTAbilitySystemComponent.h"
#include "GameplayAbilitySystem/PRTAttributeSet.h"
#include "GameplayAbilitySystem/PRTGameplayAbility.h"
#include "Net/UnrealNetwork.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "MathUtils.h"
#include "GameplayEffect.h"
#include "Core/ArcherPRTData.h"
#include "GameFramework/CharacterMovementComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogGameCharacter, Warning, All);

AGameCharacter::AGameCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	//Init Components

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
	
}

void AGameCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	InitializeAttributes();
	GiveAbilities();
	WasInitiatedAttributes = true;

}

void AGameCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);
	const auto FallVelocityZ = -GetCharacterMovement()->Velocity.Z;
	if (FallVelocityZ < LandedDamageVelocity.X) return;

	LastDamageFromLanded = FMath::GetMappedRangeValueClamped(LandedDamageVelocity, LandedDamage, FallVelocityZ);
	ImplementTakeDamage(LandedDamageData);

}

#pragma region GAS

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

UAbilitySystemComponent* AGameCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
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

bool AGameCharacter::IsAlive()
{
	if (IsValid(Attributes) && WasInitiatedAttributes)
	{
		return Attributes->GetHealth() > 0;
	}
	return true;
	
}
#pragma endregion 

#pragma region TakeDamage

bool AGameCharacter::IsInvulnerable()
{
	return bInvulnerable;
}

void AGameCharacter::ImplementTakeDamage(FDamageData DamageData)
{
	if (IsInvulnerable()) return;

	if (IsValid(DamageData.DamageGameplayEffect))
	{
		AbilitySystemComponent->ApplyGameplayEffectToSelf(DamageData.DamageGameplayEffect.GetDefaultObject(), 1.0, AbilitySystemComponent->MakeEffectContext());
	}
	AfterTakeDamage(DamageData);
}

void AGameCharacter::MakeMiss()
{
	for (AActor* & CheckingActor : DamageActors)
	{
		const auto CheckPawn = Cast<AGameCharacter>(CheckingActor);
		if (CheckPawn != nullptr)
		{
			CheckPawn->MissTakeDamage();
		}
	}
}

void AGameCharacter::OnDeath()
{
	AfterOnDeath();
}
#pragma endregion

#pragma region MakeDamage

void AGameCharacter::MakeStrike(float StrikeDistance, float MinAngle, float MaxAngle, bool IgnoreBlock, bool MakeStagger)
{
}

void AGameCharacter::ClearTempInternalActors()
{
	IgnoreActorsDamage.Empty();
	DamageActors.Empty();
	SuccessDamageCount = 0;
}

#pragma endregion
