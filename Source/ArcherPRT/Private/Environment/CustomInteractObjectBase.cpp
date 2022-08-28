// Archer Prototype. All rights reserved


#include "Environment/CustomInteractObjectBase.h"
#include "Core/ArcherPRTData.h"
#include "Components/InventoryComponent.h"
#include "Player/PlayerCharacter.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/ActorComponent.h"
#include "Components/BoxComponent.h"
#include "Components/ArrowComponent.h"


ACustomInteractObjectBase::ACustomInteractObjectBase()
{
	BoxGabaritCollision = CreateDefaultSubobject<UBoxComponent>("GabaritCollision");
	BoxGabaritCollision->SetupAttachment(PivotArrow);
	BoxGabaritCollision->SetBoxExtent(FVector::ZeroVector);
	BoxGabaritCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	BoxGabaritCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Overlap);
	BoxGabaritCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
	BoxGabaritCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
}

bool ACustomInteractObjectBase::CheckCanSpawnThisObject()
{  
	TArray<AActor*> OverlapingActors;	
	BoxGabaritCollision->GetOverlappingActors(OverlapingActors);
	
	if (OverlapingActors.Contains(this)) OverlapingActors.Remove(this);
	if (OverlapingActors.Contains(BuildingPlayer)) OverlapingActors.Remove(BuildingPlayer);
	
	return OverlapingActors.Num() == 0;
}

void ACustomInteractObjectBase::SetBuildingMaterial() 
{
	const auto BuildingMaterial = CheckCanSpawnThisObject() ? SuccessPreSpawnMaterial : FailPreSpawnMaterial;
	const auto Components = GetComponents();

	for (UActorComponent* CurrentComponent : Components)
	{
		UStaticMeshComponent* CurrentStaticMesh = Cast<UStaticMeshComponent>(CurrentComponent);
		if (CurrentStaticMesh)
		{
			for (int i = 0; i < CurrentStaticMesh->GetNumMaterials(); i++)
			{
				CurrentStaticMesh->SetMaterial(i, BuildingMaterial);
			}
		}

		USkeletalMeshComponent* CurrentSkeletalMesh = Cast<USkeletalMeshComponent>(CurrentComponent);
		if (CurrentSkeletalMesh)
		{
			for (int i = 0; i < CurrentSkeletalMesh->GetNumMaterials(); i++)
			{
				CurrentSkeletalMesh->SetMaterial(i, BuildingMaterial);
			}
		}
	}
}

void ACustomInteractObjectBase::TryUseInteractObject(APlayerCharacter* Pawn)
{
	Super::TryUseInteractObject(Pawn);

	if (!Pawn) return;

	if (ResoursesForInteract.Num()>0)
	{
		if (!Activate)
		{
			if (Pawn->InventoryComponent->LoopOnResourcesByMap(ResoursesForInteract))
			{
				Pawn->InventoryComponent->LoopOnResourcesByMap(ResoursesForInteract, true);
				Activate = true;
				AfterWasSuccessUseInteractObject();
			}
		}
		else
		{
			if (ReturnResources)
			{
				Pawn->InventoryComponent->LoopOnResourcesByMap(ResoursesForInteract, false, true);
			}
			Activate = false;
			AfterWasSuccessUseInteractObject();
		}

	}

	else 
	{
		AfterWasSuccessUseInteractObject();
	}
	
}
