// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RTSDKCommandsItemWidget.generated.h"

/**
 * Blueprintable UMG Slate Widget with some native variables and methods to drive a Commands Item widget for UIs that integrate RTSDK
 * Designed to be a button etc that displays the command in some form. These are generally managed by a bp class derived from RTSCommandsTabWidget
 */
UCLASS(Abstract, editinlinenew, BlueprintType, Blueprintable, meta = (DontUseGenericSpawnObject = "True", DisableNativeTick))
class RTSDKUI_API URTSDKCommandsItemWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RTS Commands Tab Widget")
	int32 TabIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RTS Commands Tab Widget")
	int32 CommandIndex;

	UFUNCTION(BlueprintCallable, Category = "RTS Commands Tab Widget Helpers")
	FText GetPlayerCommandDisplayName();

	UFUNCTION(BlueprintCallable, Category = "RTS Commands Tab Widget Helpers")
	FText GetPlayerCommandTooltip();

	UFUNCTION(BlueprintCallable, Category = "RTS Commands Tab Widget Helpers")
	UMaterialInterface* GetPlayerCommandIcon();
};
