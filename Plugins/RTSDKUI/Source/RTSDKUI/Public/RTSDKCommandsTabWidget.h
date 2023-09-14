// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RTSDKCommandsTabWidget.generated.h"

/**
 * Blueprintable UMG Slate Widget with some native variables and methods to drive a Commands Tab widget for UIs that integrate RTSDK
 * Generally derived classes of this manage and contain one or more command items, based on the contents of a command menu page tab.
 * Also useful as a base for tab switching buttons for a RTSCommandsTabContainer derived widget
 */
UCLASS(Abstract, editinlinenew, BlueprintType, Blueprintable, meta = (DontUseGenericSpawnObject = "True", DisableNativeTick))
class RTSDKUI_API URTSDKCommandsTabWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RTS Commands Tab Widget")
	int32 TabIndex;

	UFUNCTION(BlueprintCallable, Category = "RTS Commands Tab Widget Helpers")
	int32 GetPlayerTabCommandsCount();

	UFUNCTION(BlueprintCallable, Category = "RTS Commands Tab Widget Helpers")
	FText GetPlayerTabDisplayName();

	UFUNCTION(BlueprintCallable, Category = "RTS Commands Tab Widget Helpers")
	FText GetPlayerTabTooltip();

	UFUNCTION(BlueprintCallable, Category = "RTS Commands Tab Widget Helpers")
	UMaterialInterface* GetPlayerTabIcon();
};
