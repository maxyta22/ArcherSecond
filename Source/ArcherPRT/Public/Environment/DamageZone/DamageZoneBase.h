// Archer Prototype. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DamageZoneBase.generated.h"

UCLASS()
class ARCHERPRT_API ADamageZoneBase : public AActor
{
	GENERATED_BODY()

public:

	ADamageZoneBase();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		USceneComponent* SceneComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Radius = 300.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FColor SphereColor = FColor::Red;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Damage = 0.1f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool DoFullDamage = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<UDamageType> DamageType;

protected:

	virtual void BeginPlay() override;

public:

	virtual void Tick(float DeltaTime) override;

};

