// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "RTSDKUnitHUDWidget.h"
#include "RTSDKUnitComponent.h"
#include "RTSDKUIHelpers.h"
#include "Blueprint/WidgetLayoutLibrary.h"

FBox2D URTSDKUnitHUDWidget::GetOwnerUnitScreenspace2DBounds()
{
	if (OwnerUnit != nullptr)
	{
//		return IRTSUnitInterface::Execute_GetUnit2DBoundsInScreenspace(OwnerUnit);
	}
	return FBox2D();
}

FBox2D URTSDKUnitHUDWidget::GetOwnerUnitWidgetspace2DBounds()
{
	if (OwnerUnit != nullptr)
	{
		FBox2D screenspacebox = GetOwnerUnitScreenspace2DBounds();
		FBox2D widgetspacebox;
		float dpiscale = UWidgetLayoutLibrary::GetViewportScale(OwnerUnit);
		widgetspacebox.Min = URTSDKUIHelpers::GetDPIScaledScreenspacePosition(screenspacebox.Min, dpiscale);
		widgetspacebox.Max = URTSDKUIHelpers::GetDPIScaledScreenspacePosition(screenspacebox.Max, dpiscale);
		return widgetspacebox;
	}
	return FBox2D();
}

FVector2D URTSDKUnitHUDWidget::GetOwnerUnitScreenspace2DBoundsSize()
{
	if (OwnerUnit != nullptr)
	{
		FBox2D screenspacebox = GetOwnerUnitScreenspace2DBounds();
		return screenspacebox.Max - screenspacebox.Min;	
	}
	return FVector2D();
}

FVector2D URTSDKUnitHUDWidget::GetOwnerUnitWidgetspace2DBoundsSize()
{
	if (OwnerUnit != nullptr)
	{
		FBox2D widgetspacebox = GetOwnerUnitWidgetspace2DBounds();
		return widgetspacebox.Max - widgetspacebox.Min;
	}
	return FVector2D();
}
