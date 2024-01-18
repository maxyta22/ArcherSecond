// Archer Prototype. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Player/GameCharacter.h"
#include "Components/InventoryComponent.h"

#include "PlayerCharacter.generated.h"


class UInputComponent;
class USkeletalMeshComponent;
class USceneComponent;
class UCameraComponent;
class UMotionControllerComponent;
class UAnimMontage;
class USoundBase;
class UCapsuleComponent;
class UInventoryComponent;
class UBuildingComponent;
class UCraftComponent;
class UArrowComponent;
class USpringArmComponent;


UCLASS(config = Game)
class ARCHERPRT_API APlayerCharacter : public AGameCharacter
{
	GENERATED_BODY()

public:

	APlayerCharacter();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

#pragma region Components

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	UPROPERTY(VisibleAnywhere, Category = "Interact Trigger")
	UCapsuleComponent* InteractCapsuleComponent;

	UPROPERTY(VisibleAnywhere, Category = "Interact")
	UArrowComponent* InteractTraceDirection;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	UInventoryComponent* InventoryComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Craft")
	UCraftComponent* CraftComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building")
	UBuildingComponent* BuildingComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpringArm")
	USpringArmComponent* CameraSpringArm;



#pragma endregion

#pragma region Animation

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseTurnRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseLookUpRate;

	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

#pragma endregion

#pragma region Interact

public:

	UFUNCTION()
	void OnOverlapBeginInteractCapsule(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEndInteractCapsule(UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void CheckInteractObjects(bool TryInteract = false);

protected:

	void TryPerformInteract();

private:

	TArray<AActor*> CurrentInteractTarget;

#pragma endregion

#pragma region TakeDamage

protected:

	virtual void ImplementTakeDamage(FDamageData DamageData) override;

	virtual void OnDeath() override;

#pragma endregion

#pragma region MakeDamage

protected:

	virtual void MakeStrike(float StrikeDistance, float MinAngle, float MaxAngle, bool IgnoreBlock, bool MakeStagger) override;

#pragma endregion

#pragma region Events For BP

public:

	UFUNCTION(BlueprintImplementableEvent, Category = "Attack")
	void PressedAttackButon();

	UFUNCTION(BlueprintImplementableEvent, Category = "Attack")
	void ReleasedAttackButton();

	UFUNCTION(BlueprintImplementableEvent, Category = "Attack")
	void OnFire();

	UFUNCTION(BlueprintImplementableEvent, Category = "Attack")
	void TryFire();

	UFUNCTION(BlueprintImplementableEvent, Category = "Attack")
	void OnReloadWeapon();

	UFUNCTION(BlueprintImplementableEvent, Category = "Attack")
	void OnStartPerfectWindow();

	UFUNCTION(BlueprintImplementableEvent, Category = "Craft")
	void AfterBeginCraft();

	UFUNCTION(BlueprintImplementableEvent, Category = "Craft")
	void OnTryPerformInteract();

#pragma endregion

#pragma region Input

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	float InputMult = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	float  SensitivityMouse = 1.0f;

protected:

	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;

	void MoveForward(float Val);

	void MoveRight(float Val);

	//Gamepad
	void TurnAtRate(float Rate);

	void LookUpAtRate(float Rate);

	//Mouse
	virtual void AddControllerYawInput(float Val) override;

	virtual void AddControllerPitchInput(float Val) override;

#pragma endregion

};
