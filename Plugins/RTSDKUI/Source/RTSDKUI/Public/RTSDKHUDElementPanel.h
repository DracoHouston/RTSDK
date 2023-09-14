// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "RTSDKHUDElementPanel.generated.h"

/**
 * 
 */
UCLASS()
class RTSDKUI_API URTSDKHUDElementPanel : public UCommonUserWidget
{
	GENERATED_BODY()
	
public:

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
		FName HUDElementName;
};
