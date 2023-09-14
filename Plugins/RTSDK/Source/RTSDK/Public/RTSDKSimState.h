// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Info.h"
#include "RTSDKPlayerCommand.h"
#include "RTSDKLaunchOptionsHelpers.h"
#include "RTSDKSimState.generated.h"

class ARTSDKPlayerController;
class ARTSDKCommanderStateBase;
class ARTSDKForceStateBase;
class ARTSDKTeamStateBase;
class URTSDKGameSimSubsystem;

UENUM()
enum class ERTSDKShouldAdvanceInputTurnResult : uint8
{
	Skip,
	Advance,
	Wait
};

UENUM()
enum class ERTSDKPreMatchTickResult : uint8
{
	Waiting,
	Ready
};

struct FRTSDKLockstepPauseCommands;

USTRUCT()
struct RTSDK_API FRTSDKLockstepPauseCommand : public FFastArraySerializerItem
{
	GENERATED_BODY()

	UPROPERTY()
	int32 Turn;

	UPROPERTY()
		bool bDoPause;

	void PreReplicatedRemove(const FRTSDKLockstepPauseCommands& InArraySerializer);
	void PostReplicatedAdd(const FRTSDKLockstepPauseCommands& InArraySerializer);
	void PostReplicatedChange(const FRTSDKLockstepPauseCommands& InArraySerializer);

};

USTRUCT()
struct RTSDK_API FRTSDKLockstepPauseCommands : public FFastArraySerializer
{
	GENERATED_BODY()

	void AddPauseCommand(int32 inTurn, bool inDoPause);

	bool HasPauseCommandOnTurn(int32 inTurn);

	bool HasTurn(int32 inTurn);

	UPROPERTY()
		TArray<FRTSDKLockstepPauseCommand> PauseCommands;

	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms)
	{
		return FFastArraySerializer::FastArrayDeltaSerialize<FRTSDKLockstepPauseCommand, FRTSDKLockstepPauseCommands>(PauseCommands, DeltaParms, *this);
	}
};

/** Specified to allow fast TArray replication */
template<>
struct TStructOpsTypeTraits<FRTSDKLockstepPauseCommands> : public TStructOpsTypeTraitsBase2<FRTSDKLockstepPauseCommands>
{
	enum
	{
		WithNetDeltaSerializer = true,
	};
};

struct FRTSDKLockstepTimescaleCommands;

USTRUCT()
struct RTSDK_API FRTSDKLockstepTimescaleCommand : public FFastArraySerializerItem
{
	GENERATED_BODY()

		UPROPERTY()
		int32 Turn;

	UPROPERTY()
		FFixed64 Timescale;

	void PreReplicatedRemove(const FRTSDKLockstepTimescaleCommands& InArraySerializer);
	void PostReplicatedAdd(const FRTSDKLockstepTimescaleCommands& InArraySerializer);
	void PostReplicatedChange(const FRTSDKLockstepTimescaleCommands& InArraySerializer);

};

USTRUCT()
struct RTSDK_API FRTSDKLockstepTimescaleCommands : public FFastArraySerializer
{
	GENERATED_BODY()

	void AddTimescaleCommand(int32 inTurn, FFixed64 inTimescale);
	void AddEmptyTimescaleCommand(int32 inTurn);

	bool HasTimescaleCommandOnTurn(int32 inTurn, FFixed64& outTimescale);

	bool HasTurn(int32 inTurn);

	UPROPERTY()
		TArray<FRTSDKLockstepTimescaleCommand> TimescaleCommands;

	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms)
	{
		return FFastArraySerializer::FastArrayDeltaSerialize<FRTSDKLockstepTimescaleCommand, FRTSDKLockstepTimescaleCommands>(TimescaleCommands, DeltaParms, *this);
	}
};

/** Specified to allow fast TArray replication */
template<>
struct TStructOpsTypeTraits<FRTSDKLockstepTimescaleCommands> : public TStructOpsTypeTraitsBase2<FRTSDKLockstepTimescaleCommands>
{
	enum
	{
		WithNetDeltaSerializer = true,
	};
};

