// Archer Prototype. All rights reserved

#include "Components/BuildingComponent.h"
#include "Environment/InteractObjectBase.h"
#include "Player/PlayerCharacter.h"


UBuildingComponent::UBuildingComponent()
{

	PrimaryComponentTick.bCanEverTick = true;

}

void UBuildingComponent::BeginPlay()
{

}

void UBuildingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	if (bBuildingMode)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Green, TEXT("BuildingMode"));
		PreSpawnObject();
	}
}

void UBuildingComponent::ToggleBuildingMode()
{
	bBuildingMode = !bBuildingMode;

	if (!bBuildingMode)
	{
		CurrentPreSpawnObject->Destroy();
		CurrentPreSpawnObject = nullptr;
	}
}

void UBuildingComponent::PreSpawnObject()
{
	UWorld* const World = GetWorld();
	if (!World) return;
	const auto Owner = Cast<APlayerCharacter>(GetOwner());
	if (!Owner) return;
	if (!AvaliableObjects[0]) return;

	//Init Spawn parameters
	const FVector SpawnLocation = Owner->GetActorLocation() + Owner->GetActorForwardVector()*SpawnOffset;
	const FRotator SpawnRotation = Owner->GetActorRotation();

	if (!CurrentPreSpawnObject)
	{
		CurrentPreSpawnObject = World->SpawnActorDeferred<AInteractObjectBase>(AvaliableObjects[0], FTransform(SpawnRotation, SpawnLocation),nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		if (CurrentPreSpawnObject)
		{
			CurrentPreSpawnObject->FinishSpawning(FTransform(SpawnRotation, SpawnLocation));
		}
	}

	
	CurrentPreSpawnObject->SetActorLocationAndRotation(SpawnLocation, SpawnRotation);
	


}

void UBuildingComponent::TrySpawnObject()
{
	UWorld* const World = GetWorld();
	if (!World) return;
	const auto Owner = Cast<APlayerCharacter>(GetOwner());
	if (!Owner) return;
	if (!AvaliableObjects[0]) return;
	if (!CurrentPreSpawnObject) return;

	const FVector SpawnLocation = CurrentPreSpawnObject->GetActorLocation();
	const FRotator SpawnRotation = CurrentPreSpawnObject->GetActorRotation();

	World->SpawnActorAbsolute(AvaliableObjects[0], FTransform(SpawnRotation, SpawnLocation));
	
}

