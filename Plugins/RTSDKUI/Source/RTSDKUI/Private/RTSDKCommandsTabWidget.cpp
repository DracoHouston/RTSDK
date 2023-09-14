// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "RTSDKCommandsTabWidget.h"
#include "RTSDKPlayerControllerInterface.h"

int32 URTSDKCommandsTabWidget::GetPlayerTabCommandsCount()
{
	/*ARTSPlayerController* rtspc = GetOwningPlayer<ARTSPlayerController>();
	if (rtspc != nullptr)
	{
		return rtspc->GetCommandMenuItemCount(TabIndex);
	}*/
	return 0;
}

FText URTSDKCommandsTabWidget::GetPlayerTabDisplayName()
{
	/*ARTSPlayerController* rtspc = GetOwningPlayer<ARTSPlayerController>();
	if (rtspc != nullptr)
	{
		return rtspc->GetCommandMenuPageTabDisplayName(TabIndex);
	}*/
	return FText::GetEmpty();
}

FText URTSDKCommandsTabWidget::GetPlayerTabTooltip()
{
	/*ARTSPlayerController* rtspc = GetOwningPlayer<ARTSPlayerController>();
	if (rtspc != nullptr)
	{
		return rtspc->GetCommandMenuPageTabTooltip(TabIndex);
	}*/
	return FText::GetEmpty();
}

UMaterialInterface* URTSDKCommandsTabWidget::GetPlayerTabIcon()
{
	/*ARTSPlayerController* rtspc = GetOwningPlayer<ARTSPlayerController>();
	if (rtspc != nullptr)
	{
		return rtspc->GetCommandMenuPageTabIcon(TabIndex);
	}*/
	return nullptr;
}
