// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ArcherPRTGameMode.generated.h"

UCLASS(minimalapi)
class AArcherPRTGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:

	AArcherPRTGameMode();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Sound")
		void ToggleSoundMode (bool BattleMode);

	UFUNCTION(BlueprintCallable, Category = "Sound")
		bool InBattleMode();

};



