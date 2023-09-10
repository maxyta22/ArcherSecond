// Archer Prototype. All rights reserved.


#include "Player/PlayerCharacter.h"
#include "Projectile/ArcherPRTProjectile.h"
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

void APlayerCharacter::OnDeath()
{
	Super::OnDeath();
	GetCharacterMovement()->DisableMovement();	
}

float APlayerCharacter::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	WeaponComponent->FinishFire(true);
	AfterTakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	return Damage;
}

void APlayerCharacter::MakeStrike(float StrikeDistance, float MinAngle, float MaxAngle, bool IgnoreBlock)
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

	const float Damage = WeaponComponent->bWeaponCharged ?
		WeaponComponent->CurrentEquipWeapon.GetDefaultObject()->ChargeDamage :
		WeaponComponent->CurrentEquipWeapon.GetDefaultObject()->Damage;
	const auto WeaponType = WeaponComponent->CurrentEquipWeapon.GetDefaultObject()->WeaponType;
	const bool Charged = WeaponComponent->bWeaponCharged;

	for (FHitResult& HitResult : OutHits)
	{
		if (HitResult.bBlockingHit)
		{
			if (HitResult.GetActor() != nullptr && HitResult.GetComponent() != nullptr && !IgnoreActorsDamage.Contains(HitResult.GetActor()))
			{
				const auto Pawn = Cast<AGameCharacter>(HitResult.GetActor());
				const auto InteractObject = Cast<AInteractObjectBase>(HitResult.GetActor());

				if (Pawn !=  nullptr && Pawn->IsInvulnerable() == false)
				{		
					DamageActors.AddUnique(Pawn);

					if (HitResult.GetComponent()->ComponentHasTag("WeakPoint"))
					{
						Pawn->TakeDamage(Damage, FDamageEvent(), GetInstigatorController(), this);
						Pawn->OnHit(GetActorForwardVector(), HitResult, this, WeaponType, Charged);
						SuccessDamageCount++;
						IgnoreActorsDamage.Add(Pawn);
					}
					ActorsToIgnore.Add(Pawn);
				}
				if (InteractObject)
				{
					InteractObject->AfterGloveHit(WeaponComponent->bWeaponCharged, HitResult, this);
					IgnoreActorsDamage.Add(InteractObject);
				}
			}
		}
	}

}

void APlayerCharacter::MoveForward(float Value)
{
	if (bLockMoveForward)
		return;

	if (Value != 0.0f)
	{
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void APlayerCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		AddMovementInput(GetActorRightVector(), Value);
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

void APlayerCharacter::OnOverlapBeginInteractCapsule(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		ShowInfoObject(OtherActor);
		CurrentInteractTarget.Add(OtherActor);
	}
}

void APlayerCharacter::OnOverlapEndInteractCapsule(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

	if (GEngine)
	{
		HideInfoObject(OtherActor);
		CurrentInteractTarget.Remove(OtherActor);
	}
}

void APlayerCharacter::ShowInfoObject(AActor* InfoObject)
{
	return;
}

void APlayerCharacter::HideInfoObject(AActor* InfoObject)
{
	return;
}

void APlayerCharacter::CheckInteractObjects(bool TryInteract)
{
	if (GetWorld() == nullptr) return;
	AInteractObjectBase* InteractObject;
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

	for (size_t i = 0; i < HitResult.Num(); i++)
	{
		HitActors.AddUnique(HitResult[i].GetActor());
	}

	for (size_t i = 0; i < HitActors.Num(); i++)
	{
		InteractObject = Cast<AInteractObjectBase>(HitActors[i]);
		if (InteractObject)
		{
			if (TryInteract)
			{
				InteractObject->TryUseInteractObject(this);
			}

			InteractObject->ShowInfo();
			
		}

	}
}

void APlayerCharacter::TryPerformInteract()
{
	OnTryPerformInteract();
	CheckInteractObjects(true);
}


