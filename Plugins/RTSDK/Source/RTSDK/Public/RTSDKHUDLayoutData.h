// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "RTSDKTypes.h"
#include "RTSDKHUDLayoutData.generated.h"

USTRUCT(BlueprintType)
struct RTSDK_API FRTSDKInitialHUDElementSettings
{
	GENERATED_BODY()
public:

	/**
	* Dev name of the HUD element
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FName ElementName;

	/**
	* Display name of the HUD element
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FText ElementDisplayName;

	/**
	* If the user may move this panel around from the intial position
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool bUserMayMovePanel;

	/**
	* Position of the anchor point of the HUD element in widget-space
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FVector2D AnchorPosition;

	/**
	* The point which we consider anchor, default is top-left growing right and down from anchor
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		ERTSDKElementAnchorPoint AnchorPoint;

	/**
	* Whether the user may change the anchor point, and therefore change the direction of growth.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool bUserMayChangeAnchor;

	/**
	* Size of this HUD element in widget-space
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FVector2D PanelSize;

	/**
	* If the user is allowed to resize this panel
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool bUserMayResizePanel;

	/**
	* Master alpha channel value, making the entire HUD element more or less transparent
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float PanelAlpha;

	/**
	* Whether the user may change the transparency alpha value for this panel beyond default
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool bUserMayChangeAlpha;

	/**
	* HUD element will only be activated if this is true
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool bIsEnabled;

	/**
	* If the user may disable this panel
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool bUserMayDisablePanel;

	/**
	* An array of 0-n int32 values for storing an enum or bitfield.
	* For interpretation by the HUD element.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TArray<int32> AdditionalOptions;

	/**
	* An array of 0-n FRTSDKColorChannel values.
	* For interpretation by the HUD element. Generic RTSDK elements use
	* them in this order: Background, Text, TextShadow, Border, Button
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TArray<FRTSDKColorChannel> ColorChannels;

};


/**
 * 
 */
UCLASS()
class RTSDK_API URTSDKHUDLayoutData : public UDataAsset
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TArray<FRTSDKInitialHUDElementSettings> Elements;

	/**
	* The widget class for this HUD
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TSoftClassPtr<UUserWidget> WidgetClass;
};
