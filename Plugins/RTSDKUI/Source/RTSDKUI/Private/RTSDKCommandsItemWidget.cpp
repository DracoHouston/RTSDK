// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "RTSDKCommandsItemWidget.h"
#include "RTSDKPlayerControllerInterface.h"

FText URTSDKCommandsItemWidget::GetPlayerCommandDisplayName()
{
	/*ARTSPlayerController* rtspc = GetOwningPlayer<ARTSPlayerController>();
	if (rtspc != nullptr)
	{
		return rtspc->GetCommandMenuItemDisplayName(CommandIndex, TabIndex);
	}*/
	return FText::GetEmpty();
}

FText URTSDKCommandsItemWidget::GetPlayerCommandTooltip()
{
	/*ARTSPlayerController* rtspc = GetOwningPlayer<ARTSPlayerController>();
	if (rtspc != nullptr)
	{
		return rtspc->GetCommandMenuItemTooltip(CommandIndex, TabIndex);
	}*/
	return FText::GetEmpty();
}

UMaterialInterface* URTSDKCommandsItemWidget::GetPlayerCommandIcon()
{
	/*ARTSPlayerController* rtspc = GetOwningPlayer<ARTSPlayerController>();
	if (rtspc != nullptr)
	{
		return rtspc->GetCommandMenuItemIcon(CommandIndex, TabIndex);
	}*/
	return nullptr;
}