struct FRTSDKLockstepTurnDurationCommands;

USTRUCT()
struct RTSDK_API FRTSDKLockstepTurnDurationCommand : public FFastArraySerializerItem
{
	GENERATED_BODY()

		UPROPERTY()
		int32 Turn;

	UPROPERTY()
		FFixed64 TurnDuration;

	void PreReplicatedRemove(const FRTSDKLockstepTurnDurationCommands& InArraySerializer);
	void PostReplicatedAdd(const FRTSDKLockstepTurnDurationCommands& InArraySerializer);
	void PostReplicatedChange(const FRTSDKLockstepTurnDurationCommands& InArraySerializer);

};

USTRUCT()
struct RTSDK_API FRTSDKLockstepTurnDurationCommands : public FFastArraySerializer
{
	GENERATED_BODY()

		void AddTurnDurationCommand(int32 inTurn, FFixed64 inTurnDuration);
	void AddEmptyTurnDurationCommand(int32 inTurn);

	bool HasTurnDurationCommandOnTurn(int32 inTurn, FFixed64& outTurnDuration);

	bool HasTurn(int32 inTurn);

	UPROPERTY()
		TArray<FRTSDKLockstepTurnDurationCommand> TurnDurationCommands;

	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms)
	{
		return FFastArraySerializer::FastArrayDeltaSerialize<FRTSDKLockstepTurnDurationCommand, FRTSDKLockstepTurnDurationCommands>(TurnDurationCommands, DeltaParms, *this);
	}
};

/** Specified to allow fast TArray replication */
template<>
struct TStructOpsTypeTraits<FRTSDKLockstepTurnDurationCommands> : public TStructOpsTypeTraitsBase2<FRTSDKLockstepTurnDurationCommands>
{
	enum
	{
		WithNetDeltaSerializer = true,
	};
};

/**
 * Info actor for the RTSDK Game Sim Subsystem.
 * Abstract class, child classes handle different network environments
 */
UCLASS(abstract)
class RTSDK_API ARTSDKSimStateBase : public AInfo
{
	GENERATED_BODY()

public:

	ARTSDKSimStateBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION()
		virtual void PlayerSetup();

	UFUNCTION()
		virtual bool GetIsPlayerSetup();

	UFUNCTION()
	virtual void Setup(URTSDKGameSimSubsystem* inSimSubsystem, UWorld* inWorld);

	UFUNCTION()
		virtual void SetupTeams(const TMap<int32,FRTSDKStateSetupInfo>& inOptionsMap) {}

	UFUNCTION()
		virtual void SetupForces(const TMap<int32, FRTSDKStateSetupInfo>& inOptionsMap) {}

	UFUNCTION()
		virtual void SetupCommanders(const TMap<int32, FRTSDKStateSetupInfo>& inOptionsMap) {}

	UFUNCTION()
		virtual void PreMatchTick() {}

	UFUNCTION()
		virtual void SetMatchHasStarted(bool inMatchHasStarted) {}

	UFUNCTION()
		virtual bool GetMatchHasStarted() { return false; }

	UFUNCTION()
		virtual void SetMatchIsPaused(bool inMatchIsPaused) {}

	UFUNCTION()
		virtual bool GetMatchIsPaused() { return false; }

	/**
	* Virtual function that performs checks for auto unpause while match is paused.
	* Returns false by default, overrides to this can handle logic for auto unpause systems.
	*/
	UFUNCTION()
		virtual bool ShouldAutoUnpause() { return false; }

	UFUNCTION()
		virtual void RequestPause(AController* inController) {}

	UFUNCTION()
		virtual void RequestUnpause(AController* inController) {}

	UFUNCTION()
		virtual void RequestTimescale(AController* inController, const FFixed64& inTimescale) {}

	UFUNCTION()
		virtual ERTSDKShouldAdvanceInputTurnResult ShouldAdvanceInputTurn() { return ERTSDKShouldAdvanceInputTurnResult::Skip; }

