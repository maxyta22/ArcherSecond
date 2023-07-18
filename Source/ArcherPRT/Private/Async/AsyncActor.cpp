// Archer Prototype. All rights reserved


#include "Async/AsyncActor.h"

// Sets default values
AAsyncActor::AAsyncActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AAsyncActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAsyncActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

