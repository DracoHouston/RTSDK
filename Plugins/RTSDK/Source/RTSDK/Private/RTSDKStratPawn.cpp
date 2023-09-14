// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSDKStratPawn.h"
#include "RTSDKDeveloperSettings.h"
#include "EnhancedInputComponent.h"
#include "AIController.h"
#include "GameFramework/HUD.h"
#include "Blueprint/UserWidget.h"
#include "InputAction.h"
#include "InputMappingContext.h"

// Sets default values
ARTSDKStratPawn::ARTSDKStratPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void ARTSDKStratPawn::PossessedBy(AController* inController)
{
	/*if ((PreviousController != nullptr) && (PlayerHUD != nullptr))
	{
		if (PreviousController->IsLocalPlayerController())
		{
			PlayerHUD->RemoveFromParent();
		}
	}*/
	Super::PossessedBy(inController);
	/*if (inController->IsLocalPlayerController())
	{
		if (PlayerHUD == nullptr)
		{	
			const URTSDKDeveloperSettings* RTSDKSettings = GetDefault<URTSDKDeveloperSettings>();
			APlayerController* pc = Cast<APlayerController>(inController);
			if ((pc != nullptr) && (RTSDKSettings->DefaultStratPawnHUD != nullptr))
			{
				PlayerHUD = CreateWidget(pc, RTSDKSettings->DefaultStratPawnHUD);
				PlayerHUD->AddToPlayerScreen();
			}
		}
		else
		{
			PlayerHUD->AddToPlayerScreen();
		}
	}*/
}

void ARTSDKStratPawn::OnRep_Controller()
{
	/*if ((PreviousController != nullptr) && (PlayerHUD != nullptr))
	{
		if (PreviousController->IsLocalPlayerController())
		{
			PlayerHUD->RemoveFromParent();
		}
	}*/
	Super::OnRep_Controller();
	/*if (Controller->IsLocalPlayerController())
	{
		if (PlayerHUD == nullptr)
		{
			const URTSDKDeveloperSettings* RTSDKSettings = GetDefault<URTSDKDeveloperSettings>();
			APlayerController* pc = Cast<APlayerController>(Controller);
			if ((pc != nullptr) && (RTSDKSettings->DefaultStratPawnHUD != nullptr))
			{
				PlayerHUD = CreateWidget(pc, RTSDKSettings->DefaultStratPawnHUD);
				PlayerHUD->AddToPlayerScreen();
			}
		}
		else
		{
			PlayerHUD->AddToPlayerScreen();
		}
	}*/
}
