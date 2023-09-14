// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFeatureData.h"
#include "RTSDKGameFeatureData.generated.h"

class URTSDKFactionDefinition;

UENUM()
enum class ERTSDKGameFeatureTypes : uint8
{
	Feature UMETA(ToolTip = "A mod that provides a modular gameplay feature, for use by other mods"),
	Mutator UMETA(ToolTip = "A mod that provides an additive modifier on the game"),
	Game UMETA(ToolTip = "A mod that provides the code and content common to playing a particular game"),
	Faction UMETA(ToolTip = "A mod that provides the code and content required for a faction, particularly definitions for associated units"),
	Map  UMETA(ToolTip = "A mod that provides code and content required for a version of a map, particularly the associated level file"),
	MAX UMETA(Hidden)
};

/**
 * 
 */
UCLASS()
class RTSDK_API URTSDKGameFeatureData : public UGameFeatureData
{
	GENERATED_BODY()

protected:

	UFUNCTION()
		TArray<FString> GetGameModNames();

	UFUNCTION()
		TArray<FString> GetCurrentTypeModNames();

	UFUNCTION()
		TArray<FString> GetAllFeatureNames();

public:

	/*
	* Game feature type, what kind of modification this plugin represents.
	* Feature mods are building blocks for mods representing games, factions, maps and mutators
	* Game, Faction and Map mods are the primary game content types
	* Mutators are additive mods that provide gameplay on top of that provided by a game mod.
	* 
	* It is the mod authors responsibility to set this correctly and fill out associated data.
	*/
	UPROPERTY(EditDefaultsOnly)
		ERTSDKGameFeatureTypes GameFeatureType;

	/*
	* Unique developer name for this mod. For internal use as an identifier.
	* By convention, this should be the same as the name of the plugin and this game feature data,
	* but can be different, as long as it is unique.
	*/
	UPROPERTY(EditDefaultsOnly)
		FName DevName;

	/*
	* User facing display name for this mod.
	*/
	UPROPERTY(EditDefaultsOnly)
		FText DisplayName;

	/*
	* User facing text for the description of this mod
	*/
	UPROPERTY(EditDefaultsOnly)
		FText DescriptionText;

	/*
	* User facing text for the name of the author of this mod.
	*/
	UPROPERTY(EditDefaultsOnly)
		FText AuthorName;

	/**
	* DevNames of any Feature mods this is dependent on. Will be activated if this mod is.
	* All must also be registered for the mod to work/validate
	*/
	UPROPERTY(EditDefaultsOnly, meta = (EditCondition = "GameFeatureType != ERTSDKGameFeatureTypes::Feature", EditConditionHides))
		TArray<FName> FeatureDependencies;

	/*
	* For Game, Faction and Map mods, an abstract mod is one that is not presented to the user for direct use.
	* It is a mod that is inherited from by child mods, to be used as the base. eg base, campaign and mp versions of a game/faction, and map variants
	*/
	UPROPERTY(EditDefaultsOnly, meta = (EditCondition = "(GameFeatureType != ERTSDKGameFeatureTypes::Feature) && (GameFeatureType != ERTSDKGameFeatureTypes::Mutator)", EditConditionHides))
		bool bIsAbstractMod;

	/*
	* For Game, Faction and Map mods, a parent mod can be declared and used as a base for this mod.
	* The mod will inherit dependencies and associations from their parent
	* If empty, this mod will have no parent
	*/
	UPROPERTY(EditDefaultsOnly, meta = (EditCondition = "(GameFeatureType != ERTSDKGameFeatureTypes::Feature) && (GameFeatureType != ERTSDKGameFeatureTypes::Mutator)", EditConditionHides))
		FName ParentModDevName;

	/*
	* For Faction, Map and Mutator mods, an association must be set with a Game mod.
	* The mod will appear as an available option when setting up matches with that game mod and their children.
	*/
	UPROPERTY(EditDefaultsOnly, meta = (EditCondition = "(GameFeatureType != ERTSDKGameFeatureTypes::Feature) && (GameFeatureType != ERTSDKGameFeatureTypes::Game)", EditConditionHides))
		FName AssociatedGameModDevName;

	/*
	* For faction mods, the Faction Definition class, for preloading associated units etc
	*/
	UPROPERTY(EditAnywhere, meta = (AllowedTypes = "Faction", EditCondition = "GameFeatureType == ERTSDKGameFeatureTypes::Faction", EditConditionHides))
		TSoftClassPtr<URTSDKFactionDefinition> FactionClass;

	/*
	* For map mods, the main level asset to be used when playing this map.
	*/
	UPROPERTY(EditAnywhere, meta = (AllowedTypes = "Map", EditCondition = "GameFeatureType == ERTSDKGameFeatureTypes::Map", EditConditionHides))
		FPrimaryAssetId MapLevel;
};
