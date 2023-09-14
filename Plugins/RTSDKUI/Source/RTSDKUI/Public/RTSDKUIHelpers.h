// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "RTSDKUIHelpers.generated.h"

struct FRTSDKUIUnitsCollection;

/**
 *
 */
UCLASS()
class RTSDKUI_API URTSDKUIHelpers : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	/**
	* Takes an absolute screenspace position (in pixels) and projects it to DPI scaled screenspace (in virtual dpi scaled pixels)
	* Essentially, Absolute * (1 / DPI Scale) on each axis.
	* An example of when this needs to happen is taking raw mouse position and trying to react to it in UMG
	* UMG Widgets position themselves with a virtual screenspace, generally on PC this is a 1080p 16:9 screenspace
	* This screenspace does not change size and shape based on the viewport, but instead scales from this screenspace to
	* the actual screenspace within the game viewport, typically the size of the game window.
	* Therefore, if the viewport is not the same size as the resolution the UMG widget was made for,
	* the cursor position does not equal the virtual position within the widget at that point on the screen
	* The position will be off by a factor of 1 over dpi scale, this compounds if you are using 2 positions
	* in absolute screenspace!
	*/
	UFUNCTION(BlueprintPure, Category = "RTSDK UI Helpers")
		static FVector2D GetDPIScaledScreenspacePosition(FVector2D inAbsolutePosition, double inDPIScale);

	UFUNCTION(BlueprintPure, Category = "RTSDK UI Helpers")
		static int32 GetAliveCountFromUnitCollection(const FRTSDKUIUnitsCollection& inUnits);
};