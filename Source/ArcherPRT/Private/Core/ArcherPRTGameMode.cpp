// Copyright Epic Games, Inc. All Rights Reserved.

#include "Core/ArcherPRTGameMode.h"
#include "Core/ArcherPRTHUD.h"
#include "Player/PlayerCharacter.h"
#include "UObject/ConstructorHelpers.h"

AArcherPRTGameMode::AArcherPRTGameMode()
	: Super()
{
	HUDClass = AArcherPRTHUD::StaticClass();
}

bool AArcherPRTGameMode::InBattleMode()
{
	return false;
}
