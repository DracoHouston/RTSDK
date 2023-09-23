// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RTSDKModDefinitionBase.generated.h"

class URTSDKModManager;
class URTSDKGameFeatureData;
class URTSDKModDefinitionBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRTSDKOnModFullyLoaded, URTSDKModDefinitionBase*, Sender);

UENUM()
enum class ERTSDKModStates : uint8
{
	Invalid,//failed build/build not started
	Building,//initial state
	Registered,//>= Registered implies valid mod
	Unloading,
	Loading,
	Loaded,//>= Loaded implies loaded mod
	Deactivating,
	Activating,
	Activated //fully activated mod
};

USTRUCT(BlueprintType)
struct RTSDK_API FRTSDKModDependencyInfo
{
public:
	GENERATED_BODY()

public:

	UPROPERTY()
	FName ModType;

	UPROPERTY()
		FName ModDevName;

	UPROPERTY()
		TWeakObjectPtr<URTSDKModDefinitionBase> ModDefinition;

};

/**
 * Base class for mod defs
 */
UCLASS()
class RTSDK_API URTSDKModDefinitionBase : public UObject
{
	GENERATED_BODY()
	
public:

	UFUNCTION()
		virtual void Init(const URTSDKGameFeatureData* inData);

	UFUNCTION()
		virtual void BuildModDependencies(URTSDKModManager* inModManager);

	UFUNCTION()
		virtual void BuildMod(URTSDKModManager* inModManager);

	UFUNCTION()
		virtual void LoadMod(UObject* inCallerObject);

	UFUNCTION()
		virtual void UnloadMod(UObject* inCallerObject);

	UFUNCTION()
		virtual void ActivateMod(UObject* inCallerObject);

	UFUNCTION()
		virtual void DeactivateMod(UObject* inCallerObject);

	UFUNCTION()
		virtual bool IsModValid();

	UFUNCTION()
		virtual bool IsModLoaded();

	UFUNCTION()
		virtual bool IsModUnloaded();

	UFUNCTION()
		virtual bool IsModActivated();

	UFUNCTION()
		virtual bool IsModDeactivated();

	UFUNCTION()
		virtual bool IsModAbstract();

	UFUNCTION()
		virtual URTSDKModDefinitionBase* GetParentMod();

	UFUNCTION()
		virtual TArray<URTSDKModDefinitionBase*> GetModDependencies();

	UFUNCTION()
		virtual FName GetModType();

	UFUNCTION()
		void DependencyLoaded(URTSDKModDefinitionBase* Sender);

	UFUNCTION()
		void DependencyUnloaded(URTSDKModDefinitionBase* Sender);

	UFUNCTION()
		void DependencyActivated(URTSDKModDefinitionBase* Sender);

	UFUNCTION()
		void DependencyDeactivated(URTSDKModDefinitionBase* Sender);

	UFUNCTION()
		void NotifyQueuedActivation();

	UFUNCTION()
		void NotifyQueuedDeactivation();

	UPROPERTY(BlueprintAssignable)
		FRTSDKOnModFullyLoaded OnModFullyLoaded;

	UPROPERTY(BlueprintAssignable)
		FRTSDKOnModFullyLoaded OnModFullyUnloaded;

	UPROPERTY(BlueprintAssignable)
		FRTSDKOnModFullyLoaded OnModFullyActivated;

	UPROPERTY(BlueprintAssignable)
		FRTSDKOnModFullyLoaded OnModFullyDeactivated;

	UPROPERTY(transient)
		FText DisplayName;

	UPROPERTY(transient)
		FName DevName;

	UPROPERTY(transient)
		FName ModTypeName;

	UPROPERTY(transient)
		FString GameFeatureName;

	UPROPERTY(transient)
		FString GameFeatureURL;

	UPROPERTY(transient)
		TArray<FRTSDKModDependencyInfo> ModDependencyInfos;

	UPROPERTY(transient)
		FRTSDKModDependencyInfo ParentModInfo;

	UPROPERTY(transient)
		bool bIsValid;

	UPROPERTY(transient)
		bool bIsAbstractMod;

	UPROPERTY(transient)
		bool bIsFullyLoaded;

	UPROPERTY(transient)
		bool bIsActivated;

	UPROPERTY(transient)
		ERTSDKModStates ModState;

	/*
	* This array keeps track of callers for load and unload
	* This mod can't enter unloaded/registered state while this has non-zero elements
	*/
	UPROPERTY(transient)
		TArray<UObject*> LoadDependingObjects;

	/*
	* This array keeps track of callers for activate and deactivate
	* This mod can't enter inactive/loaded state while this has non-zero elements
	*/
	UPROPERTY(transient)
		TArray<UObject*> ActivateDependingObjects;
};

/**
 * Base class for mod defs that have association with a mod
 * child classes are responsible for setting the associated mod's type to search for during their Init override
 */
UCLASS()
class RTSDK_API URTSDKAssociatedModDefinitionBase : public URTSDKModDefinitionBase
{
	GENERATED_BODY()

public:

	virtual void Init(const URTSDKGameFeatureData* inData) override;

	UFUNCTION()
		virtual URTSDKModDefinitionBase* GetAssociatedMod();

	UPROPERTY()
		FRTSDKModDependencyInfo AssociatedModInfo;
};
