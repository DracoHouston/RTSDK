// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Info.h"
#include "RTSDKPlayerCommand.h"
#include "RTSDKCommanderState.generated.h"

class ARTSDKCommanderStateBase;
class URTSDKGameSimSubsystem;
class ARTSDKSimStateBase;
class ARTSDKForceStateBase;
class ARTSDKTeamStateBase;
class URTSDKUnitComponent;

USTRUCT(BlueprintType)
struct RTSDK_API FRTSDKCommanderSelectionUnit
{
	GENERATED_BODY()

public:

	UPROPERTY()
		uint32 UnitID;

	UPROPERTY(BlueprintReadOnly)
		TObjectPtr<URTSDKUnitComponent> Unit;

	FRTSDKCommanderSelectionUnit() {}

	FRTSDKCommanderSelectionUnit(uint32 inUnitID, URTSDKUnitComponent* inUnitComponent)	:
		UnitID(inUnitID),
		Unit(inUnitComponent)
	{
	}
};

USTRUCT()
struct RTSDK_API FRTSDKCommanderSelectionUnits
{
	GENERATED_BODY()

public:

	UPROPERTY()
		TArray<FRTSDKCommanderSelectionUnit> Units;
};

USTRUCT()
struct RTSDK_API FRTSDKCommanderSelectionUnitWithDefinition
{
	GENERATED_BODY()

public:

	UPROPERTY()
		uint32 UnitID;

	UPROPERTY()
		TObjectPtr<URTSDKUnitComponent> Unit;

	UPROPERTY()
		TSubclassOf<URTSDKUnitDefinition> UnitDefinition;

	FRTSDKCommanderSelectionUnitWithDefinition() {}

	FRTSDKCommanderSelectionUnitWithDefinition(uint32 inUnitID, URTSDKUnitComponent* inUnitComponent, UClass* inUnitDefinition) :
		UnitID(inUnitID),
		Unit(inUnitComponent),
		UnitDefinition()
	{
	}

	bool operator==(const FRTSDKCommanderSelectionUnitWithDefinition& Other)
	{
		return UnitID == Other.UnitID;
	}
};

RTSDK_API bool operator==(const FRTSDKCommanderSelectionUnit& LHS, const FRTSDKCommanderSelectionUnit& RHS);

USTRUCT(BlueprintType)
struct RTSDK_API FRTSDKCommanderSelectionInfo
{
	GENERATED_BODY()

public:

	UPROPERTY()
	TMap<TSubclassOf<URTSDKUnitDefinition>, FRTSDKCommanderSelectionUnits> UnitsByType;

	void SortSelection();
	void AddUnit(UClass* inDefinition, uint32 inID, URTSDKUnitComponent* inUnit);
	void AddUnits(UClass* inDefinition, TArray<FRTSDKCommanderSelectionUnit> inUnits);
	void RemoveUnit(UClass* inDefinition, FRTSDKCommanderSelectionUnit inUnits);
	void RemoveUnits(UClass* inDefinition, TArray<FRTSDKCommanderSelectionUnit> inUnits);
	void RemoveUnit(URTSDKUnitComponent* inUnit);
	void ClearSelection();
	TArray<FRTSDKCommanderSelectionUnit> GetAllUnits() const;
	TArray<FRTSDKCommanderSelectionUnitWithDefinition> GetAllUnitsWithDefinitions() const;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FRTSDKOnCommanderSelectionChanged, ARTSDKCommanderStateBase*, Sender, const FRTSDKCommanderSelectionInfo&, NewSelection);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FRTSDKOnCommanderSubSelectionChanged, ARTSDKCommanderStateBase*, Sender, const FRTSDKCommanderSelectionUnit&, NewSubSelection);

struct FRTSDKTurnDataArray;

USTRUCT()
struct RTSDK_API FRTSDKTurnData : public FFastArraySerializerItem
{
	GENERATED_BODY()

		UPROPERTY()
		int32 Turn;

	UPROPERTY()
		TArray<FRTSDKPlayerCommandReplicationInfo> Commands;

	void PreReplicatedRemove(const FRTSDKTurnDataArray& InArraySerializer);
	void PostReplicatedAdd(const FRTSDKTurnDataArray& InArraySerializer);
	void PostReplicatedChange(const FRTSDKTurnDataArray& InArraySerializer);

};

USTRUCT()
struct RTSDK_API FRTSDKTurnDataArray : public FFastArraySerializer
{
	GENERATED_BODY()

		void AddTurn(FRTSDKTurnData inTurnData);

	bool GetTurn(int32 inTurn, FRTSDKTurnData& outTurnData);

	bool HasTurn(int32 inTurn);

