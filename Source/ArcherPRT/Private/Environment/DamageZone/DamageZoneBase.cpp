// Archer Prototype. All rights reserved


#include "Environment/DamageZone/DamageZoneBase.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Player/GameCharacter.h"
#include "GameplayEffect.h"



ADamageZoneBase::ADamageZoneBase()
{

	PrimaryActorTick.bCanEverTick = false;

	SceneComponent = CreateDefaultSubobject<USceneComponent>("SceneComponent");
	SetRootComponent(SceneComponent);
	BoxCollision = CreateDefaultSubobject<UBoxComponent>("BoxCollisison");
	BoxCollision->SetupAttachment(RootComponent);
	SphereCollision = CreateDefaultSubobject<USphereComponent>("SphereCollision");
	SphereCollision->SetupAttachment(RootComponent);
	CapsuleCollision = CreateDefaultSubobject<UCapsuleComponent>("CapsuleCollision");
	CapsuleCollision->SetupAttachment(RootComponent);

	//Bind

	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &ADamageZoneBase::OnOverlapBegin);
	BoxCollision->OnComponentEndOverlap.AddDynamic(this, &ADamageZoneBase::OnOverlapEnd);

	SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &ADamageZoneBase::OnOverlapBegin);
	SphereCollision->OnComponentEndOverlap.AddDynamic(this, &ADamageZoneBase::OnOverlapEnd);

	CapsuleCollision->OnComponentBeginOverlap.AddDynamic(this, &ADamageZoneBase::OnOverlapBegin);
	CapsuleCollision->OnComponentEndOverlap.AddDynamic(this, &ADamageZoneBase::OnOverlapEnd);


}

void ADamageZoneBase::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (IsValid(OtherActor))
	{
		AGameCharacter* LCharacter = Cast<AGameCharacter>(OtherActor);
		if (IsValid(LCharacter))
		{
			DamageTarget = LCharacter;
			MakeDamageByTarget();
			GetWorld()->GetTimerManager().SetTimer(DamageTimer, this, &ADamageZoneBase::MakeDamageByTarget, DamageFrequency, true);
		}
	}
}

void ADamageZoneBase::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (IsValid(OtherActor) && IsValid(DamageTarget) && OtherActor == DamageTarget)
	{
		DamageTarget = nullptr;
		GetWorld()->GetTimerManager().ClearTimer(DamageTimer);
	}
}

void ADamageZoneBase::MakeDamageByTarget()
{
	if (IsValid(DamageTarget))
	{
		AGameCharacter* gameChar = Cast<AGameCharacter>(DamageTarget);
		if (IsValid(gameChar))
		{
			FDamageData  damageData;
			damageData.DamageDirection = GetActorForwardVector();
			damageData.DamageInstigator = GetInstigatorController();
			damageData.DamageCauser = this;
			damageData.DamageGameplayEffect = DamageGameplayEffect;

			gameChar->ImplementTakeDamage(damageData);
		}
		
	}
}
