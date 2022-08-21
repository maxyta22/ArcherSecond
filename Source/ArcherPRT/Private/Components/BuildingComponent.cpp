// Archer Prototype. All rights reserved


#include "Components/BuildingComponent.h"


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
	}
}

void UBuildingComponent::BuildingMode()
{
	bBuildingMode = !bBuildingMode;
}

