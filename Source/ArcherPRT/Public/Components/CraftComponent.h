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

	
	void TryCraftItem();

protected:
	
	virtual void BeginPlay() override;

private:

	bool CheckRecipe(TSubclassOf<URecipeBase> Recipe, bool SpendResources);
	void GetRecipeResult(TSubclassOf<URecipeBase> Recipe);
	
	
		
};
