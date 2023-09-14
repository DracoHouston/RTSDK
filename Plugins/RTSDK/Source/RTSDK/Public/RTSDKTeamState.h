// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Info.h"
#include "RTSDKTeamState.generated.h"

class ARTSDKForceStateBase;
class ARTSDKCommanderStateBase;
class ARTSDKSimStateBase;
class URTSDKGameSimSubsystem;

struct FRTSDKStateSetupInfo;

/**
 * 
 */
UCLASS(abstract)
class RTSDK_API ARTSDKTeamStateBase : public AInfo
{
	GENERATED_BODY()
	
public:

	ARTSDKTeamStateBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()); 

	UFUNCTION()
		virtual void Setup(const FRTSDKStateSetupInfo& inSetup, ARTSDKSimStateBase* inSimState, URTSDKGameSimSubsystem* inSimSubsystem);

	UFUNCTION()
		virtual FText GetDisplayName() { return FText(); }

	UFUNCTION()
		virtual void SetDisplayName(const FText& inName) {}

	UFUNCTION()
		virtual TArray<ARTSDKForceStateBase*> GetForces() { return TArray<ARTSDKForceStateBase*>(); }

	UFUNCTION()
		virtual void SetForces(TArray<ARTSDKForceStateBase*> inForces) {}

	UFUNCTION()
		virtual int32 AddForce(ARTSDKForceStateBase* inForce) { return -1; }

	UFUNCTION()
		virtual int32 GetForceCount() { return 0; }

protected:

		//Set on spawn, the sim subsystem this team belongs to.
		UPROPERTY(Transient)
			TObjectPtr<URTSDKGameSimSubsystem> SimSubsystem;

		//Set on spawn, the sim state this team belongs to.
		UPROPERTY(Transient)
			TObjectPtr<ARTSDKSimStateBase> SimState;
};

/**
 *
 */
UCLASS()
class RTSDK_API ARTSDKTeamStateSPOnly : public ARTSDKTeamStateBase
{
	GENERATED_BODY()

public:

	ARTSDKTeamStateSPOnly(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual FText GetDisplayName() override;
	virtual void SetDisplayName(const FText& inName) override;
	virtual TArray<ARTSDKForceStateBase*> GetForces() override;
	virtual void SetForces(TArray<ARTSDKForceStateBase*> inForces) override;
	virtual int32 AddForce(ARTSDKForceStateBase* inForce) override;
	virtual int32 GetForceCount() override;

protected:

	UPROPERTY(Transient)
		TArray<TObjectPtr<ARTSDKForceStateBase>> Forces;

	UPROPERTY(Transient)
		FText DisplayName;
};

/**
 *
 */
UCLASS()
class RTSDK_API ARTSDKTeamStateServerClientLockstep : public ARTSDKTeamStateBase
{
	GENERATED_BODY()

public:

	ARTSDKTeamStateServerClientLockstep(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual FText GetDisplayName() override;
	virtual void SetDisplayName(const FText& inName) override;
	virtual TArray<ARTSDKForceStateBase*> GetForces() override;
	virtual void SetForces(TArray<ARTSDKForceStateBase*> inForces) override;
	virtual int32 AddForce(ARTSDKForceStateBase* inForce) override;
	virtual int32 GetForceCount() override;

protected:

	UPROPERTY(Transient, Replicated)
		TArray<TObjectPtr<ARTSDKForceStateBase>> Forces;

	UPROPERTY(Transient, Replicated)
		FText DisplayName;
};

/**
 *
 */
UCLASS()
class RTSDK_API ARTSDKTeamStateServerClientCurves : public ARTSDKTeamStateBase
{
	GENERATED_BODY()

public:

	ARTSDKTeamStateServerClientCurves(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual FText GetDisplayName() override;
	virtual void SetDisplayName(const FText& inName) override;
	virtual TArray<ARTSDKForceStateBase*> GetForces() override;
	virtual void SetForces(TArray<ARTSDKForceStateBase*> inForces) override;
	virtual int32 AddForce(ARTSDKForceStateBase* inForce) override;
	virtual int32 GetForceCount() override;

protected:

	UPROPERTY(Transient, Replicated)
		TArray<TObjectPtr<ARTSDKForceStateBase>> Forces;

	UPROPERTY(Transient, Replicated)
		FText DisplayName;
};