	UPROPERTY()
		TArray<FRTSDKTurnData> Turns;

	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms)
	{
		return FFastArraySerializer::FastArrayDeltaSerialize<FRTSDKTurnData, FRTSDKTurnDataArray>(Turns, DeltaParms, *this);
	}
};

/** Specified to allow fast TArray replication */
template<>
struct TStructOpsTypeTraits<FRTSDKTurnDataArray> : public TStructOpsTypeTraitsBase2<FRTSDKTurnDataArray>
{
	enum
	{
		WithNetDeltaSerializer = true,
	};
};

struct FRTSDKReplicatedSelectionArray;

USTRUCT()
struct RTSDK_API FRTSDKReplicatedSelectionItem : public FFastArraySerializerItem
{
	GENERATED_BODY()

	UPROPERTY()
		uint32 UnitID;

	void PreReplicatedRemove(const FRTSDKReplicatedSelectionArray& InArraySerializer);
	void PostReplicatedAdd(const FRTSDKReplicatedSelectionArray& InArraySerializer);
	void PostReplicatedChange(const FRTSDKReplicatedSelectionArray& InArraySerializer);

};

USTRUCT()
struct RTSDK_API FRTSDKReplicatedSelectionArray : public FFastArraySerializer
{
	GENERATED_BODY()

	void Add(FRTSDKReplicatedSelectionItem inSelectionItem);

	bool Get(int32 inIndex, FRTSDKReplicatedSelectionItem& outItem);

	UPROPERTY()
		TArray<FRTSDKReplicatedSelectionItem> Items;

	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms)
	{
		return FFastArraySerializer::FastArrayDeltaSerialize<FRTSDKReplicatedSelectionItem, FRTSDKReplicatedSelectionArray>(Items, DeltaParms, *this);
	}
};

/** Specified to allow fast TArray replication */
template<>
struct TStructOpsTypeTraits<FRTSDKReplicatedSelectionArray> : public TStructOpsTypeTraitsBase2<FRTSDKReplicatedSelectionArray>
{
	enum
	{
		WithNetDeltaSerializer = true,
	};
};

/**
 * Abstract base class for the state of a Commander.
 * Commanders are the will behind forces.
 * Can be a player, ai, or replay dummy.
 * Child classes of this implement different networking environments.
 */
UCLASS(abstract)
class RTSDK_API ARTSDKCommanderStateBase : public AInfo
{
	GENERATED_BODY()

public:

	ARTSDKCommanderStateBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION()
		virtual void Setup(const FRTSDKStateSetupInfo& inSetup, ARTSDKSimStateBase* inSimState, URTSDKGameSimSubsystem* inSimSubsystem);

	UFUNCTION()
		virtual FText GetDisplayName() { return FText(); }

	UFUNCTION()
		virtual void SetDisplayName(const FText& inName) {}

	UFUNCTION()
		virtual bool GetIsPlayer() { return false; }

	UFUNCTION()
		virtual void SetIsPlayer(bool inIsPlayer) {}

	UFUNCTION()
		virtual bool GetIsReady() { return false; }

	UFUNCTION()
		virtual void SetIsReady(bool inIsReady) {}

	UFUNCTION()
		virtual int32 GetPlayerID() { return -1; }

	UFUNCTION()
		virtual void SetPlayerID(int32 inPlayerID) {}

	//Get all commands for this commander for a turn as an array of player command infos
	UFUNCTION()
	virtual TArray<FRTSDKPlayerCommandReplicationInfo> GetCommandsByTurn(int32 inTurn) { return TArray<FRTSDKPlayerCommandReplicationInfo>();	}

	//Get all commands for this commander for a turn as an array of player command infos
	UFUNCTION()
		virtual bool HasTurn(int32 inTurn) { return false; }

	//Set the last completed turn of a commander. 
	//This is used in multiplayer supporting commander states to record when
	//participating players have completed a turn, or set of frames.
	//When one of the commanders falls sufficiently behind this will prevent the
	//server from advancing sim frame and input turn during a game thread tick.
	//Singleplayer only and the curves based server authoritative child classes ignore this.
	UFUNCTION()
		virtual void SetLastCompletedTurn(int32 inTurn);

	//Get the last completed turn of a commander, usually the last completed turn of
	//a connected player in a networked game on their machine. Bots, the server authorative
	//and singleplayer only versions always return the current turn.
	UFUNCTION()
		virtual int32 GetLastCompletedTurn();

	//Add a command to the command buffer, as a player command info
	UFUNCTION()
	virtual void AddCommandToCommandBuffer(FRTSDKPlayerCommandReplicationInfo inCommand) {}

