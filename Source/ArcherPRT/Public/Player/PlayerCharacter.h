// Archer Prototype. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Player/GameCharacter.h"
#include "Components/InventoryComponent.h"
#include "Net/UnrealNetwork.h"
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
class UCraftComponent;


UCLASS(config = Game)
class ARCHERPRT_API APlayerCharacter : public AGameCharacter
{
	GENERATED_BODY()

public:

	APlayerCharacter();

	//Components

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		UCameraComponent* FirstPersonCameraComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Mesh)
		USkeletalMeshComponent* HandMesh;

	UPROPERTY(VisibleAnywhere, Category = "Interact Trigger")
		UCapsuleComponent* InteractCapsuleComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
		UInventoryComponent* InventoryComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Craft")
		UCraftComponent* CraftComponent;

	//Animation

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseTurnRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseLookUpRate;

	USkeletalMeshComponent* GetHandMesh() const { return HandMesh; }

	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

	UFUNCTION(Server, Reliable)
		void ServerOnOverlapBeginInteractCapsule(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(Server, Reliable)
		void ServerOnOverlapEndInteractCapsule(UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	//Blueprint Event for BP 

	UFUNCTION(BlueprintImplementableEvent, Category = "Attack")
		void PressedAttackButon();

	UFUNCTION(BlueprintImplementableEvent, Category = "Attack")
		void ReleasedAttackButton();

protected:

	virtual void BeginPlay() override;

	virtual void EventTick();

	void MoveForward(float Val);

	void MoveRight(float Val);

	void TurnAtRate(float Rate);

	void LookUpAtRate(float Rate);

	UFUNCTION(Server, Reliable)

	void ServerTryPerformInteract();
	
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;

	virtual void OnDeath() override;

private:

	TArray<AActor*> CurrentInteractTarget;


};
