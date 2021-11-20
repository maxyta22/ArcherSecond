// Archer Prototype. All rights reserved


#include "ExampleCode/ExampleCodeActor.h"

AExampleCodeActor::AExampleCodeActor()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AExampleCodeActor::ExampleBlueprintCallableFunction(int value)
{
	UE_LOG(LogTemp, Warning, TEXT("Return function %d"), value);
}

int AExampleCodeActor::ExampleBlueprintPureFunction() const
{
	return 2 + 2 * 2;
}

void AExampleCodeActor::ExampleNativeEventFunction_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Call ExampleNativeFunction"));
}

void AExampleCodeActor::ExampleInputOtputFunction(int InValue, FVector InVector, const FVector& constInVector, int& OutInt, bool& OutBool, FVector& OutVector)
{
	OutInt = InValue;
}

void AExampleCodeActor::BeginPlay()
{
	Super::BeginPlay();
	ExampleNativeEventFunction();
	ExampleArrayOperation();
}

void AExampleCodeActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AExampleCodeActor::ExampleArrayOperation()
{
	//Array add Elements / Arrays

	TArray<FVector> LArray;

	ExampleArray.Add(FVector(3, 4, 2));
	ExampleArray.Insert(FVector::ZeroVector, 1);
	ExampleArray.Append(LArray);

	//Array num elements / Get

	const int NumElements = ExampleArray.Num();
	const int ArrayLastIndex = ExampleArray.Num() - 1;
	FVector ArrayElement = ExampleArray[0];				//Create Copy 
	const FVector& RefArrayElement = ExampleArray[0];	//Only Read reference

	//Array find / Contains

	//Contains
	if (ExampleArray.Contains(FVector(3,4,2)))
	{
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Green, TEXT("Contains Element"));
	}

	//Find var 1
	const int Index = ExampleArray.Find(FVector(3,4,2));
	if (Index != INDEX_NONE)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Green, TEXT("Find Element var 1"));
	}

	//Find var 2
	int IndexForFind;
	if (ExampleArray.Find(FVector(3, 4, 2), IndexForFind))
	{
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Green, TEXT("Find Element var 2"));
	}

	//Array Remove / Clear

	ExampleArray.Remove(FVector(3, 4, 2));	//Remove element by content
	//ExampleArray.RemoveAt(1);				//Remove Element by index		
	ExampleArray.Empty();					//Full clear array

	//Array Iteration

	//for loop 
	const int Num = ExampleArray.Num();
	for (int i = 0; i < Num; i++)
	{
		ExampleArray[i];
	}

	//range-based for each loop 1
	for (FVector& Vec : ExampleArray)
	{
		float val = Vec.X;
	}

	//for witch iterator
	for (auto It = ExampleArray.CreateIterator(); It; ++It)
	{
		FVector& Vec = *It;
		float Val = Vec.X;
	}










}