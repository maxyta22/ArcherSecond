// Fill out your copyright notice in the Description page of Project Settings.


#include "Environment/InteractObjectBase.h"
#include "Components/SphereComponent.h"
#include "Player/PlayerCharacter.h"
#include "Net/UnrealNetwork.h"
#include "Components/InventoryComponent.h"

DEFINE_LOG_CATEGORY_STATIC(PickupBase, All, All);

AInteractObjectBase::AInteractObjectBase()
{
	SphereCollision = CreateDefaultSubobject<USphereComponent>("SphereCollision");
	SetRootComponent(SphereCollision);

	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMesh");
	SkeletalMeshComponent->SetupAttachment(SphereCollision);

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	StaticMeshComponent->SetupAttachment(SphereCollision);

	bReplicates = true;

}

void AInteractObjectBase::BeginPlay()
{
	Super::BeginPlay();
}

void AInteractObjectBase::TryUseInteractObject(APlayerCharacter* Pawn)
{
	AfterWasTryUseInteractObject();
}













