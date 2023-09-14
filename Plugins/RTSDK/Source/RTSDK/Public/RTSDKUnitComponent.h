// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MassCommonTypes.h"
#include "RTSDKTypes.h"
#include "MassEntityTemplate.h"
#include "MassEntityConfigAsset.h"
#include "MassEntityTraitBase.h"
#include "RTSDKScriptExecutionContext.h"
#include "RTSDKUnitComponent.generated.h"

class URTSDKGameSimSubsystem;
class URTSDKUnitDefinition;
class ARTSDKForceStateBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FRTSDKOnUnitCollided, FRTSDKScriptExecutionContext, ScriptContext, FHitResult, TriggeringHit);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FRTSDKOnUnitHealthChanged, FRTSDKScriptExecutionContext, ScriptContext, URTSDKUnitComponent*, Sender, const FFixed64&, OldHealth, const FFixed64&, NewHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FRTSDKOnUnitMaxHealthChanged, FRTSDKScriptExecutionContext, ScriptContext, URTSDKUnitComponent*, Sender, const FFixed64&, OldHealth, const FFixed64&, NewHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FRTSDKOnUnitDamaged, FRTSDKScriptExecutionContext, ScriptContext, URTSDKUnitComponent*, Sender, const FRTSDKDamageInfo&, DamageInfo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FRTSDKOnUnitDied, FRTSDKScriptExecutionContext, ScriptContext, URTSDKUnitComponent*, Sender, const FRTSDKDamageInfo&, DamageInfo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FRTSDKOnUnitRemoved, FRTSDKScriptExecutionContext, ScriptContext, URTSDKUnitComponent*, Sender);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FRTSDKOnUnitDisplayNameChanged, URTSDKUnitComponent*, Sender, const FText&, NewDisplayName);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FRTSDKOnUnitIconChanged, URTSDKUnitComponent*, Sender, UMaterialInterface*, NewIcon);

/**
 * Component that provides script functionality and bridging Mass Entity and the Actor framework.
 * 
 * Script functionality comes in 3 flavours:
 * Events the sim may trigger at the end of a frame. See RTSDKBatchedSimCommands.h
 * Accessors that can pull values out of the Mass Entity database.
 * Mirrors of some infrequently changed data pertinent to player inputs and scripts
 * 
 * This component is often looked for in checks performed during player input.
 */
UCLASS(Blueprintable, ClassGroup = RTSDK, meta = (BlueprintSpawnableComponent), hidecategories = (Sockets, Collision))
class RTSDK_API URTSDKUnitComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	//virtual void OnRegister() override;

	virtual void Setup(URTSDKGameSimSubsystem* inSimSubsystem, uint32 inUnitID, int32 inForceID, UClass* inUnitDefintion, FMassEntityHandle inEntityHandle);

	/**
	* Gets the UnitID for this unit
	*/
	UFUNCTION(BlueprintPure)
	int64 GetUnitID() const;

	/**
	* UnitID for this unit
	*/
	UPROPERTY()
	uint32 UnitID;

	/**
	* ForceID for this unit
	*/
	UPROPERTY()
		uint32 ForceID;

	/**
	* Force State for this unit
	*/
	UPROPERTY()
		TObjectPtr<ARTSDKForceStateBase> ForceState;

	UPROPERTY()
		FMassEntityHandle UnitEntityHandle;

	/**
	* UnitID for this unit
	*/
	UPROPERTY()
	TSubclassOf<URTSDKUnitDefinition> UnitDefintion;

	UPROPERTY()
		TObjectPtr<URTSDKUnitDefinition> UnitDefintionCDO;

	UPROPERTY()
		TObjectPtr<UMaterialInterface> UnitIcon;

	UPROPERTY()
		FText UnitDisplayName;

	UPROPERTY()
		bool bUnitIsAlive;

	UPROPERTY(BlueprintAssignable)
	FRTSDKOnUnitCollided OnUnitCollided;

	UPROPERTY(BlueprintAssignable)
		FRTSDKOnUnitHealthChanged OnUnitHealthChanged;

	UPROPERTY(BlueprintAssignable)
		FRTSDKOnUnitMaxHealthChanged OnUnitMaxHealthChanged;

	UPROPERTY(BlueprintAssignable)
		FRTSDKOnUnitDamaged OnUnitDamaged;

	UPROPERTY(BlueprintAssignable)
		FRTSDKOnUnitDied OnUnitDied;

	UPROPERTY(BlueprintAssignable)
		FRTSDKOnUnitRemoved OnUnitRemoved;

	UPROPERTY(BlueprintAssignable)
		FRTSDKOnUnitDisplayNameChanged OnUnitDisplayNameChanged;

	UPROPERTY(BlueprintAssignable)
		FRTSDKOnUnitIconChanged OnUnitIconChanged;

	UFUNCTION(BlueprintPure)
		FVector GetUnitInput();

	UFUNCTION(BlueprintPure)
		FFixed64 GetUnitHealth();

	UFUNCTION(BlueprintPure)
		FFixed64 GetUnitMaxHealth();

	UFUNCTION(BlueprintPure)
		FText GetUnitDisplayName();

	UFUNCTION(BlueprintPure)
		UMaterialInterface* GetUnitIcon();

	UFUNCTION(BlueprintPure)
		bool GetUnitIsAlive();

	UFUNCTION(BlueprintCallable)
		void SetUnitInput(FRTSDKScriptExecutionContext inContext, FVector inDir);

	TObjectPtr<URTSDKGameSimSubsystem> OwningSim;
};
