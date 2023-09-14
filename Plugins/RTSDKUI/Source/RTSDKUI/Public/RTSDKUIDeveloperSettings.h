// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "RTSDKUIDeveloperSettings.generated.h"

class UGameUIPolicy;
class UCommonActivatableWidget;

/**
 * 
 */
UCLASS(config = RTSDKUI, defaultconfig, DisplayName = "Real Time Strategy Dev Kit - UI", AutoExpandCategories = "RTSDKUI")
class RTSDKUI_API URTSDKUIDeveloperSettings : public UDeveloperSettings
{
	GENERATED_BODY()
	
public:

	URTSDKUIDeveloperSettings(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UPROPERTY(EditDefaultsOnly, config, Category = "RTSDKUI")
		TSubclassOf<UGameUIPolicy> DefaultUIPolicyClass;

	UPROPERTY(EditDefaultsOnly, config, Category = "RTSDKUI")
		TSubclassOf<UCommonActivatableWidget> GameMenuWidgetClass;

	UPROPERTY(EditDefaultsOnly, config, Category = "RTSDKUI")
		TSubclassOf<UCommonActivatableWidget> MainMenuWidgetClass;
};
