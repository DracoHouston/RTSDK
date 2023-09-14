// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Engine/NavigationObjectBase.h"
#include "RTSDKUnitSpawner.generated.h"

class URTSDKUnitDefinition;

UCLASS(Blueprintable, ClassGroup = Common, hidecategories = Collision)
class RTSDK_API ARTSDKUnitSpawner : public ANavigationObjectBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARTSDKUnitSpawner(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UPROPERTY(EditAnywhere)
		TSubclassOf<URTSDKUnitDefinition> UnitType;

	UPROPERTY(EditAnywhere)
		int32 ForceID;

#if WITH_EDITORONLY_DATA
private:
	UPROPERTY()
		TObjectPtr<class UArrowComponent> ArrowComponent;
public:
#endif

#if WITH_EDITORONLY_DATA
	/** Returns ArrowComponent subobject **/
	class UArrowComponent* GetArrowComponent() const;
#endif

};
