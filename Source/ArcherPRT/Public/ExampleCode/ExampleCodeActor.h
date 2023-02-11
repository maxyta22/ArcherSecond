// Archer Prototype. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ExampleCodeActor.generated.h"

UCLASS()
class ARCHERPRT_API AExampleCodeActor : public AActor
{
	GENERATED_BODY()
	
public:	
	
	AExampleCodeActor();

	//Example UPROPERTY

	/* "Specifiers"
	* BlueprintReadOnly
	* BlueprintReadWrite
	* VisibleAnywhehe
	* VisibleDefaultsOnly
	* VisibleInstanceOnly
	* EditAnywhere
	* EditDefaultsOnly
	* EditInstanceOnly
	* Category
	*/

	/*
	* "Important Meta Tags"
	* DisplayName = "Property Name" (Blueprint Displayed Name)
	* Tooltip = "This is a tooltip" (Blueprint Displayed Tooltip)
	* ClampMin = "N" (float and integers, min value that can be entered)
	* ClampMax = "N" (float and integers, max value that can be entered)
	* AllowPrivateAccess = "true|false"
	*/

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="ExampleVariable", DisplayName = "ChangeDisplayName", meta = (Tooltip = "This is a tooltip"))
		int ExampleVarEditAnywhere;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MainCategory|ExampleVariable")
		int ExampleVarEditDefaultsOnly;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "ExampleVariable")
		int ExampleVarVisibleAnywhere;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ExampleVariable", meta = (ClampMin = "5", ClampMax = "60"))
		int ExampleClamp;


private:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ExampleVariable", meta = (AllowPrivateAccess="true"))
		int ExampleAllowPrivateAccessVar;

public:

	//Example UFUNCTION

/* "Specifiers"
* BlueprintCallable
* BlueprintPure
* BlueprintImplementableEvent
* BlueprintNativeEvent
* Category
*/

/* "Important Meta Tags"
* DisplayName = "Property Name" (Blueprint Displayed Name)
* Tooltip = "Long Tooltip"
* ShortToolTip = "Short tooltip"
* HideSelfPin = true
*/

	UFUNCTION(BlueprintCallable, Category = "ExampleUfunctions")
		void ExampleBlueprintCallableFunction(int value);

	UFUNCTION(BlueprintPure, Category = "ExampleUfunctions", meta = (HideSelfPin= "true"))
		int ExampleBlueprintPureFunction() const;

	UFUNCTION(BlueprintImplementableEvent, Category = "ExampleUfunctions")
		void ExampleImplementableEventFunction(int value);

	UFUNCTION(BlueprintImplementableEvent, Category = "ExampleUfunctions")
		int ExampleImplementableEventReturnIntFunction(int value);

	UFUNCTION(BlueprintNativeEvent, Category = "ExampleUfunctions")
		void ExampleNativeEventFunction();

	UFUNCTION(BlueprintCallable, Category = "ExampleUfunctions")
		void ExampleInputOtputFunction(int InValue, FVector InVector, const FVector& constInVector, int& OutInt, bool& OutBool, FVector& OutVector);


	TArray<FVector> ExampleArray;
	void ExampleArrayOperation();





protected:
	
	virtual void BeginPlay() override;

public:	
	
	virtual void Tick(float DeltaTime) override;

};
