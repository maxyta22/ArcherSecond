// Archer Prototype. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "MakeStrikeANS.generated.h"

UCLASS()
class ARCHERPRT_API UMakeStrikeANS : public UAnimNotifyState
{
	GENERATED_BODY()

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StrikeSetting")
		float Distance = 200.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StrikeSetting")
		float MinAngle = -45.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StrikeSetting")
		float MaxAngle = 45.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool IgnoreBlock;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool MakeStagger;

private:
	
	bool ChargeSuccess;
	
};
