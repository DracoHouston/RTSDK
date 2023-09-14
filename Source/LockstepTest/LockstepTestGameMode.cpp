// Copyright Epic Games, Inc. All Rights Reserved.

#include "LockstepTestGameMode.h"
#include "LockstepTestPlayerController.h"
#include "LockstepTestCharacter.h"
#include "UObject/ConstructorHelpers.h"

ALockstepTestGameMode::ALockstepTestGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = ALockstepTestPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	// set default controller to our Blueprinted controller
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownPlayerController"));
	if(PlayerControllerBPClass.Class != NULL)
	{
		PlayerControllerClass = PlayerControllerBPClass.Class;
	}
}