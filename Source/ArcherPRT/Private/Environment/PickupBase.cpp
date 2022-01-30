// Fill out your copyright notice in the Description page of Project Settings.


#include "Environment/PickupBase.h"
#include "Components/SphereComponent.h"
#include "Player/PlayerCharacter.h"
#include "Net/UnrealNetwork.h"
#include "Components/InventoryComponent.h"

DEFINE_LOG_CATEGORY_STATIC(PickupBase, All, All);

APickupBase::APickupBase()
{
	SphereCollision = CreateDefaultSubobject<USphereComponent>("SphereCollision");
	SetRootComponent(SphereCollision);

	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMesh");
	SkeletalMeshComponent->SetupAttachment(SphereCollision);

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	StaticMeshComponent->SetupAttachment(SphereCollision);

	bReplicates = true;

}

void APickupBase::ShowInfo_Implementation()
{
}

void APickupBase::HideInfo_Implementation()
{
}

void APickupBase::BeginPlay()
{
	Super::BeginPlay();
}

void APickupBase::TryTakePickup_Server_Implementation(APlayerCharacter* Pawn)
{
	 
}


void APickupBase::TakePickup_Server_Implementation()
{
	AfterWasTaken();
	Destroy();
}











