// Archer Prototype. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "PRTAIController.generated.h"

class UPRTAIPerceptionComponent;
class AGameCharacter;

UCLASS()
class ARCHERPRT_API APRTAIController : public AAIController
{
	GENERATED_BODY()

public:
	APRTAIController();

	void LockBehavior(bool Lock);

	void SetEnemy(AActor* Enemy);

	void FocusService();

	UFUNCTION(BlueprintPure)
	AActor* GetEnemy();

	bool FocusActive = true;
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		UPRTAIPerceptionComponent* PRTAIPerceptionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
		FName FocusOnKeyName = "EnemyActor";

private:

	AActor* GetFocusOnActor() const;
	AGameCharacter* ControlledPawn;

	virtual void OnPossess(APawn* InPawn) override;
	virtual void Tick(float DeltaTime) override;
	
};
