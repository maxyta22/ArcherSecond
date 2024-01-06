// Archer Prototype. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractInterface.generated.h"


UINTERFACE(MinimalAPI)
class UInteractInterface : public UInterface
{
	GENERATED_BODY()
};

class ARCHERPRT_API IInteractInterface
{
	GENERATED_BODY()


public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interact")
	void I_Interact(APlayerCharacter* Pawn);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Description")
	void I_ShowInfo();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Description")
	void I_HideInfo();
};
