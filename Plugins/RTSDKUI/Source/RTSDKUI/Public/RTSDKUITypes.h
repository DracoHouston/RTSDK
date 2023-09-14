// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "RTSDKUITypes.generated.h"

struct FRTSDKCommanderSelectionUnit;
class URTSDKUnitDefinition;

UENUM(BlueprintType)
enum class ERTSSelectionPanelCursorActions : uint8
{
	DoNothing		 		UMETA(DisplayName = "Do Nothing"),
	SetSubSelection 		UMETA(DisplayName = "Set Sub Selection To Target"),
	RemoveOneUnit			UMETA(DisplayName = "Remove One Target From Selection"),
	RemoveAllUnitsOfClass 	UMETA(DisplayName = "Remove All Units Of Same Class To Target From Selection"),
	SelectOneUnit			UMETA(DisplayName = "Select One Target, Discard The Rest From Selection"),
	SelectAllUnitsOfClass	UMETA(DisplayName = "Select All Units Of Same Class To Target, Discard The Rest From Selection"),
};

/**
* For widgets derived from RTSDKSelectionContainerWidget or RTSDKSelectionItemWidget
* this enum is used to pick an option for what happens when a unit dies, or when a unit
* is destroyed (removed from play). The selection container widget can remove dead or
* destroyed units without affecting the actual player selection, which otherwise leaves
* dead and invalid units alone.
*/
UENUM(BlueprintType)
enum class ERTSDKSelectionWidgetUnitLostOption : uint8
{
	RemoveItem 			UMETA(DisplayName = "Remove Item"),
	HideItem			UMETA(DisplayName = "Hide Item"),
	DisplayDeathStatus	UMETA(DisplayName = "Display Death Status"),
	DisplayBlankIcon 	UMETA(DisplayName = "Display Blank Icon"),
};

USTRUCT(BlueprintType)
struct RTSDKUI_API FRTSDKUIUnitsCollection
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
	TSubclassOf<URTSDKUnitDefinition> Definition;

	UPROPERTY(BlueprintReadOnly)
	TArray<FRTSDKCommanderSelectionUnit> Items;
};
