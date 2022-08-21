// Archer Prototype. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Environment/InteractObjectBase.h"
#include "BuildingComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARCHERPRT_API UBuildingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UBuildingComponent();

protected:
	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:	

	void BuildingMode();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AvaliableObjects")
		TArray<TSubclassOf<AInteractObjectBase>> AvaliableObjects;

private:

	bool bBuildingMode;
	

		
};
