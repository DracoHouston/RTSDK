// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Info.h"
#include "RTSDKForceState.generated.h"

class ARTSDKTeamStateBase;
class ARTSDKSimStateBase;
class ARTSDKCommanderStateBase;
class URTSDKGameSimSubsystem;

/**
 * 
 */
UCLASS(abstract)
class RTSDK_API ARTSDKForceStateBase : public AInfo
{
	GENERATED_BODY()

public:

	ARTSDKForceStateBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()); 
	
	UFUNCTION()
		virtual void Setup(const FRTSDKStateSetupInfo& inSetup, ARTSDKSimStateBase* inSimState, URTSDKGameSimSubsystem* inSimSubsystem);

	UFUNCTION()
		virtual FText GetDisplayName() { return FText(); }

	UFUNCTION()
		virtual void SetDisplayName(const FText& inName) {}

	UFUNCTION()
		virtual ARTSDKTeamStateBase* GetTeam() { return nullptr; }

	UFUNCTION()
		virtual void SetTeam(ARTSDKTeamStateBase* inTeam) {}

	UFUNCTION()
		virtual TArray<ARTSDKCommanderStateBase*> GetCommanders() { return TArray<ARTSDKCommanderStateBase*>(); }

	UFUNCTION()
		virtual ARTSDKCommanderStateBase* GetCommanderByIndex(int32 CommanderIndex) { return nullptr; }

	UFUNCTION()
		virtual void SetCommanders(TArray<ARTSDKCommanderStateBase*> inCommanders) {}

	UFUNCTION()
		virtual int32 AddCommander(ARTSDKCommanderStateBase* inCommander) { return -1; }

	UFUNCTION()
		virtual int32 GetCommanderCount() { return 0; }

protected:

	//Set on spawn, the sim subsystem this force belongs to.
	UPROPERTY(Transient)
		TWeakObjectPtr<URTSDKGameSimSubsystem> SimSubsystem;

	//Set on spawn, the sim state this force belongs to.
	UPROPERTY(Transient)
		TWeakObjectPtr<ARTSDKSimStateBase> SimState;
};

/**
 *
 */
UCLASS()
class RTSDK_API ARTSDKForceStateSPOnly : public ARTSDKForceStateBase
{
	GENERATED_BODY()

public:

	ARTSDKForceStateSPOnly(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual FText GetDisplayName() override;
	virtual void SetDisplayName(const FText& inName) override;
	virtual ARTSDKTeamStateBase* GetTeam() override;
	virtual void SetTeam(ARTSDKTeamStateBase* inTeam) override;
	virtual TArray<ARTSDKCommanderStateBase*> GetCommanders() override;
	virtual ARTSDKCommanderStateBase* GetCommanderByIndex(int32 CommanderIndex) override;
	virtual void SetCommanders(TArray<ARTSDKCommanderStateBase*> inCommanders) override;
	virtual int32 AddCommander(ARTSDKCommanderStateBase* inCommander) override;
	virtual int32 GetCommanderCount() override;


protected:

	UPROPERTY(Transient)
	TArray<TObjectPtr<ARTSDKCommanderStateBase>> Commanders;

	UPROPERTY(Transient)
	TObjectPtr<ARTSDKTeamStateBase> Team;

	UPROPERTY(Transient)
		FText DisplayName;

};

/**
 *
 */
UCLASS()
class RTSDK_API ARTSDKForceStateServerClientLockstep : public ARTSDKForceStateBase
{
	GENERATED_BODY()

public:

	ARTSDKForceStateServerClientLockstep(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual FText GetDisplayName() override;
	virtual void SetDisplayName(const FText& inName) override;
	virtual ARTSDKTeamStateBase* GetTeam() override;
	virtual void SetTeam(ARTSDKTeamStateBase* inTeam) override;
	virtual TArray<ARTSDKCommanderStateBase*> GetCommanders() override;
	virtual ARTSDKCommanderStateBase* GetCommanderByIndex(int32 CommanderIndex) override;
	virtual void SetCommanders(TArray<ARTSDKCommanderStateBase*> inCommanders) override;
	virtual int32 AddCommander(ARTSDKCommanderStateBase* inCommander) override;
	virtual int32 GetCommanderCount() override;

protected:

	UPROPERTY(Transient, Replicated)
		TArray<TObjectPtr<ARTSDKCommanderStateBase>> Commanders;

	UPROPERTY(Transient, Replicated)
		TObjectPtr<ARTSDKTeamStateBase> Team;

	UPROPERTY(Transient, Replicated)
		FText DisplayName;
};

/**
 *
 */
UCLASS()
class RTSDK_API ARTSDKForceStateServerClientCurves : public ARTSDKForceStateBase
{
	GENERATED_BODY()

public:

	ARTSDKForceStateServerClientCurves(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual FText GetDisplayName() override;
	virtual void SetDisplayName(const FText& inName) override;
	virtual ARTSDKTeamStateBase* GetTeam() override;
	virtual void SetTeam(ARTSDKTeamStateBase* inTeam) override;
	virtual TArray<ARTSDKCommanderStateBase*> GetCommanders() override;
	virtual ARTSDKCommanderStateBase* GetCommanderByIndex(int32 CommanderIndex) override;
	virtual void SetCommanders(TArray<ARTSDKCommanderStateBase*> inCommanders) override;
	virtual int32 AddCommander(ARTSDKCommanderStateBase* inCommander) override;
	virtual int32 GetCommanderCount() override;

protected:

	UPROPERTY(Transient, Replicated)
		TArray<TObjectPtr<ARTSDKCommanderStateBase>> Commanders;

	UPROPERTY(Transient, Replicated)
		TObjectPtr<ARTSDKTeamStateBase> Team;

	UPROPERTY(Transient, Replicated)
		FText DisplayName;
};
