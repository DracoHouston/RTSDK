// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputModifiers.h"
#include "RTSDKTypes.h"
#include "RTSDKInputModifiers.generated.h"

/**
 * 
 */
UCLASS()
class RTSDK_API URTSDKInputModifierAdditionalKeys : public UInputModifier
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Settings")
		TArray<FKey> Keys;

protected:
	virtual FInputActionValue ModifyRaw_Implementation(const UEnhancedPlayerInput* PlayerInput, FInputActionValue CurrentValue, float DeltaTime) override;
};

/**
 *
 */
UCLASS()
class RTSDK_API URTSDKInputModifierDefaultModifierKeyOptions : public UInputModifier
{
	GENERATED_BODY()

public:

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Settings")
		TArray<FText> AvailableOptions;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Settings")
		TArray<FRTSDKInputModifierAndOption> OptionsPerModifierKeyCombo;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Settings", meta = (Bitmask, BitmaskEnum = ERTSDKModifierKeys))
		ERTSDKModifierKeys DefaultActiveModifierKeys;

protected:
	virtual FInputActionValue ModifyRaw_Implementation(const UEnhancedPlayerInput* PlayerInput, FInputActionValue CurrentValue, float DeltaTime) override;
};

/**
 *
 */
UCLASS()
class RTSDK_API URTSDKInputModifierIgnoreTapOrHoldOptions : public UInputModifier
{
	GENERATED_BODY()

public:

protected:
	virtual FInputActionValue ModifyRaw_Implementation(const UEnhancedPlayerInput* PlayerInput, FInputActionValue CurrentValue, float DeltaTime) override;
};

/**
 *
 */
UCLASS()
class RTSDK_API URTSDKInputModifierSettingsTooltip : public UInputModifier
{
	GENERATED_BODY()

public:

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Settings")
		FText TooltipText;
protected:
	virtual FInputActionValue ModifyRaw_Implementation(const UEnhancedPlayerInput* PlayerInput, FInputActionValue CurrentValue, float DeltaTime) override;
};
