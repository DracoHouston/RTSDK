// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "RTSDKCommandsTabContainerWidget.h"
#include "RTSDKPlayerControllerInterface.h"

int32 URTSDKCommandsTabContainerWidget::GetPlayerTabsCount()
{
	/*ARTSPlayerController* rtspc = GetOwningPlayer<ARTSPlayerController>();
	if (rtspc != nullptr)
	{
		return rtspc->GetCommandMenuPageTabCount();
	}*/
	return 0;
}

FText URTSDKCommandsTabContainerWidget::GetPlayerPageDisplayName()
{
	/*ARTSPlayerController* rtspc = GetOwningPlayer<ARTSPlayerController>();
	if (rtspc != nullptr)
	{
		return rtspc->GetCommandMenuPageDisplayName();
	}*/
	return FText::GetEmpty();
}

FText URTSDKCommandsTabContainerWidget::GetPlayerPageTooltip()
{
	/*ARTSPlayerController* rtspc = GetOwningPlayer<ARTSPlayerController>();
	if (rtspc != nullptr)
	{
		return rtspc->GetCommandMenuPageTooltip();
	}*/
	return FText::GetEmpty();
}

UMaterialInterface* URTSDKCommandsTabContainerWidget::GetPlayerPageIcon()
{
	/*ARTSPlayerController* rtspc = GetOwningPlayer<ARTSPlayerController>();
	if (rtspc != nullptr)
	{
		return rtspc->GetCommandMenuPageIcon();
	}*/
	return nullptr;
}
