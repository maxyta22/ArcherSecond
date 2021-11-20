// Copyright Epic Games, Inc. All Rights Reserved.

#include "Core/ArcherPRTGameMode.h"
#include "Core/ArcherPRTHUD.h"
#include "Player/PlayerCharacter.h"
#include "UObject/ConstructorHelpers.h"

AArcherPRTGameMode::AArcherPRTGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Blueprint/Player/PlayerCharacter_BP"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AArcherPRTHUD::StaticClass();
}
