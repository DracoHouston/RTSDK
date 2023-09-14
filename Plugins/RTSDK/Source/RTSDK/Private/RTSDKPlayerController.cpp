// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSDKPlayerController.h"
#include "RTSDKPlayerState.h"
#include "FixedPointTypes.h"
#include "RTSDKPlayerCommand.h"
#include "RTSDKCommanderState.h"
#include "RTSDKGameSimSubsystem.h"
#include "RTSDKUnitComponent.h"
#include "RTSDKSimState.h"
#include "Net/UnrealNetwork.h"
#include "CommonUIExtensions.h"
#include "Input/CommonUIInputTypes.h"
#include "Misc/AssertionMacros.h"
#include "NativeGameplayTags.h"
#include "UITag.h"
#include "RTSDKDeveloperSettings.h"
#include "CommonActivatableWidget.h"

UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_UI_LAYER_GAMEMENU, "UI.Layer.GameMenu");

ARTSDKPlayerController::ARTSDKPlayerController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bWantsToBeReady = false;
}

void ARTSDKPlayerController::BeginPlay()
{
	Super::BeginPlay();
#if WITH_EDITORONLY_DATA
	UWorld* world = GetWorld();
	if ((world->WorldType == EWorldType::PIE) && IsLocalPlayerController())
	{
		SetReadyState(true);
	}
#endif
}

void ARTSDKPlayerController::Server_SendPlayerCommand_Implementation(const FRTSDKPlayerCommandReplicationInfo& inCommand)
{
	SendPlayerCommand(inCommand);
}

void ARTSDKPlayerController::Server_SetReadyState_Implementation(bool inIsReady)
{
	SetReadyState(inIsReady);
}

void ARTSDKPlayerController::OnRep_CommanderState()
{
	if (bWantsToBeReady)
	{
		SetReadyState(true);
	}
}

void ARTSDKPlayerController::Server_RequestTimescale_Implementation(const FFixed64& inTimescale)
{
	RequestTimescale(inTimescale);
}

void ARTSDKPlayerController::RTSDKTogglePause()
{
	UWorld* world = GetWorld();
	if (world == nullptr)
	{
		return;
	}
	URTSDKGameSimSubsystem* sim = world->GetSubsystem<URTSDKGameSimSubsystem>();
	if (sim == nullptr)
	{
		return;
	}
	ARTSDKSimStateBase* simstate = sim->GetSimState();
	if (simstate == nullptr)
	{
		return;
	}
	if (simstate->GetMatchIsPaused())
	{
		RequestUnpause();
		return;
	}
	RequestPause();
}

void ARTSDKPlayerController::RTSDKRequestPause()
{
	RequestPause();
}

void ARTSDKPlayerController::RTSDKRequestUnpause()
{
	RequestUnpause();
}

void ARTSDKPlayerController::RTSDKRequestTimescale(double inTimescale)
{
	RequestTimescale(inTimescale);
}

void ARTSDKPlayerController::RTSDKToggleIngameMenu()
{
	/*if ((GameMenuWidget == nullptr) || (GameMenuWidget->GetParent() == nullptr))
	{
		RTSDKOpenIngameMenu();
	}
	else
	{
		RTSDKCloseIngameMenu();
	}*/
}

void ARTSDKPlayerController::RTSDKOpenIngameMenu()
{
	/*ULocalPlayer* player = GetLocalPlayer();
	if (player == nullptr)
	{
		return;
	}
	URTSDKDeveloperSettings* rtsdksettings = GetMutableDefault<URTSDKDeveloperSettings>();
	if (rtsdksettings->GameMenuWidgetClass == nullptr)
	{
		return;
	}
	if ((GameMenuWidget == nullptr) || (GameMenuWidget->GetParent() == nullptr))
	{		
		GameMenuWidget = UCommonUIExtensions::PushContentToLayer_ForPlayer(player, TAG_UI_LAYER_GAMEMENU, rtsdksettings->GameMenuWidgetClass);
	}*/
}

void ARTSDKPlayerController::RTSDKCloseIngameMenu()
{
	/*ULocalPlayer* player = GetLocalPlayer();
	if ((player != nullptr) && (GameMenuWidget != nullptr))
	{
		UCommonUIExtensions::PopContentFromLayer(GameMenuWidget);
		GameMenuWidget = nullptr;
	}*/
}

