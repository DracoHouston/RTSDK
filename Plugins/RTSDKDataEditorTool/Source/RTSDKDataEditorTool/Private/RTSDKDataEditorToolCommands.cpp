// Copyright Epic Games, Inc. All Rights Reserved.

#include "RTSDKDataEditorToolCommands.h"

#define LOCTEXT_NAMESPACE "FRTSDKDataEditorToolModule"

void FRTSDKDataEditorToolCommands::RegisterCommands()
{
	UI_COMMAND(OpenPluginWindow, "RTSDKDataEditorTool", "Bring up RTSDKDataEditorTool window", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
