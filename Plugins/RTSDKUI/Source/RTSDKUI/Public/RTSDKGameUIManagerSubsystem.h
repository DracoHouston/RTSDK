// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameUIManagerSubsystem.h"
#include "RTSDKGameUIManagerSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class RTSDKUI_API URTSDKGameUIManagerSubsystem : public UGameUIManagerSubsystem
{
	GENERATED_BODY()
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
};