void ARTSDKPlayerController::TestMoveInputCommand(const FVector2D& input)
{
	FRTSDKPlayerCommandReplicationInfo cmd;
	cmd.Class = URTSDKPawnMoveInputPlayerCommand::StaticClass();
	FVector2D inputnormalized = input.GetSafeNormal();
	cmd.TargetLocations.Add(FFixedVector64(inputnormalized.X, inputnormalized.Y, 0.0));
	cmd.UnitIDs.Add(0);
	SendPlayerCommand(cmd);
}

void ARTSDKPlayerController::Server_RequestPause_Implementation()
{
	RequestPause();
}

void ARTSDKPlayerController::Server_RequestUnpause_Implementation()
{
	RequestUnpause();
}

void ARTSDKPlayerController::SetReadyState(bool inIsReady)
{
	if (CommanderState == nullptr)
	{
		bWantsToBeReady = inIsReady;
		return;
	}
	if (GetWorld()->GetNetMode() == ENetMode::NM_Client)
	{
		Server_SetReadyState(inIsReady);
		return;
	}
	CommanderState->SetIsReady(inIsReady);
}

void ARTSDKPlayerController::SendPlayerCommand(const FRTSDKPlayerCommandReplicationInfo& inCommand)
{
	if (CommanderState == nullptr)
	{
		return;
	}
	
	if (GetWorld()->GetNetMode() == ENetMode::NM_Client)
	{
		Server_SendPlayerCommand(inCommand);
		return;
	}
	CommanderState->AddCommandToCommandBuffer(inCommand);
}

void ARTSDKPlayerController::FinishInputTurn(int32 inTurn, int32 inChecksum)
{
	if (CommanderState == nullptr)
	{
		return;
	}
	if (GetWorld()->GetNetMode() == ENetMode::NM_Client)
	{
		Server_FinishInputTurn(inTurn, inChecksum);
		return;
	}
	CommanderState->SetLastCompletedTurn(inTurn);
}

void ARTSDKPlayerController::RequestPause()
{
	if (GetWorld()->GetNetMode() == ENetMode::NM_Client)
	{
		Server_RequestPause();
		return;
	}

	UWorld* world = GetWorld();
	if (world == nullptr)
	{
		return;
	}
	URTSDKGameSimSubsystem* sim = world->GetSubsystem<URTSDKGameSimSubsystem>();
	if (sim == nullptr)
	{
		return;
	}
	ARTSDKSimStateBase* simstate = sim->GetSimState();
	if (simstate == nullptr)
	{
		return;
	}

	simstate->RequestPause(this);
}

void ARTSDKPlayerController::RequestUnpause()
{
	if (GetWorld()->GetNetMode() == ENetMode::NM_Client)
	{
		Server_RequestUnpause();
		return;
	}

	UWorld* world = GetWorld();
	if (world == nullptr)
	{
		return;
	}
	URTSDKGameSimSubsystem* sim = world->GetSubsystem<URTSDKGameSimSubsystem>();
	if (sim == nullptr)
	{
		return;
	}
	ARTSDKSimStateBase* simstate = sim->GetSimState();
	if (simstate == nullptr)
	{
		return;
	}

	simstate->RequestUnpause(this);
}

void ARTSDKPlayerController::RequestTimescale(const FFixed64& inTimescale)
{
	if (GetWorld()->GetNetMode() == ENetMode::NM_Client)
	{
		Server_RequestTimescale(inTimescale);
		return;
	}

	UWorld* world = GetWorld();
	if (world == nullptr)
	{
		return;
	}
	URTSDKGameSimSubsystem* sim = world->GetSubsystem<URTSDKGameSimSubsystem>();
	if (sim == nullptr)
	{
		return;
	}
	ARTSDKSimStateBase* simstate = sim->GetSimState();
	if (simstate == nullptr)
	{
		return;
	}
	simstate->RequestTimescale(this, inTimescale);
}

ARTSDKCommanderStateBase* ARTSDKPlayerController::GetCommanderState()
{
	return CommanderState;
}

void ARTSDKPlayerController::SetCommanderState(ARTSDKCommanderStateBase* inState)
{
	if (GetWorld()->GetNetMode() < ENetMode::NM_Client)
	{
		CommanderState = inState;
		OnRep_CommanderState();
	}
}

bool ARTSDKPlayerController::GetWantsToBeReady()
{
	return bWantsToBeReady;
}

void ARTSDKPlayerController::Server_FinishInputTurn_Implementation(int32 inTurn, int32 inChecksum)
{
	FinishInputTurn(inTurn, inChecksum);
}

void ARTSDKPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ARTSDKPlayerController, CommanderState);
}