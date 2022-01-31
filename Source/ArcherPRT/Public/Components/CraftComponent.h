// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CraftComponent.generated.h"

class URecipeBase;


UCLASS(ClassGroup = (Custom), blueprinttype, blueprintable, meta = (BlueprintSpawnableComponent))
class ARCHERPRT_API UCraftComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	
	UCraftComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recipe database")
		TArray<TSubclassOf<URecipeBase>> RecipeDataBase;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Internal")
		int SelectedIndex = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Craft Setting")
		float TimeCraft = 5;

	UFUNCTION(BlueprintPure)
		bool CraftInProgress() { return  CraftInProgressTimer.IsValid(); }

	UFUNCTION(BlueprintPure)
		float GetCraftTimeRemaining();

	void TryCraftItem();

	void AbortCraftProcess();

	void CraftSucceess();

protected:
	
	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:

	bool bCraftInPrigress;

	FTimerHandle CraftInProgressTimer;

	bool CheckRecipe(TSubclassOf<URecipeBase> Recipe, bool SpendResources);
	void GetRecipeResult(TSubclassOf<URecipeBase> Recipe);
	
	
		
};
