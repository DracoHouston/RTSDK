// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "MassEntityConfigAsset.h"
#include "RTSDKUnitDefinition.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class RTSDK_API URTSDKUnitDefinition : public UObject
{
	GENERATED_BODY()
	
public:

	/**
	* Mass Entity config for the entity that will represent this unit in the game sim
	*/
	UPROPERTY(EditDefaultsOnly)
		FMassEntityConfig UnitConfig;

	/**
	* Unreal actor class for the actor that will represent this unit in the game world
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		TSubclassOf<AActor> UnitActor;

	/**
	* Display name for this unit, for UI purposes
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		FText DisplayName;

	/**
	* Icon for this unit, for UI purposes
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		TObjectPtr<UMaterialInterface> Icon;

#if WITH_EDITORONLY_DATA
	UPROPERTY(EditDefaultsOnly)
		TObjectPtr<class USkeletalMesh> PreviewSkeletalMeshAsset;
#endif
};
