// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MassEntityTypes.h"
#include "RTSDKConstants.h"
#include "FixedPointTypes.h"
#include "RTSDKTypes.generated.h"

class URTSDKUnitComponent;
class UInputMappingContext;
class URTSDKKeyBindMetadata;
class URTSDKGameSimSubsystem;
struct FEnhancedActionKeyMapping;
//struct FRTSNumber64;
//struct FRTSVector64;

struct RTSDK_API FRTSTraitHelpers
{
	static AActor* GetOwnerAsActor(UObject* inOwner);


	static bool GetBounds(URTSDKGameSimSubsystem* inSim, AActor* inActor, FRTSVector64& outMin, FRTSVector64& outMax, FRTSVector64& outSize, FRTSNumber64& outHalfHeight, FRTSNumber64& outRadius, FRTSVector64& outFeetPosition);
};

USTRUCT(BlueprintType)
struct RTSDK_API FRTSDKDamageInfo
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite)
		FFixed64 DamageTaken;

	UPROPERTY(BlueprintReadWrite)
		TWeakObjectPtr<URTSDKUnitComponent> Unit;

	UPROPERTY(BlueprintReadWrite)
		TWeakObjectPtr<URTSDKUnitComponent> ResponsibleUnit;
};

UENUM(meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class ERTSDKModifierKeys : uint8
{
	None = 0 UMETA(Hidden),
	Shift = 1 << 0,
	Ctrl = 1 << 1,
	Alt = 1 << 2,
	ShiftCtrl = Shift | Ctrl UMETA(Hidden),
	ShiftAlt = Shift | Alt UMETA(Hidden),
	CtrlAlt = Ctrl | Alt UMETA(Hidden),
	All = Shift | Ctrl | Alt UMETA(Hidden)
};
ENUM_CLASS_FLAGS(ERTSDKModifierKeys)

UENUM(meta = (Bitflags))
enum class ERTSDKInputTapOrHold : uint8
{
	Ignore UMETA(Hidden),
	Tap,
	Hold
};
ENUM_CLASS_FLAGS(ERTSDKInputTapOrHold)

USTRUCT(BlueprintType)
struct RTSDK_API FRTSDKInputModifierAndOption
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		ERTSDKModifierKeys Modifiers;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int32 Options;
};

USTRUCT(BlueprintType)
struct RTSDK_API FRTSDKSavedBind
{
	GENERATED_BODY()
public:

	FRTSDKSavedBind() {}

	FRTSDKSavedBind(const FEnhancedActionKeyMapping& inInputMapping);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FName Action;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TArray<FKey> BoundKeys;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Bitmask, BitmaskEnum = ERTSDKInputTapOrHold))
		ERTSDKInputTapOrHold TapOrHoldOptions;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Bitmask, BitmaskEnum = ERTSDKModifierKeys))
		ERTSDKModifierKeys ActiveModifiers;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TArray<FRTSDKInputModifierAndOption> ModifierOptions;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FText> AvailableModifierOptions;
};

USTRUCT(BlueprintType)
struct RTSDK_API FRTSDKSavedBindPair
{
	GENERATED_BODY()
public:

	FRTSDKSavedBindPair() {}

	FRTSDKSavedBindPair(const FEnhancedActionKeyMapping& inPrimaryMapping, const FEnhancedActionKeyMapping& inSecondaryMapping);

	//for game settings plugin, sets a transient restore point other than default, for cancelling before applying.
	//Sets the initial binds to the current binds
	void SetInitialValues();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FRTSDKSavedBind Primary;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FRTSDKSavedBind Secondary;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FName BindName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FText TooltipText;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TSoftObjectPtr<URTSDKKeyBindMetadata> BindMetadata;

	FRTSDKSavedBind GetBind(bool inbIsPrimary = true);

	FRTSDKSavedBind& GetMutableBind(bool inbIsPrimary = true);
};

USTRUCT(BlueprintType)
struct RTSDK_API FRTSDKSavedBindContext
{
	GENERATED_BODY()
public:

	FRTSDKSavedBindContext() {}

	FRTSDKSavedBindContext(UInputMappingContext* inInputMappingContext);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FName ContextName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TArray<FRTSDKSavedBindPair> PlayerBinds;

	FRTSDKSavedBindPair GetBindsByName(FName inBindName);

	FRTSDKSavedBindPair& GetMutableBindsByName(FName inBindName);
};

USTRUCT(BlueprintType)
struct RTSDK_API FRTSDKInputSettingsProfile
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FText ProfileName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TArray<FRTSDKSavedBindContext> PlayerBindContexts;

	FRTSDKSavedBindContext GetContextByName(FName inContextName);

	FRTSDKSavedBindContext& GetMutableContextByName(FName inContextName);
};

UENUM()
enum class ERTSDKElementAnchorPoint : uint8
{
	TopLeft,
	Top,
	TopRight,
	CenterLeft,
	Center,
	CenterRight,
	BottomLeft,
	Bottom,
	BottomRight,
	MAX UMETA(Hidden)
};

USTRUCT(BlueprintType)
struct RTSDK_API FRTSDKColorChannel
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FName ChannelName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FText ChannelDisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FLinearColor Color;
};

/**
* Represents saved settings for a single HUD element within a profile for a HUD.
*/
USTRUCT(BlueprintType)
struct RTSDK_API FRTSDKSavedHUDElementSettings
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
	* Position of the top left corner of the HUD element in widget-space
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FVector2D AnchorPosition;

	/**
	* The point which we consider anchor, default is top-left growing right and down from anchor
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		ERTSDKElementAnchorPoint AnchorPoint;

	/**
	* Size of this HUD element in widget-space
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FVector2D PanelSize;

	/**
	* Master alpha channel value, making the entire HUD element more or less transparent
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float PanelAlpha;

	/**
	* HUD element will only be activated if this is true
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool bIsEnabled;

	/**
	* An array of 0-n int32 values for storing an enum or bitfield.
	* For interpretation by the HUD element.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TArray<int32> AdditionalOptions;

	/**
	* An array of 0-n FRTSDKColorChannel values.
	* For interpretation by the HUD element. Generic RTSDK elements use
	* channels by these names: Background, Text, TextShadow, Border, Button
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TArray<FRTSDKColorChannel> ColorChannels;
};

USTRUCT(BlueprintType)
struct RTSDK_API FRTSDKHUDSettingsProfile
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FText ProfileName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TArray<FRTSDKSavedHUDElementSettings> Elements;

	FRTSDKSavedHUDElementSettings GetElementByName(FName inElementName);
};

USTRUCT(BlueprintType)
struct RTSDK_API FRTSDKSavedHUDSettings
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FName HUDName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FText HUDDisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TArray<FRTSDKHUDSettingsProfile> Profiles;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int32 ActiveProfile;
};

USTRUCT(BlueprintType)
struct RTSDK_API FRTSDKSavedUISettings
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TArray<FRTSDKSavedHUDSettings> HUDs;
};
