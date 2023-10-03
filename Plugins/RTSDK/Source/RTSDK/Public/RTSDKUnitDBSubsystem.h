// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "RTSDKUnitDBSubsystem.generated.h"

class URTSDKUnitDefinition;
class URTSDKUnitDefinitionModifier;
class URTSDKModDefinitionBase;

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

	UFUNCTION()
		void AddUnit(TSubclassOf<URTSDKUnitDefinition> inDefinition);

	/*UFUNCTION()
		void AddUnitModifier(URTSDKModDefinitionBase* Caller, TSubclassOf<URTSDKUnitDefinition> inDefinition, URTSDKUnitDefinitionModifier* inModifier);

	UFUNCTION()
		void RemoveUnitModifier(URTSDKModDefinitionBase* Caller, TSubclassOf<URTSDKUnitDefinition> inDefinition, URTSDKUnitDefinitionModifier* inModifier);*/

	UFUNCTION()
		void RemoveUnit(TSubclassOf<URTSDKUnitDefinition> inDefinition);

	UFUNCTION()
		URTSDKUnitDefinition* GetFinalUnitDefinition(TSubclassOf<URTSDKUnitDefinition> inDefinition);

	//add unit
	//add modifier to unit
	//remove modifier to unit
	//remove unit
	//get unit
};