	//Flush the current command buffer, adding it to the commands per turn for the current input turn.
	UFUNCTION()
	virtual void FlushCommandBuffer() {}

	//Get the team this commander belongs to.
	UFUNCTION()
	virtual ARTSDKTeamStateBase* GetTeam() { return nullptr; }

	//Get the force this commander belongs to.
	UFUNCTION()
		virtual ARTSDKForceStateBase* GetForce() { return nullptr; }

	UFUNCTION()
		virtual void SetForce(ARTSDKForceStateBase* inForce) { }

	UFUNCTION()
		virtual void SetCurrentSelection(FRTSDKCommanderSelectionInfo inSelectionInfo);

	/**
	* Add Units To Selection
	* Takes an array of selection unit structs and a UClass pointer to the unit definition
	* adds multiple units of one type.
	*/
	UFUNCTION()
		virtual void AddUnitsToSelection(const TArray<FRTSDKCommanderSelectionUnit>& inUnits, UClass* inUnitDefClass);

	UFUNCTION()
		virtual void AddUnitToSelection(FRTSDKCommanderSelectionUnitWithDefinition inUnit);

	/**
	* Remove Units From Selection
	* Takes an array of selection unit structs and a UClass pointer to the unit definition
	* removes multiple units of one type, if they exist in selection.
	*/
	UFUNCTION()
		virtual void RemoveUnitsFromSelection(const TArray<FRTSDKCommanderSelectionUnit>& inUnits, UClass* inUnitDefClass);

	UFUNCTION()
		virtual void RemoveUnitFromSelection(FRTSDKCommanderSelectionUnitWithDefinition inUnit);

	UFUNCTION()
		virtual FRTSDKCommanderSelectionInfo GetCurrentSelection() { return CommanderSelection; }

	UFUNCTION()
		virtual TArray<FRTSDKCommanderSelectionUnit> GetAllCurrentSelectionUnits() { return CommanderSelection.GetAllUnits(); }

	UPROPERTY(BlueprintAssignable)
		FRTSDKOnCommanderSelectionChanged OnSelectionChanged;

	UPROPERTY(BlueprintAssignable)
		FRTSDKOnCommanderSubSelectionChanged OnSubSelectionChanged;

protected:

	//Set on spawn, the sim subsystem this commander belongs to.
	UPROPERTY(Transient)
	TObjectPtr<URTSDKGameSimSubsystem> SimSubsystem;

	//Set on spawn, the sim state this commander belongs to.
	UPROPERTY(Transient)
	TObjectPtr<ARTSDKSimStateBase> SimState;

	//Input Command Buffer, the commands so far for this input turn. 
	//FlushCommandBuffer to commit to turn data.
	UPROPERTY(Transient)
	TArray<FRTSDKPlayerCommandReplicationInfo> InputCommandBuffer;

	//Unreplicated counter for the last completed turn of a commander state.
	//Used by some child classes to indicate how a remote player is progressing
	//through sim frames and input turns. Generally set via some sort of RPC.
	//Never valid on singleplayer only and server client curves versions
	//or on clients in the server client lockstep version, where it is used by the server.
	//valid on all machines for peer 2 peer lockstep version
	UPROPERTY(Transient)
		int32 LastCompletedTurn;

	UPROPERTY(Transient)
		FRTSDKCommanderSelectionInfo CommanderSelection;

	UPROPERTY(Transient)
		FRTSDKCommanderSelectionUnitWithDefinition CommanderSubSelection;
};

/**
 * RTSDK Commander State SP Only, the singleplayer only version of RTSDK Commander State Base.
 * No networking.
 */
UCLASS()
class RTSDK_API ARTSDKCommanderStateSPOnly : public ARTSDKCommanderStateBase
{
	GENERATED_BODY()

public:

	ARTSDKCommanderStateSPOnly(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual FText GetDisplayName() override;
	virtual void SetDisplayName(const FText& inName) override;
	virtual bool GetIsPlayer() override;
	virtual void SetIsPlayer(bool inIsPlayer) override;
	virtual bool GetIsReady() override;
	virtual void SetIsReady(bool inIsReady) override;
	virtual int32 GetPlayerID() override;
	virtual void SetPlayerID(int32 inPlayerID) override;
	virtual bool HasTurn(int32 inTurn) override;
	virtual void SetLastCompletedTurn(int32 inTurn) override {} //we don't use this in sp only
	virtual int32 GetLastCompletedTurn() override;
	virtual TArray<FRTSDKPlayerCommandReplicationInfo> GetCommandsByTurn(int32 inTurn) override;
	virtual void AddCommandToCommandBuffer(FRTSDKPlayerCommandReplicationInfo inCommand) override;
	virtual void FlushCommandBuffer() override;
	virtual ARTSDKTeamStateBase* GetTeam() override;
	virtual ARTSDKForceStateBase* GetForce() override;
	virtual void SetForce(ARTSDKForceStateBase* inForce) override;

protected:
	
	UPROPERTY(Transient)
		TObjectPtr<ARTSDKForceStateBase> Force;

	UPROPERTY(Transient)
		FRTSDKTurnDataArray TurnData;

	UPROPERTY(Transient)
		FText DisplayName;

	UPROPERTY(Transient)
		bool bIsPlayer;

	UPROPERTY(Transient)
		int32 PlayerID;

	UPROPERTY(Transient)
		int32 bIsReady;
};

/**
 * RTSDK Commander State SP, the singleplayer only version of RTSDK Commander State Base.
 * Networked via actor replication. All players get turn data.
 */
UCLASS()
class RTSDK_API ARTSDKCommanderStateServerClientLockstep : public ARTSDKCommanderStateBase
{
	GENERATED_BODY()

public:

	ARTSDKCommanderStateServerClientLockstep(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual FText GetDisplayName() override;
	virtual void SetDisplayName(const FText& inName) override;
	virtual bool GetIsPlayer() override;
	virtual void SetIsPlayer(bool inIsPlayer) override;
	virtual bool GetIsReady() override;
	virtual void SetIsReady(bool inIsReady) override;
	virtual int32 GetPlayerID() override;
	virtual void SetPlayerID(int32 inPlayerID) override;
	virtual bool HasTurn(int32 inTurn) override;
	virtual TArray<FRTSDKPlayerCommandReplicationInfo> GetCommandsByTurn(int32 inTurn) override;
	virtual void AddCommandToCommandBuffer(FRTSDKPlayerCommandReplicationInfo inCommand) override;
	virtual void FlushCommandBuffer() override;
	virtual ARTSDKTeamStateBase* GetTeam() override;
	virtual ARTSDKForceStateBase* GetForce() override;
	virtual void SetForce(ARTSDKForceStateBase* inForce) override;

protected:

	UPROPERTY(Transient, Replicated)
	TObjectPtr<ARTSDKForceStateBase> Force;

	UPROPERTY(Transient, Replicated)
	FRTSDKTurnDataArray TurnData;

	UPROPERTY(Transient, Replicated)
		FText DisplayName;

	UPROPERTY(Transient, Replicated)
		bool bIsPlayer;

	UPROPERTY(Transient, Replicated)
		int32 PlayerID;

	UPROPERTY(Transient, Replicated)
		int32 bIsReady;
};

/**
 * RTSDK Commander State Server Client Curves, a multiplayer version of RTSDK Commander State Base.
 * Networked via actor replication. Only server sees turn data.
 */
UCLASS()
class RTSDK_API ARTSDKCommanderStateServerClientCurves : public ARTSDKCommanderStateBase
{
	GENERATED_BODY()

public:

	ARTSDKCommanderStateServerClientCurves(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual FText GetDisplayName() override;
	virtual void SetDisplayName(const FText& inName) override;
	virtual bool GetIsPlayer() override;
	virtual void SetIsPlayer(bool inIsPlayer) override;
	virtual bool GetIsReady() override;
	virtual void SetIsReady(bool inIsReady) override;
	virtual int32 GetPlayerID() override;
	virtual void SetPlayerID(int32 inPlayerID) override;
	virtual bool HasTurn(int32 inTurn) override;
	virtual void SetLastCompletedTurn(int32 inTurn) override {} //we don't use this, server's sim frames and input turns waits for no one
	virtual int32 GetLastCompletedTurn() override;
	virtual TArray<FRTSDKPlayerCommandReplicationInfo> GetCommandsByTurn(int32 inTurn) override;
	virtual void AddCommandToCommandBuffer(FRTSDKPlayerCommandReplicationInfo inCommand) override;
	virtual void FlushCommandBuffer() override;
	virtual ARTSDKTeamStateBase* GetTeam() override;
	virtual ARTSDKForceStateBase* GetForce() override;
	virtual void SetForce(ARTSDKForceStateBase* inForce) override;

protected:

	UPROPERTY(Transient, Replicated)
		TObjectPtr<ARTSDKForceStateBase> Force;

	UPROPERTY(Transient)
		FRTSDKTurnDataArray TurnData;

	UPROPERTY(Transient, Replicated)
		FText DisplayName;

	UPROPERTY(Transient, Replicated)
		bool bIsPlayer;

	UPROPERTY(Transient, Replicated)
		int32 PlayerID;

	UPROPERTY(Transient, Replicated)
		int32 bIsReady;
};