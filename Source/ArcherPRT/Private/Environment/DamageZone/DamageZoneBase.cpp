// Archer Prototype. All rights reserved


#include "Environment/DamageZone/DamageZoneBase.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"



ADamageZoneBase::ADamageZoneBase()
{

	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>("SceneComponent");
	SetRootComponent(SceneComponent);
}

void ADamageZoneBase::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ADamageZoneBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DrawDebugSphere(GetWorld(), GetActorLocation(), Radius, 24, SphereColor);
	UGameplayStatics::ApplyRadialDamage(GetWorld(), Damage, GetActorLocation(), Radius, DamageType, {}, this, nullptr, DoFullDamage);

}
