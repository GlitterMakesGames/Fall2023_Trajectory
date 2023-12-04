// Copyright Epic Games, Inc. All Rights Reserved.

#include "Fall2023_Traject_v1GameMode.h"
#include "Fall2023_Traject_v1HUD.h"
#include "Fall2023_Traject_v1Character.h"
#include "UObject/ConstructorHelpers.h"

AFall2023_Traject_v1GameMode::AFall2023_Traject_v1GameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AFall2023_Traject_v1HUD::StaticClass();
}