	UFUNCTION()
		virtual void OnPreAdvanceInputTurn() {}

	UFUNCTION()
	virtual ARTSDKCommanderStateBase* GetCommander(const int32& inCommanderID) { return nullptr; }

	UFUNCTION()
	virtual TArray<ARTSDKCommanderStateBase*> GetCommanders() { return TArray<ARTSDKCommanderStateBase*>(); }

	UFUNCTION()
		virtual int32 AddCommander(ARTSDKCommanderStateBase* inCommanderState) { return -1; }

	UFUNCTION()
		virtual void SetCommanders(TArray<ARTSDKCommanderStateBase*> inCommanderStates) {}

	UFUNCTION()
		virtual int32 GetCommanderCount() { return 0; }

	UFUNCTION()
		virtual ARTSDKForceStateBase* GetForce(const int32& inForceID) { return nullptr; }
	
	UFUNCTION()
		virtual TArray<ARTSDKForceStateBase*> GetForces() { return TArray<ARTSDKForceStateBase*>(); }

	UFUNCTION()
		virtual int32 AddForce(ARTSDKForceStateBase* inForceState) { return -1; }

	UFUNCTION()
		virtual void SetForces(TArray<ARTSDKForceStateBase*> inForceStates) {}

	UFUNCTION()
		virtual int32 GetForceCount() { return 0; }

	UFUNCTION()
		virtual ARTSDKTeamStateBase* GetTeam(const int32& inTeamID) { return nullptr; }

	UFUNCTION()
		virtual TArray<ARTSDKTeamStateBase*> GetTeams() { return TArray<ARTSDKTeamStateBase*>(); }

	UFUNCTION()
		virtual int32 AddTeam(ARTSDKTeamStateBase* inTeamState) { return -1; }

	UFUNCTION()
		virtual void SetTeams(TArray<ARTSDKTeamStateBase*> inTeamStates) {}

	UFUNCTION()
		virtual int32 GetTeamCount() { return 0; }

	UFUNCTION()
	virtual TArray<FRTSDKPlayerCommandReplicationInfo> GetCommandsForCommanderByTurn(const int32& inCommanderID, const int32& inTurn) 
	{ 
		return TArray<FRTSDKPlayerCommandReplicationInfo>(); 
	}

	virtual void BeginPlay() override;

protected:

	UPROPERTY(Transient)
	TObjectPtr<URTSDKGameSimSubsystem> SimSubsystem;

	UPROPERTY(Transient)
		FString OptionsString;

	UPROPERTY(Transient)
		TMap<FString, FString> OptionsMap;

	UPROPERTY(Transient)
		TObjectPtr<ARTSDKCommanderStateBase> LocalCommander;

	UPROPERTY(Transient)
		TObjectPtr<APlayerController> LocalPlayerController;
};

/**
 * RTSDK Sim State SP Only, the singleplayer only version of RTSDK Sim State Base.
 * No networking. Command turns happen every frame.
 */
UCLASS()
class RTSDK_API ARTSDKSimStateSPOnly : public ARTSDKSimStateBase
{
	GENERATED_BODY()

public:

	ARTSDKSimStateSPOnly(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//RTSDKSimState interface
	virtual void SetupTeams(const TMap<int32, FRTSDKStateSetupInfo>& inOptionsMap) override;
	virtual void SetupForces(const TMap<int32, FRTSDKStateSetupInfo>& inOptionsMap) override;
	virtual void SetupCommanders(const TMap<int32, FRTSDKStateSetupInfo>& inOptionsMap) override;
	virtual void PreMatchTick() override; 
	virtual void SetMatchHasStarted(bool inMatchHasStarted) override;
	virtual bool GetMatchHasStarted() override;
	virtual void SetMatchIsPaused(bool inMatchIsPaused) override;
	virtual bool GetMatchIsPaused() override;
	virtual void RequestPause(AController* inController) override;
	virtual void RequestUnpause(AController* inController) override;
	virtual void RequestTimescale(AController* inController, const FFixed64& inTimescale) override;
	virtual ERTSDKShouldAdvanceInputTurnResult ShouldAdvanceInputTurn() override;
	virtual void OnPreAdvanceInputTurn() override;
	virtual ARTSDKCommanderStateBase* GetCommander(const int32& inCommanderID) override;
	virtual TArray<ARTSDKCommanderStateBase*> GetCommanders() override;
	virtual int32 AddCommander(ARTSDKCommanderStateBase* inCommanderState) override;
	virtual void SetCommanders(TArray<ARTSDKCommanderStateBase*> inCommanderStates) override;
	virtual int32 GetCommanderCount() override;
	virtual ARTSDKForceStateBase* GetForce(const int32& inForceID) override;
	virtual TArray<ARTSDKForceStateBase*> GetForces() override;
	virtual int32 AddForce(ARTSDKForceStateBase* inForceState) override;
	virtual void SetForces(TArray<ARTSDKForceStateBase*> inForceStates) override;
	virtual int32 GetForceCount() override;
	virtual ARTSDKTeamStateBase* GetTeam(const int32& inTeamID) override;
	virtual TArray<ARTSDKTeamStateBase*> GetTeams() override;
	virtual int32 AddTeam(ARTSDKTeamStateBase* inTeamState) override;
	virtual void SetTeams(TArray<ARTSDKTeamStateBase*> inTeamStates) override;
	virtual int32 GetTeamCount() override;
	virtual TArray<FRTSDKPlayerCommandReplicationInfo> GetCommandsForCommanderByTurn(const int32& inCommanderID, const int32& inTurn) override;
	//~RTSDKSimState interface

protected:

	UPROPERTY(Transient)
		TArray<TObjectPtr<ARTSDKCommanderStateBase>> Commanders;

	UPROPERTY(Transient)
		TArray<TObjectPtr<ARTSDKForceStateBase>> Forces;

	UPROPERTY(Transient)
		TArray<TObjectPtr<ARTSDKTeamStateBase>> Teams;

	UPROPERTY(Transient)
		bool bMatchHasStarted;

	UPROPERTY(Transient)
		bool bMatchIsPaused;
};

/**
 * RTSDK Sim State Server Client Lockstep, a multiplayer version of RTSDKSimStateBase
 * Performs lockstep networking via Unreal's built in server-client replication.
 * This networking method is useful for games that wish to run dedicated servers
 * that can record replays. Sim runs on all machines. Command turns frame duration is
 * dependent on highest client ping. Commands replicate to all clients, commands are sent to the server.
 */
UCLASS()
class RTSDK_API ARTSDKSimStateServerClientLockstep : public ARTSDKSimStateBase
{
	GENERATED_BODY()

public:

