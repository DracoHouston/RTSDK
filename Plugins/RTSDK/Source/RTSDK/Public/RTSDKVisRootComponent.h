// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "RTSDKVisRootComponent.generated.h"

/**
* A scene component which acts as the root component for RTSDK unit's visualization.
* Components not attached to this one are considered for use in the unit's simulation.
* When interpolating between sim frames, this component will move its children with it
* relative to the actual root component, which only moves during sim frames.
*/
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RTSDK_API URTSDKVisRootComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URTSDKVisRootComponent();
	
};
