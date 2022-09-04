// Archer Prototype. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "RotationOnTargetANS.generated.h"


UCLASS()
class ARCHERPRT_API URotationOnTargetANS : public UAnimNotifyState
{
	GENERATED_BODY()
	
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime) override;
	
};
