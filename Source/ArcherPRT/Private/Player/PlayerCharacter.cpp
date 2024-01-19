// Archer Prototype. All rights reserved.


#include "Player/PlayerCharacter.h"
#include "Projectile/ArcherPRTProjectile.h"
#include "AI/AICharacter.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "MotionControllerComponent.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Actor.h"
#include "GameplayAbilitySystem/PRTAbilitySystemComponent.h"
#include "GameplayAbilitySystem/PRTAttributeSet.h"
#include "GameplayAbilitySystem/PRTGameplayAbility.h"
#include "Environment/InteractObjectBase.h"
#include "Components/CraftComponent.h"
#include "Components/InventoryComponent.h"
#include "Components/BuildingComponent.h"
#include "Components/WeaponComponent.h"
#include "Weapon/WeaponBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Interfaces/TakeDamageInterface.h"
#include "Interfaces/InteractInterface.h"
#include "Components/ArrowComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogPlayerCharacter, Warning, All);

APlayerCharacter::APlayerCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	//FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetupAttachment(GetMesh(), "camera_socket");
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true; // Set false If Use Camera Animation

	//Create InteractCapsule
	InteractCapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("InteractCapsule"));
	InteractCapsuleComponent->SetupAttachment(FirstPersonCameraComponent);

	InteractCapsuleComponent->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::OnOverlapBeginInteractCapsule);
	InteractCapsuleComponent->OnComponentEndOverlap.AddDynamic(this, &APlayerCharacter::OnOverlapEndInteractCapsule);

	//Create InteractTraceDirection
	InteractTraceDirection = CreateDefaultSubobject<UArrowComponent>(TEXT("InteractTraceDirection"));
	InteractTraceDirection->SetupAttachment(FirstPersonCameraComponent);

	//Create InventoryComponent
	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));

	//Create CraftComponent
	CraftComponent = CreateDefaultSubobject<UCraftComponent>(TEXT("CraftComponent"));

	//Create BuildingComponent
	BuildingComponent = CreateDefaultSubobject<UBuildingComponent>(TEXT("BuildingComponent"));

}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	CheckInteractObjects();
}

#pragma region Input

void APlayerCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	
	check(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &APlayerCharacter::PressedAttackButon);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, WeaponComponent, &UWeaponComponent::OnFire);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &APlayerCharacter::ReleasedAttackButton);
	PlayerInputComponent->BindAction("Fire", IE_Released, WeaponComponent, &UWeaponComponent::TryFire);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, BuildingComponent, &UBuildingComponent::TrySpawnObject);
	PlayerInputComponent->BindAction("AltFire", IE_Pressed, WeaponComponent, &UWeaponComponent::OnAltFire);
	PlayerInputComponent->BindAction("AltFire", IE_Released, WeaponComponent, &UWeaponComponent::FinishAltFire);
	PlayerInputComponent->BindAction("Reloading", IE_Pressed, WeaponComponent, &UWeaponComponent::TryReloadWeapon);
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &APlayerCharacter::TryPerformInteract);
	//PlayerInputComponent->BindAction("TryCraftItem", IE_Pressed, CraftComponent, &UCraftComponent::TryCraftItem);
	PlayerInputComponent->BindAction("StartBuilding", IE_Pressed, BuildingComponent, &UBuildingComponent::ToggleBuildingMode);
	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &APlayerCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &APlayerCharacter::LookUpAtRate);

	if (AbilitySystemComponent && InputComponent)
	{
		const FGameplayAbilityInputBinds Binds("Confirm", "Cancel", "EAbilityInputID", static_cast<int32>(EAbilityInputID::Confirm), static_cast<int32>(EAbilityInputID::Cancel));
		AbilitySystemComponent->BindAbilityActivationToInputComponent(InputComponent, Binds);
	}
	
}

void APlayerCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		AddMovementInput(GetActorForwardVector(), Value * InputMult);
	}
}

void APlayerCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		AddMovementInput(GetActorRightVector(), Value * InputMult);
	}
}

