// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "MassEntityConfigAsset.h"
#include "RTSDKUnitDBSubsystem.generated.h"

class URTSDKUnitDefinition;
class URTSDKUnitDefinitionModifier;
class URTSDKModDefinitionBase;
class UMassEntityTraitBase;

USTRUCT(BlueprintType)
struct RTSDK_API FRTSDKUnitDBTraitCategoryInfo
{
public:
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
		FName CategoryName;

	UPROPERTY(Category = "Traits", EditAnywhere, Instanced)
		TArray<TObjectPtr<UMassEntityTraitBase>> Traits;
};

USTRUCT(BlueprintType)
struct RTSDK_API FRTSDKUnitDesignerTraitCollection
{
public:
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
		TArray<FRTSDKUnitDBTraitCategoryInfo> TraitCategories;
};

USTRUCT(BlueprintType)
struct RTSDK_API FRTSDKUnitDBTraitArray
{
public:
	GENERATED_BODY()
public:
	UPROPERTY()
		TArray<TObjectPtr<UMassEntityTraitBase>> Traits;
};

/**
 * RTSDK Unit Database
 * Stores the allowed unit types for a world, as declared by mods.
 * Allows non-destructive modifiers to existing declared units by other mods.
 * Resulting unit definitions are the sum of all units from active mods with final modified stats.
 */
UCLASS()
class RTSDK_API URTSDKUnitDBSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
public:

	/**
	* Adds mandatory unit traits for a specified category.
	* These traits are meant to be conflict free and additive.
	* They are also expected to be the bare minimum a unit requires to function.
	* If a unit definition does not implement a mandatory trait it is auto added.
	* Duplicate trait classes between mods are resolved as the last mod to declare it wins
	*/
	UFUNCTION()
		void AddMandatoryUnitTraits(const TArray<FRTSDKUnitDBTraitCategoryInfo>& inTraits);

	/**
	* Adds optional unit traits for a specified category.
	* These traits are meant to be conflict free and additive.
	* They are also expected to offer optional functionality.
	* They are never auto-added to a final unit definition.
	* Used for surfacing available optional traits in the editor
	* Duplicate trait classes between mods are resolved as the last mod to declare it wins
	*/
	UFUNCTION()
		void AddOptionalUnitTraits(const TArray<FRTSDKUnitDBTraitCategoryInfo>& inTraits);

	/**
	* Adds mutually exclusive mandatory traits for a specified category. 
	* These are traits that do not work together, but 1 of them is a requirement.
	* Units may implement only one of these per category. 
	* First one implemented by a unit definition wins, others are discarded.
	* If one is not implemented by a unit, units will use the first one declared by any mod.
	* Duplicate trait classes between mods are resolved as the last mod to declare it wins
	*/
	UFUNCTION()
		void AddMutuallyExclusiveMandatoryUnitTraits(const TArray<FRTSDKUnitDBTraitCategoryInfo>& inTraits);

	/**
	* Adds mutually exclusive optional traits for a specified category.
	* These are traits that do not work together, but adding at least 1 offers functionality.
	* Units may implement only one of these per category.
	* First one implemented by a unit definition wins, others are discarded.
	* If one is not implemented by a unit, units will not use any of them.
	* Duplicate trait classes between mods are resolved as the last mod to declare it wins
	*/
	UFUNCTION()
		void AddMutuallyExclusiveOptionalUnitTraits(const TArray<FRTSDKUnitDBTraitCategoryInfo>& inTraits);

	UFUNCTION()
		void AddUnit(TSubclassOf<URTSDKUnitDefinition> inDefinition);

	UFUNCTION()
		void AddUnitModifier(TSubclassOf<URTSDKUnitDefinition> inDefinition, URTSDKUnitDefinitionModifier* inModifier);

	UFUNCTION()
		void RemoveUnitModifier(TSubclassOf<URTSDKUnitDefinition> inDefinition, URTSDKUnitDefinitionModifier* inModifier);

	UFUNCTION()
		void RemoveUnit(TSubclassOf<URTSDKUnitDefinition> inDefinition);

	UFUNCTION()
		URTSDKUnitDefinition* GetFinalUnitDefinition(TSubclassOf<URTSDKUnitDefinition> inDefinition);

	//add unit
	//add modifier to unit
	//remove modifier to unit
	//remove unit
	//get unit
protected:

	UPROPERTY()
		TMap<FName, FRTSDKUnitDBTraitArray> MandatoryUnitTraitsByCategory;

	UPROPERTY()
		TMap<FName, FRTSDKUnitDBTraitArray> OptionalUnitTraitsByCategory;

	UPROPERTY()
		TMap<FName, FRTSDKUnitDBTraitArray> MandatoryMutuallyExclusiveUnitTraitsByCategory;

	UPROPERTY()
		TMap<FName, FRTSDKUnitDBTraitArray> OptionalMutuallyExclusiveUnitTraitsByCategory;

	UPROPERTY()
		TMap<TSubclassOf<URTSDKUnitDefinition>, TObjectPtr<URTSDKUnitDefinition>> WorldUnitDB;
};

UCLASS()
class RTSDK_API URTSDKUnitDefinitionModifier : public UObject
{
	GENERATED_BODY()
public:

	UFUNCTION()
		virtual void ApplyModifier(URTSDKUnitDBSubsystem* UnitDB) {}
};