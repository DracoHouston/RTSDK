// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSDKInputModifiers.h"
#include "EnhancedPlayerInput.h"

FInputActionValue URTSDKInputModifierAdditionalKeys::ModifyRaw_Implementation(const UEnhancedPlayerInput* PlayerInput, FInputActionValue CurrentValue, float DeltaTime)
{
	const FVector value = CurrentValue.Get<FVector>();
	FVector totalvalue = value;
	bool keysaredown = true;
	for (int32 i = 0; i < Keys.Num(); i++)
	{
		const FKeyState* key = PlayerInput->GetKeyState(Keys[i]);
		keysaredown = keysaredown && key->bDown;
		totalvalue += key->Value;
	}
	if (!keysaredown)
	{
		//if the other keys aren't all 'down' (button down, an axis non zero etc) we return a reset value
		CurrentValue.Reset();
		return CurrentValue;
	}
	else
	{
		//if they are down, clamp the total value between -1 and 1 on each axis and return it, same type
		totalvalue.X = FMath::Clamp(totalvalue.X, -1.0, 1.0);
		totalvalue.Y = FMath::Clamp(totalvalue.Y, -1.0, 1.0);
		totalvalue.Z = FMath::Clamp(totalvalue.Z, -1.0, 1.0);
		return FInputActionValue(CurrentValue.GetValueType(), totalvalue);
	}
}

FInputActionValue URTSDKInputModifierDefaultModifierKeyOptions::ModifyRaw_Implementation(const UEnhancedPlayerInput* PlayerInput, FInputActionValue CurrentValue, float DeltaTime)
{
	return CurrentValue;
}

FInputActionValue URTSDKInputModifierIgnoreTapOrHoldOptions::ModifyRaw_Implementation(const UEnhancedPlayerInput* PlayerInput, FInputActionValue CurrentValue, float DeltaTime)
{
	return CurrentValue;
}

FInputActionValue URTSDKInputModifierSettingsTooltip::ModifyRaw_Implementation(const UEnhancedPlayerInput* PlayerInput, FInputActionValue CurrentValue, float DeltaTime)
{
	return CurrentValue;
}
