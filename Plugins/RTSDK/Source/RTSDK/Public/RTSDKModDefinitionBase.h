// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFeatureData.h"
#include "RTSDKModDefinitionBase.generated.h"

class URTSDKModManager;
class URTSDKGameFeatureData;
class URTSDKModDefinitionBase;
class UWorld;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FRTSDKOnModStateChanged, UWorld*, WorldContext, URTSDKModDefinitionBase*, Sender);

UENUM()
enum class ERTSDKModStates : uint8
{
	Invalid,//failed build/build not started
	Building,//initial state
	Registered,//>= Registered implies valid mod
	Unloading,
	Loading,
	Loaded //ready for use
};

UENUM()
enum class ERTSDKWorldModStates : uint8
{
	Inactive,
	Deactivating,
	Activating,
	Activated
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

USTRUCT(BlueprintType)
struct RTSDK_API FRTSDKModDependentsArray
{
public:
	GENERATED_BODY()

public:

	UPROPERTY()
		TArray<UObject*> Dependents;

};

/**
 * Base class for mod defs
 */
UCLASS()
class RTSDK_API URTSDKModDefinitionBase : public UGameFeatureData
{
	GENERATED_BODY()
	
public:

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

	UPROPERTY(EditDefaultsOnly)
		TArray<TSoftObjectPtr<URTSDKModDefinitionBase>> FeatureDependencies;	

	UPROPERTY(EditDefaultsOnly)
		TSoftObjectPtr<URTSDKModDefinitionBase> ParentMod;
//#if !WITH_EDITORONLY_DATA
	UPROPERTY(transient)
		TObjectPtr<URTSDKModDefinitionBase> ParentModPtr;

	UPROPERTY(transient)
		TArray<TObjectPtr<URTSDKModDefinitionBase>> FeatureDependencyPtrs;
//#endif
	UPROPERTY(EditDefaultsOnly)
		bool bIsAbstractMod;

	UFUNCTION()
		virtual void InitMod(URTSDKModManager* inModManager);

	UFUNCTION()
		virtual void BuildModDependencies(URTSDKModManager* inModManager);

	UFUNCTION()
		virtual void BuildMod(URTSDKModManager* inModManager);

	UFUNCTION()
		virtual void LoadMod(UWorld* inWorldContext, UObject* inCallerObject);

	UFUNCTION()
		virtual void UnloadMod(UWorld* inWorldContext, UObject* inCallerObject);

	UFUNCTION()
		virtual void ActivateMod(UWorld* inWorldContext, UObject* inCallerObject);

	UFUNCTION()
		virtual void DeactivateMod(UWorld* inWorldContext, UObject* inCallerObject);

	UFUNCTION()
		virtual bool IsModValid();

	UFUNCTION()
		virtual bool IsModLoaded();

	UFUNCTION()
		virtual bool IsModUnloaded();

	UFUNCTION()
		virtual bool IsModActivated(UWorld* WorldContext);

	UFUNCTION()
		virtual bool IsModDeactivated(UWorld* WorldContext);

	UFUNCTION()
		virtual bool IsModAbstract();

	UFUNCTION()
		virtual URTSDKModDefinitionBase* GetParentMod();

	UFUNCTION()
		virtual TArray<URTSDKModDefinitionBase*> GetModDependencies();

	UFUNCTION()
		virtual FName GetModType() const;

	UFUNCTION()
		void DependencyLoaded(UWorld* WorldContext, URTSDKModDefinitionBase* Sender);

	UFUNCTION()
		void DependencyUnloaded(UWorld* WorldContext, URTSDKModDefinitionBase* Sender);

	/*UFUNCTION()
		void DependencyActivated(UWorld* WorldContext, URTSDKModDefinitionBase* Sender);

	UFUNCTION()
		void DependencyDeactivated(UWorld* WorldContext, URTSDKModDefinitionBase* Sender);*/

	UPROPERTY(BlueprintAssignable)
		FRTSDKOnModStateChanged OnModFullyLoaded;

	UPROPERTY(BlueprintAssignable)
		FRTSDKOnModStateChanged OnModFullyUnloaded;

	UPROPERTY(BlueprintAssignable)
		FRTSDKOnModStateChanged OnModFullyActivated;

	UPROPERTY(BlueprintAssignable)
		FRTSDKOnModStateChanged OnModFullyDeactivated;

	UPROPERTY(transient)
		FString GameFeatureName;

	UPROPERTY(transient)
		FString GameFeatureURL;

	UPROPERTY(transient)
		TMap<UWorld*, ERTSDKWorldModStates> ModWorldStates;

	UPROPERTY(transient)
		ERTSDKModStates ModState;

	/*
	* This array keeps track of callers for load and unload
	* This mod can't enter unloaded/registered state while this has non-zero elements
	*/
	UPROPERTY(transient)
		TMap<UWorld*, FRTSDKModDependentsArray> LoadDependingObjects;

	/*
	* This array keeps track of callers for activate and deactivate
	* This mod can't enter inactive/loaded state while this has non-zero elements
	*/
	UPROPERTY(transient)
		TMap<UWorld*, FRTSDKModDependentsArray> ActivateDependingObjects;

	protected:

		/*UFUNCTION()
			virtual void Internal_ActivateInWorld(UWorld* WorldContext);*/
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
	virtual void BuildModDependencies(URTSDKModManager* inModManager) override;

	UFUNCTION()
		virtual URTSDKModDefinitionBase* GetAssociatedMod();

	UFUNCTION()
		virtual FName GetAllowedAssociatedModType() const;

	UPROPERTY(EditDefaultsOnly)
		TSoftObjectPtr<URTSDKModDefinitionBase> AssociatedMod;
//#if !WITH_EDITORONLY_DATA
	UPROPERTY(transient)
		TObjectPtr<URTSDKModDefinitionBase> AssociatedModPtr;
//#endif
};