	ARTSDKSimStateServerClientLockstep(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//RTSDKSimState interface
	virtual bool GetIsPlayerSetup() override;
	virtual void Setup(URTSDKGameSimSubsystem* inSimSubsystem, UWorld* inWorld) override;
	virtual void SetupTeams(const TMap<int32, FRTSDKStateSetupInfo>& inOptionsMap) override;
	virtual void SetupForces(const TMap<int32, FRTSDKStateSetupInfo>& inOptionsMap) override;
	virtual void SetupCommanders(const TMap<int32, FRTSDKStateSetupInfo>& inOptionsMap) override;
	virtual void PreMatchTick() override;
	virtual void SetMatchHasStarted(bool inMatchHasStarted) override;
	virtual bool GetMatchHasStarted() override;
	virtual void SetMatchIsPaused(bool inMatchIsPaused) override;
	virtual bool GetMatchIsPaused() override;
	virtual void RequestPause(AController* inController) override;
	virtual void RequestUnpause(AController* inController) override;
	virtual void RequestTimescale(AController* inController, const FFixed64& inTimescale) override;
	virtual ERTSDKShouldAdvanceInputTurnResult ShouldAdvanceInputTurn() override;
	virtual void OnPreAdvanceInputTurn() override;
	virtual ARTSDKCommanderStateBase* GetCommander(const int32& inCommanderID) override;
	virtual TArray<ARTSDKCommanderStateBase*> GetCommanders() override;
	virtual int32 AddCommander(ARTSDKCommanderStateBase* inCommanderState) override;
	virtual void SetCommanders(TArray<ARTSDKCommanderStateBase*> inCommanderStates) override;
	virtual int32 GetCommanderCount() override;
	virtual ARTSDKForceStateBase* GetForce(const int32& inForceID) override;
	virtual TArray<ARTSDKForceStateBase*> GetForces() override;
	virtual int32 AddForce(ARTSDKForceStateBase* inForceState) override;
	virtual void SetForces(TArray<ARTSDKForceStateBase*> inForceStates) override;
	virtual int32 GetForceCount() override;
	virtual ARTSDKTeamStateBase* GetTeam(const int32& inTeamID) override;
	virtual TArray<ARTSDKTeamStateBase*> GetTeams() override;
	virtual int32 AddTeam(ARTSDKTeamStateBase* inTeamState) override;
	virtual void SetTeams(TArray<ARTSDKTeamStateBase*> inTeamStates) override;
	virtual int32 GetTeamCount() override;
	virtual TArray<FRTSDKPlayerCommandReplicationInfo> GetCommandsForCommanderByTurn(const int32& inCommanderID, const int32& inTurn) override;
	//~RTSDKSimState interface

	UFUNCTION()
		virtual bool PlayerMayRequestPause(AController* inController, ARTSDKCommanderStateBase*& outCommander);

	UFUNCTION()
		virtual bool PlayerMayRequestUnpause(AController* inController, ARTSDKCommanderStateBase*& outCommander);

	UFUNCTION()
		virtual bool PlayerMayRequestTimescale(AController* inController, const FFixed64& inTimescale);

	UFUNCTION(NetMulticast, Reliable)
		void Multicast_OnUnpauseMatch();

	UFUNCTION()
		virtual void AdvancePauseCommands();

	UFUNCTION()
		virtual void AdvanceTimescaleCommands();

	UFUNCTION()
		virtual void AdvanceTurnDurationCommands();

protected:

	int32 CalculateFramesPerTurn();

	FFixed64 CalculateTurnDuration();

	UPROPERTY(Transient)
	int32 LastTurnFrame;

	UPROPERTY(Transient)
	int32 FramesPerTurn;

	UPROPERTY(Transient, Replicated)
		int32 LockstepTimeoutTurnCount;

	UPROPERTY(Transient)
		FFixed64 TurnDuration;

	UPROPERTY(Transient)
	FFixed64 MinTurnDuration;

	UPROPERTY(Transient, Replicated)
		FFixed64 InitialTurnDuration;

	UPROPERTY(Transient)
		FFixed64 DesiredTimescale;

	UPROPERTY(Transient, Replicated)
		TArray<TObjectPtr<ARTSDKCommanderStateBase>> Commanders;

	UPROPERTY(Transient, Replicated)
		TArray<TObjectPtr<ARTSDKForceStateBase>> Forces;

	UPROPERTY(Transient, Replicated)
		TArray<TObjectPtr<ARTSDKTeamStateBase>> Teams;

	UPROPERTY(Transient, Replicated)
		bool bMatchHasStarted;

	//happens on commands executing in lockstep, so not replicated.
	UPROPERTY(Transient)
		bool bMatchIsPaused;

	UPROPERTY(Transient, Replicated)
		TArray<TObjectPtr<ARTSDKCommanderStateBase>> Pausers;

	UPROPERTY(Transient, Replicated)
		FRTSDKLockstepPauseCommands PauseCommands;

	UPROPERTY(Transient, Replicated)
		FRTSDKLockstepTimescaleCommands TimescaleCommands;

	UPROPERTY(Transient, Replicated)
		FRTSDKLockstepTurnDurationCommands TurnDurationCommands;
};

/**
 * RTSDK Sim State Server Client Curves, a multiplayer version of RTSDKSimStateBase
 * Performs keyframed curve based networking via Unreal's built in server-client replication.
 * This networking method is useful for games that wish to have smooth direct player control
 * of units.
 * Curve replication sends keyframes for all relevant unit's properties to clients, the sim runs
 * only on the server. For most properties, values can be interpolated between keyframes. 
 * Keyframes are predicted. Command turns happen every sim frame. Commands are not replicated back 
 * to clients, only sent to server.
 */
UCLASS()
class RTSDK_API ARTSDKSimStateServerClientCurves : public ARTSDKSimStateBase
{
	GENERATED_BODY()

public:

