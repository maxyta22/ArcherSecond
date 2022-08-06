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
#include "XRMotionControllerBase.h" // for FXRMotionControllerBase::RightHandSourceId
#include "Environment/InteractObjectBase.h"
#include "Components/CraftComponent.h"
#include "Components/InventoryComponent.h"
#include "Components/WeaponComponent.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/CharacterMovementComponent.h"

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
	FirstPersonCameraComponent->SetupAttachment(GetMesh());
	//FirstPersonCameraComponent->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	//Create InteractCapsule
	InteractCapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("InteractCapsule"));
	InteractCapsuleComponent->SetupAttachment(FirstPersonCameraComponent);

	InteractCapsuleComponent->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::ServerOnOverlapBeginInteractCapsule_ServerRPC);
	InteractCapsuleComponent->OnComponentEndOverlap.AddDynamic(this, &APlayerCharacter::ServerOnOverlapEndInteractCapsule_ServerRPC);

	//Create InventoryComponent
	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));

	//Create CraftComponent
	CraftComponent = CreateDefaultSubobject<UCraftComponent>(TEXT("CraftComponent"));

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	HandMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	HandMesh->SetOnlyOwnerSee(true);
	HandMesh->SetupAttachment(FirstPersonCameraComponent);
	HandMesh->bCastDynamicShadow = false;
	HandMesh->CastShadow = false;
	HandMesh->SetRelativeRotation(FRotator(1.9f, -19.19f, 5.2f));
	HandMesh->SetRelativeLocation(FVector(-0.5f, -4.4f, -155.7f));
}

void APlayerCharacter::BeginPlay()
{
	
	Super::BeginPlay();

}

void APlayerCharacter::EventTick()
{
}

void APlayerCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	
	check(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, WeaponComponent, &UWeaponComponent::OnAiming_ServerRPC);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &APlayerCharacter::PressedAttackButon);
	PlayerInputComponent->BindAction("Fire", IE_Released, WeaponComponent, &UWeaponComponent::OnFire_ServerRPC);
	PlayerInputComponent->BindAction("Fire", IE_Released, WeaponComponent, &UWeaponComponent::OffAiming_ServerRPC);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &APlayerCharacter::ReleasedAttackButton);
	PlayerInputComponent->BindAction("SwitchAmmo", IE_Pressed, WeaponComponent, &UWeaponComponent::SwitchAmmoInCurrentEquipWeapon);
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &APlayerCharacter::TryPerformInteract_ServerRPC);
	PlayerInputComponent->BindAction("TryCraftItem", IE_Pressed, CraftComponent, &UCraftComponent::TryCraftItem);
	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &APlayerCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &APlayerCharacter::LookUpAtRate);
	
}

void APlayerCharacter::OnDeath()
{
	Super::OnDeath();
	GetCharacterMovement()->DisableMovement();	
}

void APlayerCharacter::MoveForward(float Value)
{
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
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void APlayerCharacter::LookUpAtRate(float Rate)
{
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void APlayerCharacter::TryPerformInteract_ServerRPC_Implementation()
{
	AInteractObjectBase* InteractObject;
	
	
	if (CurrentInteractTarget.Num() == 0) return;
	
	InteractObject = Cast<AInteractObjectBase>(CurrentInteractTarget[0]);
	if (InteractObject)
		{
			InteractObject->TryUseInteractObject(this);
			return;
		}

	AArcherPRTProjectile* Projectile;
	Projectile = Cast<AArcherPRTProjectile>(CurrentInteractTarget[0]);
	if (Projectile)
		{
			Projectile->TryTakeProjectile_ServerRPC(this);
			return;
		}
}

void APlayerCharacter::ServerOnOverlapBeginInteractCapsule_ServerRPC_Implementation(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		ShowInfoObject_ClientRPC(OtherActor);
		CurrentInteractTarget.Add(OtherActor);
	}
}

void APlayerCharacter::ServerOnOverlapEndInteractCapsule_ServerRPC_Implementation(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

	if (GEngine)
	{
		HideInfoObject_ClientRPC(OtherActor);
		CurrentInteractTarget.Remove(OtherActor);
	}
}

void APlayerCharacter::ShowInfoObject_ClientRPC_Implementation(AActor* InfoObject)
{
	if (GEngine)
	{
		AInteractObjectBase* InteractObject;
		InteractObject = Cast<AInteractObjectBase>(InfoObject);
		AArcherPRTProjectile* Projectile;
		Projectile = Cast<AArcherPRTProjectile>(InfoObject);
		if (InteractObject)
		{
			InteractObject->ShowInfo();
		}
		if (Projectile)
		{
			Projectile->ShowInfo();
		}
	}
}

void APlayerCharacter::HideInfoObject_ClientRPC_Implementation(AActor* InfoObject)
{
	if (GEngine)
	{
		AInteractObjectBase* InteractObject;
		InteractObject = Cast<AInteractObjectBase>(InfoObject);
		AArcherPRTProjectile* Projectile;
		Projectile = Cast<AArcherPRTProjectile>(InfoObject);
		if (InteractObject)
		{
			InteractObject->HideInfo();
		}
		if (Projectile)
		{
			Projectile->HideInfo();
		}
	}
}


