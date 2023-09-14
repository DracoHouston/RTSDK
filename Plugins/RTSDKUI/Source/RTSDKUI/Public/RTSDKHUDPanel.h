// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "RTSDKHUDPanel.generated.h"

class UCanvasPanel;
class URTSDKHUDElementPanel;

/**
 * 
 */
UCLASS()
class RTSDKUI_API URTSDKHUDPanel : public UCommonUserWidget
{
	GENERATED_BODY()
	
protected:

	virtual void OnWidgetRebuilt() override;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		TObjectPtr<UCanvasPanel> HUDCanvas;

	UPROPERTY()
	TMap<FName, TObjectPtr<URTSDKHUDElementPanel>> HUDElements;
};