void APlayerCharacter::TurnAtRate(float Rate)
{
	if (GetWorld() == nullptr) return;
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void APlayerCharacter::LookUpAtRate(float Rate)
{
	if (GetWorld() == nullptr) return;
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void APlayerCharacter::AddControllerYawInput(float Val)
{
	Super::AddControllerYawInput(Val * SensitivityMouse);
}

void APlayerCharacter::AddControllerPitchInput(float Val)
{
	Super::AddControllerPitchInput(Val * SensitivityMouse);
}

#pragma endregion

#pragma region Interact

void APlayerCharacter::OnOverlapBeginInteractCapsule(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	if (IsValid(OtherActor) && (OtherActor != this))
	{
		CurrentInteractTarget.Add(OtherActor);
	}
}

void APlayerCharacter::OnOverlapEndInteractCapsule(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

	if (IsValid(OtherActor))
	{
		CurrentInteractTarget.Remove(OtherActor);
	}
}

void APlayerCharacter::CheckInteractObjects(bool TryInteract)
{
	if (GetWorld() == nullptr) return;
	TArray<AActor*> HitActors;
	TArray<FHitResult> HitResult;
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectsTypeQuery;
	ObjectsTypeQuery.Add(UEngineTypes::ConvertToObjectType(ECC_WorldStatic));
	ObjectsTypeQuery.Add(UEngineTypes::ConvertToObjectType(ECC_WorldDynamic));
	TArray<AActor*, FDefaultAllocator> IgnoreActors;

	const auto StartTrace = InteractTraceDirection->GetComponentLocation();
	const auto EndTrace = StartTrace + (InteractTraceDirection->GetForwardVector() * 200);
	UKismetSystemLibrary::SphereTraceMultiForObjects(GetWorld(), StartTrace, EndTrace, 10, ObjectsTypeQuery, true, IgnoreActors, EDrawDebugTrace::None, HitResult, true, FLinearColor::Green, FLinearColor::Red, 5.0);

	if (HitResult.Num() == 0) return;

	for (FHitResult CheckResult : HitResult)
	{
		HitActors.AddUnique(CheckResult.GetActor());
	}

	for (AActor* CheckActor : HitActors)
	{

		if (CheckActor->GetClass()->ImplementsInterface(UInteractInterface::StaticClass()))
		{
			if (TryInteract)
			{
				IInteractInterface::Execute_I_Interact(CheckActor, this);
			}
			IInteractInterface::Execute_I_ShowInfo(CheckActor);
		}
	}
}

void APlayerCharacter::TryPerformInteract()
{
	OnTryPerformInteract();
	CheckInteractObjects(true);
}

#pragma endregion

#pragma region TakeDamage

void APlayerCharacter::ImplementTakeDamage_Implementation(FDamageData DamageData)
{
	Super::ImplementTakeDamage_Implementation(DamageData);
}

void APlayerCharacter::OnDeath()
{
	Super::OnDeath();
	GetCharacterMovement()->DisableMovement();
}

#pragma endregion

#pragma region MakeDamage

void APlayerCharacter::MakeStrike(float StrikeDistance, float MinAngle, float MaxAngle, bool IgnoreBlock, bool MakeStagger)
{
	if (GetWorld() == nullptr) return;

	StrikeInProgress();

	FVector StartTrace = GetFirstPersonCameraComponent()->GetComponentLocation() + GetFirstPersonCameraComponent()->GetForwardVector() * 50;
	FVector EndTrace = StartTrace + GetFirstPersonCameraComponent()->GetForwardVector() * WeaponComponent->GloveAttackDistance;

	TArray < TEnumAsByte < EObjectTypeQuery > >  ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Destructible));
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_WorldStatic));
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_WorldDynamic));
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));

	TArray < AActor* > ActorsToIgnore;
	ActorsToIgnore.Add(this);

	TArray < FHitResult > OutHits;

	UKismetSystemLibrary::SphereTraceMultiForObjects(GetWorld(), StartTrace, EndTrace, WeaponComponent->GloveAttackRadius, ObjectTypes, true, ActorsToIgnore, EDrawDebugTrace::None, OutHits, true, FLinearColor::Red, FLinearColor::Green, 0.5f);

	FDamageData damageData;
	damageData.DamageGameplayEffect = WeaponComponent->CurrentEquipWeapon.GetDefaultObject()->WeaponGameplayEffect;
	damageData.PerfectDamageGameplayEffect = WeaponComponent->CurrentEquipWeapon.GetDefaultObject()->PerfectDamageGE;
	damageData.DamageWeaponType = WeaponComponent->CurrentEquipWeapon.GetDefaultObject()->WeaponType;
	damageData.DamageInstigator = GetInstigatorController();
	damageData.DamageCauser = this;
	damageData.DamageCharged = WeaponComponent->bWeaponCharged;
	damageData.PerfectDamage = WeaponComponent->bPerfectDamage;

	for (FHitResult& HitResult : OutHits)
	{
		damageData.DamageDirection = GetActorForwardVector();
		damageData.DamagePoint = HitResult;

		if (HitResult.bBlockingHit)
		{
			if (HitResult.GetActor() != nullptr && HitResult.GetComponent() != nullptr && !IgnoreActorsDamage.Contains(HitResult.GetActor()))
			{
				AAICharacter* Pawn = Cast<AAICharacter>(HitResult.GetActor());
				
				if (IsValid(Pawn) && Pawn->IsInvulnerable() == false)
				{
					DamageActors.AddUnique(Pawn);

					if (MakeStagger)
					{
						Pawn->PerformOnStaggerReaction();
						IgnoreActorsDamage.Add(Pawn);
						AddSuccessDamageCount();
					}
					else 
					{
						if (HitResult.GetComponent()->ComponentHasTag("WeakPoint"))
						{
							Pawn->ImplementTakeDamage(damageData);
							AddSuccessDamageCount();
							IgnoreActorsDamage.Add(Pawn);
						}
					}
					ActorsToIgnore.Add(Pawn);
				}

				if (HitResult.GetActor()->GetClass()->ImplementsInterface(UTakeDamageInterface::StaticClass()))
				{
					ITakeDamageInterface::Execute_I_TakeDamage(HitResult.GetActor(), damageData);
					ActorsToIgnore.Add(HitResult.GetActor());
					IgnoreActorsDamage.Add(HitResult.GetActor());
				}
			}
		}
	}

}

#pragma endregion


