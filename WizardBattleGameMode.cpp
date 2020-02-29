// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "WizardBattleGameMode.h"
#include "WizardBattleHUD.h"
#include "WizardBattleCharacter.h"
#include "UObject/ConstructorHelpers.h"

AWizardBattleGameMode::AWizardBattleGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AWizardBattleHUD::StaticClass();
}
