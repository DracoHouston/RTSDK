// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "RTSDKDataEditorToolStyle.h"

class FRTSDKDataEditorToolCommands : public TCommands<FRTSDKDataEditorToolCommands>
{
public:

	FRTSDKDataEditorToolCommands()
		: TCommands<FRTSDKDataEditorToolCommands>(TEXT("RTSDKDataEditorTool"), NSLOCTEXT("Contexts", "RTSDKDataEditorTool", "RTSDKDataEditorTool Plugin"), NAME_None, FRTSDKDataEditorToolStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > OpenPluginWindow;
};