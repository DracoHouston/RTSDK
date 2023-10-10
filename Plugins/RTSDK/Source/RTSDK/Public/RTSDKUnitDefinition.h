// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "RTSDKUnitDBSubsystem.h"
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

	//this and the one below should be an array of category/array of traits structs
	UPROPERTY(Category = "Traits", EditDefaultsOnly)
		FRTSDKUnitDesignerTraitCollection UnitTraits;

	UPROPERTY(transient)
		FRTSDKUnitDesignerTraitCollection BeforeModifiersFinalUnitTraits;

	//this should be a mass config we can use to spawn from
	UPROPERTY(transient)
		FMassEntityConfig FinalUnitTraits;

	/**
	* Unreal actor class for the actor that will represent this unit in the game world
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		TSubclassOf<AActor> UnitActor;

	/**
	* Unreal actor class for the actor that will represent this unit in the game world
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		TSubclassOf<AActor> UnitSimActor;

	/**
	* Unreal actor class for the actor that will represent this unit in the game world
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		TSubclassOf<AActor> UnitVisActor;

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

	UFUNCTION()
		void InitRuntimeTraits();

	UFUNCTION()
		void BuildFinalRuntimeTraits();

	UFUNCTION()
		void ApplyMandatoryUnitTrait(FName inCategoryName, UMassEntityTraitBase* inTrait);
};
