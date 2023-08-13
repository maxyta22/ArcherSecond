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

	UFUNCTION(BlueprintPure, Category = "Check")
		bool InBattleMode();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Internal")
		bool BossFightMode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|BattleMode")
		float MaxDistanceBattleMode = 1500;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|BattleMode")
		float Z_ErrorBattleMode = 500;


};



