// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once 

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "ArcherPRTHUD.generated.h"

UCLASS()
class AArcherPRTHUD : public AHUD
{
	GENERATED_BODY()

public:
	AArcherPRTHUD();

	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;

private:
	/** Crosshair asset pointer */
	class UTexture2D* CrosshairTex;

};

