// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RTSDKCommandsTabContainerWidget.generated.h"

/**
 * Blueprintable UMG Slate Widget with some native variables and methods to drive a Commands Tab Container widget for UIs that integrate RTSDK
 * Intended to display one or more widgets derived from RTSCommandsTabWidget with optional clickable tabs to change the player's current tab
 */
UCLASS(Abstract, editinlinenew, BlueprintType, Blueprintable, meta = (DontUseGenericSpawnObject = "True", DisableNativeTick))
class RTSDKUI_API URTSDKCommandsTabContainerWidget : public UUserWidget
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintCallable, Category = "RTS Commands Tab Widget Helpers")
	int32 GetPlayerTabsCount();

	UFUNCTION(BlueprintCallable, Category = "RTS Commands Tab Widget Helpers")
	FText GetPlayerPageDisplayName();

	UFUNCTION(BlueprintCallable, Category = "RTS Commands Tab Widget Helpers")
	FText GetPlayerPageTooltip();

	UFUNCTION(BlueprintCallable, Category = "RTS Commands Tab Widget Helpers")
	UMaterialInterface* GetPlayerPageIcon();
};
