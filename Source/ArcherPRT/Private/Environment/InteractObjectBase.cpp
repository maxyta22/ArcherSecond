// Fill out your copyright notice in the Description page of Project Settings.


#include "Environment/InteractObjectBase.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Player/PlayerCharacter.h"
#include "Net/UnrealNetwork.h"
#include "Components/ArrowComponent.h"
#include "Components/InventoryComponent.h"

DEFINE_LOG_CATEGORY_STATIC(PickupBase, All, All);

AInteractObjectBase::AInteractObjectBase()
{
	PivotArrow = CreateDefaultSubobject<UArrowComponent>("ArrowComponent");
	SetRootComponent(PivotArrow);

	SphereCollision = CreateDefaultSubobject<USphereComponent>("SphereCollision");
	SphereCollision->SetupAttachment(PivotArrow);

	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMesh");
	SkeletalMeshComponent->SetupAttachment(PivotArrow);

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	StaticMeshComponent->SetupAttachment(PivotArrow);

	
	PrimaryActorTick.bStartWithTickEnabled = false;
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.SetTickFunctionEnable(false);

}

void AInteractObjectBase::BeginPlay()
{
	Super::BeginPlay();
}

void AInteractObjectBase::TryUseInteractObject(APlayerCharacter* Pawn)
{
	AfterWasTryUseInteractObject();
}

void AInteractObjectBase::I_Interact_Implementation(APlayerCharacter* Pawn)
{
	TryUseInteractObject(Pawn);
}

void AInteractObjectBase::I_ShowInfo_Implementation()
{
	ShowInfo();
}

void AInteractObjectBase::I_HideInfo_Implementation()
{
	HideInfo();
}