	ARTSDKSimStateServerClientCurves(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//RTSDKSimState interface
	/*virtual void PlayerSetup() override;
	virtual bool GetIsPlayerSetup() override;*/
	virtual void SetupTeams(const TMap<int32, FRTSDKStateSetupInfo>& inOptionsMap) override;
	virtual void SetupForces(const TMap<int32, FRTSDKStateSetupInfo>& inOptionsMap) override;
	virtual void SetupCommanders(const TMap<int32, FRTSDKStateSetupInfo>& inOptionsMap) override;
	virtual void SetMatchHasStarted(bool inMatchHasStarted) override;
	virtual bool GetMatchHasStarted() override;
	virtual void SetMatchIsPaused(bool inMatchIsPaused) override;
	virtual bool GetMatchIsPaused() override;
	virtual void RequestPause(AController* inController) override;
	virtual void RequestUnpause(AController* inController) override;
	virtual ERTSDKShouldAdvanceInputTurnResult ShouldAdvanceInputTurn() override;
	virtual void OnPreAdvanceInputTurn() override;
	virtual ARTSDKCommanderStateBase* GetCommander(const int32& inCommanderID) override;
	virtual TArray<ARTSDKCommanderStateBase*> GetCommanders() override;
	virtual int32 AddCommander(ARTSDKCommanderStateBase* inCommanderState) override;
	virtual void SetCommanders(TArray<ARTSDKCommanderStateBase*> inCommanderStates) override;
	virtual int32 GetCommanderCount() override;
	virtual ARTSDKForceStateBase* GetForce(const int32& inForceID) override;
	virtual TArray<ARTSDKForceStateBase*> GetForces() override;
	virtual int32 AddForce(ARTSDKForceStateBase* inForceState) override;
	virtual void SetForces(TArray<ARTSDKForceStateBase*> inForceStates) override;
	virtual int32 GetForceCount() override;
	virtual ARTSDKTeamStateBase* GetTeam(const int32& inTeamID) override;
	virtual TArray<ARTSDKTeamStateBase*> GetTeams() override;
	virtual int32 AddTeam(ARTSDKTeamStateBase* inTeamState) override;
	virtual void SetTeams(TArray<ARTSDKTeamStateBase*> inTeamStates) override;
	virtual int32 GetTeamCount() override;
	virtual TArray<FRTSDKPlayerCommandReplicationInfo> GetCommandsForCommanderByTurn(const int32& inCommanderID, const int32& inTurn) override;
	//~RTSDKSimState interface

protected:

	UPROPERTY(Transient, Replicated)
		TArray<TObjectPtr<ARTSDKCommanderStateBase>> Commanders;

	UPROPERTY(Transient, Replicated)
		TArray<TObjectPtr<ARTSDKForceStateBase>> Forces;

	UPROPERTY(Transient, Replicated)
		TArray<TObjectPtr<ARTSDKTeamStateBase>> Teams;

	UPROPERTY(Transient, Replicated)
		bool bMatchHasStarted;

	//This class has the only replicated match is paused flag, set only by the server.
	UPROPERTY(Transient, Replicated)
		bool bMatchIsPaused;

	UPROPERTY(Transient, Replicated)
		TArray<TObjectPtr<ARTSDKCommanderStateBase>> Pausers;
};
