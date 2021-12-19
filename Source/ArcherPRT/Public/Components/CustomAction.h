// Archer Prototype. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CustomAction.generated.h"

class AGameCharacter;


UCLASS( ClassGroup=(Custom), blueprinttype, blueprintable, meta=(BlueprintSpawnableComponent) )
class ARCHERPRT_API UCustomAction : public UActorComponent
{
	GENERATED_BODY()

public:	
	
	UCustomAction();

	UFUNCTION(Server, Reliable, BlueprintCallable,Category = "Custom Action")
		void TryPerformPlayAnimMontage_Server(UAnimMontage* Montage, bool CanInterruptCurrentMontage);

	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category = "Custom Action")
		void TryPerformPlayAnimMontage_Multicast(UAnimMontage* Montage, bool CanInterruptCurrentMontage);

	UFUNCTION(Server, Reliable)
		void FinishCustomAction_Server();

	UFUNCTION(NetMulticast, Reliable)
		void FinishCustomAction_Multicast();

	UFUNCTION(BlueprintPure)
		bool CustomActionInProgress() { return  bCustomActionInProgress; }

	UFUNCTION(BlueprintPure)
		UAnimMontage* GetLastAnimMontage() { return  LastAnimMontage; }

	


protected:
	
	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:


	FTimerHandle FinishCustomActionTimer;

	bool bCustomActionInProgress;

	UAnimMontage* LastAnimMontage;
	
	

		
};
