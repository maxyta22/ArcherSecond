// Archer Prototype. All rights reserved

#include "Components/BuildingComponent.h"
#include "Environment/InteractObjectBase.h"
#include "Player/PlayerCharacter.h"
#include "Craft/RecipeBase.h"


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
	if (!AvaliableRecipes[SelectedIndex]) return;
	if (!AvaliableRecipes[SelectedIndex].GetDefaultObject()->Object) return;

	//Init Spawn parameters
	const FVector SpawnLocation = Owner->GetActorLocation() + Owner->GetActorForwardVector()*SpawnOffset;
	const FRotator SpawnRotation = Owner->GetActorRotation();
	const TSubclassOf<AActor> SpawnActor = AvaliableRecipes[SelectedIndex].GetDefaultObject()->Object;

	if (!CurrentPreSpawnObject)
	{
		CurrentPreSpawnObject = World->SpawnActorDeferred<AInteractObjectBase>(SpawnActor, FTransform(SpawnRotation, SpawnLocation), nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
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
	if (!CurrentPreSpawnObject) return;
	if (!AvaliableRecipes[SelectedIndex]) return;
	if (!AvaliableRecipes[SelectedIndex].GetDefaultObject()->Object) return;

	const FVector SpawnLocation = CurrentPreSpawnObject->GetActorLocation();
	const FRotator SpawnRotation = CurrentPreSpawnObject->GetActorRotation();

	const TSubclassOf<AActor> SpawnActor = AvaliableRecipes[SelectedIndex].GetDefaultObject()->Object;
	const TSubclassOf<URecipeBase>  CurrentRecipe = AvaliableRecipes[SelectedIndex];
	const TMap<EResourcesType, int> NeededResources = CurrentRecipe.GetDefaultObject()->RecipeMap;

	
	if (Owner->InventoryComponent->LoopOnResourcesByMap(NeededResources))
	{
		Owner->InventoryComponent->LoopOnResourcesByMap(NeededResources, true, false);
		World->SpawnActorAbsolute(SpawnActor, FTransform(SpawnRotation, SpawnLocation));
	}
	
	
}

