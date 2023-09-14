// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RTSDKFactionDefinition.generated.h"

class URTSDKUnitDefinition;

/**
 * Definition of a Faction, a set of associated units.
 */
UCLASS(Blueprintable)
class RTSDK_API URTSDKFactionDefinition : public UObject
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		FText DisplayName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		FName DevName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		TObjectPtr<UMaterialInterface> Icon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		TSubclassOf<UUserWidget> StratPawnHUDWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		TArray<TSubclassOf<URTSDKUnitDefinition>> FactionUnitClasses;

	

};
