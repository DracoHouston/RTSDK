// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MassCommandBuffer.h"
#include "RTSDKScriptExecutionContext.generated.h"

USTRUCT(BlueprintType)
struct RTSDK_API FRTSDKScriptExecutionContext
{
public:
	GENERATED_BODY()

	FRTSDKScriptExecutionContext() {}

	FRTSDKScriptExecutionContext(FMassCommandBuffer* inBuffer) : ScriptCommandBuffer(inBuffer) {}

	FMassCommandBuffer* ScriptCommandBuffer;
};