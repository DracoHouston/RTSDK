// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RTSDKUnitHUDWidget.generated.h"

/**
 * Blueprintable UMG Slate Widget with some native variables and methods to drive a Commands Tab widget for UIs that integrate RTSDK
 * Generally derived classes of this manage and contain one or more command items, based on the contents of a command menu page tab.
 * Also useful as a base for tab switching buttons for a RTSCommandsTabContainer derived widget
 */
UCLASS(Abstract, editinlinenew, BlueprintType, Blueprintable, meta = (DontUseGenericSpawnObject = "True", DisableNativeTick))
class RTSDKUI_API URTSDKUnitHUDWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, Category = "RTS Unit HUD Widget")
		AActor* OwnerUnit;
	
	/**
	* The 2D bounds of owning unit in unscaled (viewport) screenspace
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "RTS Unit HUD Widget")
		virtual FBox2D GetOwnerUnitScreenspace2DBounds();

	/**
	* The 2D bounds of owning unit, in widgetspace (dpi scaled).
	* This will correspond with the widgetspace coords that lie under the screenspace coords at current viewport size
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "RTS Unit HUD Widget")
		virtual FBox2D GetOwnerUnitWidgetspace2DBounds();

	/**
	* The size of the 2D bounds of owning unit, in unscaled (viewport) screenspace.
	* In pixels, x component represents the width, y component represents height.
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "RTS Unit HUD Widget")
		virtual FVector2D GetOwnerUnitScreenspace2DBoundsSize();

	/**
	* The size of the 2D bounds of owning unit, in widgetspace (dpi scaled).
	* This corresponds to the size in widgetspace needed to match the size in screenspace at current viewport size
	* In pixels, x component represents the width, y component represents height.
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "RTS Unit HUD Widget")
		virtual FVector2D GetOwnerUnitWidgetspace2DBoundsSize();
};
