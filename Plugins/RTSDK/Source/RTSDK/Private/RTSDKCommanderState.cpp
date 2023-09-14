// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSDKCommanderState.h"
#include "RTSDKTeamState.h"
#include "RTSDKForceState.h"
#include "RTSDKSimState.h"
#include "RTSDKUnitComponent.h"
#include "RTSDKGameSimSubsystem.h"
#include "RTSDKLaunchOptionsHelpers.h"
#include "Net/UnrealNetwork.h"

void FRTSDKTurnData::PreReplicatedRemove(const FRTSDKTurnDataArray& InArraySerializer)
{
}

void FRTSDKTurnData::PostReplicatedAdd(const FRTSDKTurnDataArray& InArraySerializer)
{
}

void FRTSDKTurnData::PostReplicatedChange(const FRTSDKTurnDataArray& InArraySerializer)
{
}

void FRTSDKTurnDataArray::AddTurn(FRTSDKTurnData inTurnData)
{
	int32 Idx = Turns.Add(inTurnData);
	MarkItemDirty(Turns[Idx]);

	// server calls "on rep" also
	Turns[Idx].PostReplicatedAdd(*this);
}

bool FRTSDKTurnDataArray::GetTurn(int32 inTurn, FRTSDKTurnData& outTurnData)
{
	for (int32 i = 0; i < Turns.Num(); i++)
	{
		if (Turns[i].Turn == inTurn)
		{
			outTurnData = Turns[i];
			return true;
		}
	}
	return false;
}

bool FRTSDKTurnDataArray::HasTurn(int32 inTurn)
{
	for (int32 i = 0; i < Turns.Num(); i++)
	{
		if (Turns[i].Turn == inTurn)
		{
			return true;
		}
	}
	return false;
}

void FRTSDKReplicatedSelectionItem::PreReplicatedRemove(const FRTSDKReplicatedSelectionArray& InArraySerializer)
{
}

void FRTSDKReplicatedSelectionItem::PostReplicatedAdd(const FRTSDKReplicatedSelectionArray& InArraySerializer)
{
}

void FRTSDKReplicatedSelectionItem::PostReplicatedChange(const FRTSDKReplicatedSelectionArray& InArraySerializer)
{
}

void FRTSDKReplicatedSelectionArray::Add(FRTSDKReplicatedSelectionItem inSelectionItem)
{
	int32 Idx = Items.Add(inSelectionItem);
	MarkItemDirty(Items[Idx]);

	// server calls "on rep" also
	Items[Idx].PostReplicatedAdd(*this);
}

bool FRTSDKReplicatedSelectionArray::Get(int32 inIndex, FRTSDKReplicatedSelectionItem& outItem)
{
	if (inIndex < Items.Num())
	{
		outItem = Items[inIndex];
		return true;
	}
	return false;
}

ARTSDKCommanderStateBase::ARTSDKCommanderStateBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;
	LastCompletedTurn = -1;
}

void ARTSDKCommanderStateBase::Setup(const FRTSDKStateSetupInfo& inSetup, ARTSDKSimStateBase* inSimState, URTSDKGameSimSubsystem* inSimSubsystem)
{
	//todo: try set force by string helper
	SimSubsystem = inSimSubsystem;
	SimState = inSimState;
	for (auto It = inSetup.OptionsMap.CreateConstIterator(); It; ++It)
	{
		if (It->Key == TEXT("Name"))
		{
			SetDisplayName(FText::FromString(It->Value));
		}
		else if (It->Key == TEXT("Force"))
		{
			int32 result = FCString::Atoi(*It->Value);
			ARTSDKForceStateBase* force = SimState->GetForce(result);
			if (force != nullptr)
			{
				SetForce(force);
			}
		}
		else if (It->Key == TEXT("IsPlayer"))
		{
			bool result = FCString::ToBool(*It->Value);
			SetIsPlayer(result);
		}
		else if (It->Key == TEXT("PlayerID"))
		{
			int32 result = FCString::Atoi(*It->Value);
			SetPlayerID(result);
		}
	}
}

void ARTSDKCommanderStateBase::SetLastCompletedTurn(int32 inTurn)
{
	LastCompletedTurn = inTurn;
}

int32 ARTSDKCommanderStateBase::GetLastCompletedTurn()
{
	return LastCompletedTurn;
}

void ARTSDKCommanderStateBase::SetCurrentSelection(FRTSDKCommanderSelectionInfo inSelectionInfo)
{
	CommanderSelection = inSelectionInfo;
}

void ARTSDKCommanderStateBase::AddUnitsToSelection(const TArray<FRTSDKCommanderSelectionUnit>& inUnits, UClass* inUnitDefClass)
{
	CommanderSelection.AddUnits(inUnitDefClass, inUnits);
}

void ARTSDKCommanderStateBase::AddUnitToSelection(FRTSDKCommanderSelectionUnitWithDefinition inUnit)
{
	CommanderSelection.AddUnit(inUnit.UnitDefinition, inUnit.UnitID, inUnit.Unit);
}

void ARTSDKCommanderStateBase::RemoveUnitsFromSelection(const TArray<FRTSDKCommanderSelectionUnit>& inUnits, UClass* inUnitDefClass)
{
	CommanderSelection.RemoveUnits(inUnitDefClass, inUnits);
}

void ARTSDKCommanderStateBase::RemoveUnitFromSelection(FRTSDKCommanderSelectionUnitWithDefinition inUnit)
{
	CommanderSelection.RemoveUnit(inUnit.UnitDefinition, FRTSDKCommanderSelectionUnit(inUnit.UnitID, inUnit.Unit));
}

ARTSDKCommanderStateSPOnly::ARTSDKCommanderStateSPOnly(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bReplicates = false;
	bIsReady = false;
	bIsPlayer = false;
	PlayerID = -1;

}

FText ARTSDKCommanderStateSPOnly::GetDisplayName()
{
	return DisplayName;
}

void ARTSDKCommanderStateSPOnly::SetDisplayName(const FText& inName)
{
	DisplayName = inName;
}

bool ARTSDKCommanderStateSPOnly::GetIsPlayer()
{
	return bIsPlayer;
}

void ARTSDKCommanderStateSPOnly::SetIsPlayer(bool inIsPlayer)
{
	bIsPlayer = inIsPlayer;
}

bool ARTSDKCommanderStateSPOnly::GetIsReady()
{
	//bots are always ready
	return (bIsPlayer && bIsReady) || (bIsPlayer == false);
}

void ARTSDKCommanderStateSPOnly::SetIsReady(bool inIsReady)
{
	bIsReady = inIsReady;
}

int32 ARTSDKCommanderStateSPOnly::GetPlayerID()
{
	//unlike networked versions of commander state, this returns the value even on bots
	//allowing it to point to some sort of character or personality setting for a bot
	//same is true for setter.
	return PlayerID;
}

void ARTSDKCommanderStateSPOnly::SetPlayerID(int32 inPlayerID)
{
	PlayerID = inPlayerID;
}

bool ARTSDKCommanderStateSPOnly::HasTurn(int32 inTurn)
{
	return true;
}

int32 ARTSDKCommanderStateSPOnly::GetLastCompletedTurn()
{
	return SimSubsystem->GetCurrentInputTurn();
}

TArray<FRTSDKPlayerCommandReplicationInfo> ARTSDKCommanderStateSPOnly::GetCommandsByTurn(int32 inTurn)
{
	FRTSDKTurnData turn;
	if (TurnData.GetTurn(inTurn, turn))
	{
		return turn.Commands;
	}
	return TArray<FRTSDKPlayerCommandReplicationInfo>();
}

void ARTSDKCommanderStateSPOnly::AddCommandToCommandBuffer(FRTSDKPlayerCommandReplicationInfo inCommand)
{
	InputCommandBuffer.Add(inCommand);
}

void ARTSDKCommanderStateSPOnly::FlushCommandBuffer()
{
	FRTSDKTurnData turn;
	turn.Commands = InputCommandBuffer;
	turn.Turn = SimSubsystem->GetCurrentInputTurn();
	TurnData.AddTurn(turn);
}

ARTSDKTeamStateBase* ARTSDKCommanderStateSPOnly::GetTeam()
{
	return Force->GetTeam();
}

ARTSDKForceStateBase* ARTSDKCommanderStateSPOnly::GetForce()
{
	return Force;
}

void ARTSDKCommanderStateSPOnly::SetForce(ARTSDKForceStateBase* inForce)
{
	Force = inForce;
}

ARTSDKCommanderStateServerClientLockstep::ARTSDKCommanderStateServerClientLockstep(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bReplicates = true;
	bAlwaysRelevant = true;
}

FText ARTSDKCommanderStateServerClientLockstep::GetDisplayName()
{
	return DisplayName;
}

void ARTSDKCommanderStateServerClientLockstep::SetDisplayName(const FText& inName)
{
	DisplayName = inName;
}

bool ARTSDKCommanderStateServerClientLockstep::GetIsPlayer()
{
	return bIsPlayer;
}

void ARTSDKCommanderStateServerClientLockstep::SetIsPlayer(bool inIsPlayer)
{
	bIsPlayer = inIsPlayer;
}

bool ARTSDKCommanderStateServerClientLockstep::GetIsReady()
{
	//bots are always ready
	return (bIsPlayer && bIsReady) || (bIsPlayer == false);
}

void ARTSDKCommanderStateServerClientLockstep::SetIsReady(bool inIsReady)
{
	if (bIsPlayer == false)
	{
		return;//don't set this on bots, waste of bandwidth.
	}
	bIsReady = inIsReady;
}

int32 ARTSDKCommanderStateServerClientLockstep::GetPlayerID()
{
	if (bIsPlayer == false)
	{
		return -1;//bots arent players
	}
	return PlayerID;
}

void ARTSDKCommanderStateServerClientLockstep::SetPlayerID(int32 inPlayerID)
{
	if (bIsPlayer == false)
	{
		return;//don't set this on bots, waste of bandwidth.
	}
	PlayerID = inPlayerID;
}

bool ARTSDKCommanderStateServerClientLockstep::HasTurn(int32 inTurn)
{
	return TurnData.HasTurn(inTurn);
}

TArray<FRTSDKPlayerCommandReplicationInfo> ARTSDKCommanderStateServerClientLockstep::GetCommandsByTurn(int32 inTurn)
{
	FRTSDKTurnData turn;
	if (TurnData.GetTurn(inTurn, turn))
	{
		return turn.Commands;
	}
	return TArray<FRTSDKPlayerCommandReplicationInfo>();
}

void ARTSDKCommanderStateServerClientLockstep::AddCommandToCommandBuffer(FRTSDKPlayerCommandReplicationInfo inCommand)
{
	InputCommandBuffer.Add(inCommand);
}

void ARTSDKCommanderStateServerClientLockstep::FlushCommandBuffer()
{
	FRTSDKTurnData turn;
	turn.Commands = InputCommandBuffer;
	turn.Turn = SimSubsystem->GetCurrentInputTurn();
	TurnData.AddTurn(turn);
	InputCommandBuffer.Empty(8);
}

ARTSDKTeamStateBase* ARTSDKCommanderStateServerClientLockstep::GetTeam()
{
	return Force->GetTeam();
}

ARTSDKForceStateBase* ARTSDKCommanderStateServerClientLockstep::GetForce()
{
	return Force;
}

void ARTSDKCommanderStateServerClientLockstep::SetForce(ARTSDKForceStateBase* inForce)
{
	Force = inForce;
}

void ARTSDKCommanderStateServerClientLockstep::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ARTSDKCommanderStateServerClientLockstep, TurnData);
	DOREPLIFETIME(ARTSDKCommanderStateServerClientLockstep, Force);
	DOREPLIFETIME(ARTSDKCommanderStateServerClientLockstep, DisplayName);
	DOREPLIFETIME(ARTSDKCommanderStateServerClientLockstep, bIsPlayer);
	DOREPLIFETIME(ARTSDKCommanderStateServerClientLockstep, bIsReady);
	DOREPLIFETIME(ARTSDKCommanderStateServerClientLockstep, PlayerID);
}

ARTSDKCommanderStateServerClientCurves::ARTSDKCommanderStateServerClientCurves(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bReplicates = true;
	bAlwaysRelevant = true;
}

FText ARTSDKCommanderStateServerClientCurves::GetDisplayName()
{
	return DisplayName;
}

void ARTSDKCommanderStateServerClientCurves::SetDisplayName(const FText& inName)
{
	DisplayName = inName;
}

bool ARTSDKCommanderStateServerClientCurves::GetIsPlayer()
{
	return bIsPlayer;
}

void ARTSDKCommanderStateServerClientCurves::SetIsPlayer(bool inIsPlayer)
{
	bIsPlayer = inIsPlayer;
}

bool ARTSDKCommanderStateServerClientCurves::GetIsReady()
{
	//bots are always ready
	return (bIsPlayer && bIsReady) || (bIsPlayer == false);
}

void ARTSDKCommanderStateServerClientCurves::SetIsReady(bool inIsReady)
{
	if (bIsPlayer == false)
	{
		return;//don't set this on bots, waste of bandwidth.
	}
	bIsReady = inIsReady;
}

int32 ARTSDKCommanderStateServerClientCurves::GetPlayerID()
{
	if (bIsPlayer == false)
	{
		return -1;//bots arent players
	}
	return PlayerID;
}

void ARTSDKCommanderStateServerClientCurves::SetPlayerID(int32 inPlayerID)
{
	if (bIsPlayer == false)
	{
		return;//don't set this on bots, waste of bandwidth.
	}
	PlayerID = inPlayerID;
}

bool ARTSDKCommanderStateServerClientCurves::HasTurn(int32 inTurn)
{
	return TurnData.HasTurn(inTurn);
}

int32 ARTSDKCommanderStateServerClientCurves::GetLastCompletedTurn()
{
	return SimSubsystem->GetCurrentInputTurn();
}

TArray<FRTSDKPlayerCommandReplicationInfo> ARTSDKCommanderStateServerClientCurves::GetCommandsByTurn(int32 inTurn)
{
	FRTSDKTurnData turn;
	if (TurnData.GetTurn(inTurn, turn))
	{
		return turn.Commands;
	}
	return TArray<FRTSDKPlayerCommandReplicationInfo>();
}

void ARTSDKCommanderStateServerClientCurves::AddCommandToCommandBuffer(FRTSDKPlayerCommandReplicationInfo inCommand)
{
	InputCommandBuffer.Add(inCommand);
}

void ARTSDKCommanderStateServerClientCurves::FlushCommandBuffer()
{
	FRTSDKTurnData turn;
	turn.Commands = InputCommandBuffer;
	turn.Turn = SimSubsystem->GetCurrentInputTurn();
	TurnData.AddTurn(turn);
}

ARTSDKTeamStateBase* ARTSDKCommanderStateServerClientCurves::GetTeam()
{
	return Force->GetTeam();
}

ARTSDKForceStateBase* ARTSDKCommanderStateServerClientCurves::GetForce()
{
	return Force;
}

void ARTSDKCommanderStateServerClientCurves::SetForce(ARTSDKForceStateBase* inForce)
{
	Force = inForce;
}

void ARTSDKCommanderStateServerClientCurves::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ARTSDKCommanderStateServerClientCurves, Force);
	DOREPLIFETIME(ARTSDKCommanderStateServerClientCurves, DisplayName);
	DOREPLIFETIME(ARTSDKCommanderStateServerClientCurves, bIsPlayer);
	DOREPLIFETIME(ARTSDKCommanderStateServerClientCurves, bIsReady);
	DOREPLIFETIME(ARTSDKCommanderStateServerClientCurves, PlayerID);
}

void FRTSDKCommanderSelectionInfo::SortSelection()
{
	UnitsByType.KeySort(
		[](const TSubclassOf<URTSDKUnitDefinition>& A, const TSubclassOf<URTSDKUnitDefinition>& B)
		{
			return A.Get()->GetClassPathName().ToString() < B.Get()->GetClassPathName().ToString();
		}
	);
	for (auto It = UnitsByType.CreateIterator(); It; ++It)
	{
		It->Value.Units.Sort(
			[](const FRTSDKCommanderSelectionUnit& A, const FRTSDKCommanderSelectionUnit& B)
			{
				return A.UnitID < B.UnitID;
			}
		);
	}
}

void FRTSDKCommanderSelectionInfo::AddUnit(UClass* inDefinition, uint32 inID, URTSDKUnitComponent* inUnit)
{
	TArray<FRTSDKCommanderSelectionUnit> units;
	units.Add(FRTSDKCommanderSelectionUnit(inID, inUnit));
	AddUnits(inDefinition, units);
}

void FRTSDKCommanderSelectionInfo::AddUnits(UClass* inDefinition, TArray<FRTSDKCommanderSelectionUnit> inUnits)
{
	if (inUnits.Num() == 0)
	{
		return;
	}
	FRTSDKCommanderSelectionUnits* result = UnitsByType.Find(inDefinition);
	if (result != nullptr)
	{
		for (int32 i = 0; i < inUnits.Num(); i++)
		{
			result->Units.AddUnique(inUnits[i]);
		}
		result->Units.Sort(
			[](const FRTSDKCommanderSelectionUnit& A, const FRTSDKCommanderSelectionUnit& B)
			{
				return A.UnitID < B.UnitID;
			}
		);
	}
	else
	{
		FRTSDKCommanderSelectionUnits& newtype = UnitsByType.Add(inDefinition);
		for (int32 i = 0; i < inUnits.Num(); i++)
		{
			newtype.Units.Add(inUnits[i]);
		}
		newtype.Units.Sort(
			[](const FRTSDKCommanderSelectionUnit& A, const FRTSDKCommanderSelectionUnit& B)
			{
				return A.UnitID < B.UnitID;
			}
		);
		UnitsByType.KeySort(
			[](const TSubclassOf<URTSDKUnitDefinition>& A, const TSubclassOf<URTSDKUnitDefinition>& B)
			{
				return A.Get()->GetClassPathName().ToString() < B.Get()->GetClassPathName().ToString();
			}
		);
	}
}

void FRTSDKCommanderSelectionInfo::RemoveUnit(UClass* inDefinition, FRTSDKCommanderSelectionUnit inUnit)
{
	TArray<FRTSDKCommanderSelectionUnit> units;
	units.Add(inUnit);
	RemoveUnits(inDefinition, units);
}

void FRTSDKCommanderSelectionInfo::RemoveUnits(UClass* inDefinition, TArray<FRTSDKCommanderSelectionUnit> inUnits)
{
	FRTSDKCommanderSelectionUnits* result = UnitsByType.Find(inDefinition);
	if (result != nullptr)
	{
		FRTSDKCommanderSelectionUnits& unitsref = *result;
		for (int32 i = 0; i < unitsref.Units.Num(); i++)
		{
			if (inUnits.Contains(unitsref.Units[i]))
			{
				unitsref.Units.RemoveAt(i);
				i--;
			}
		}
		if (unitsref.Units.Num() == 0)
		{
			UnitsByType.Remove(inDefinition);
			UnitsByType.KeySort(
				[](const TSubclassOf<URTSDKUnitDefinition>& A, const TSubclassOf<URTSDKUnitDefinition>& B)
				{
					return A.Get()->GetClassPathName().ToString() < B.Get()->GetClassPathName().ToString();
				}
			);
		}
	}
}

void FRTSDKCommanderSelectionInfo::RemoveUnit(URTSDKUnitComponent* inUnit)
{
	if (inUnit == nullptr)
	{
		return;
	}
	TArray<FRTSDKCommanderSelectionUnit> units;
	units.Add(FRTSDKCommanderSelectionUnit(inUnit->GetUnitID(), inUnit));
	RemoveUnits(inUnit->UnitDefintion, units);
}

void FRTSDKCommanderSelectionInfo::ClearSelection()
{
	UnitsByType.Empty();
}

TArray<FRTSDKCommanderSelectionUnit> FRTSDKCommanderSelectionInfo::GetAllUnits() const
{
	TArray<FRTSDKCommanderSelectionUnit> retval;
	for (auto It = UnitsByType.CreateConstIterator(); It; ++It)
	{
		retval += It->Value.Units;
	}
	return retval;
}

TArray<FRTSDKCommanderSelectionUnitWithDefinition> FRTSDKCommanderSelectionInfo::GetAllUnitsWithDefinitions() const
{
	TArray<FRTSDKCommanderSelectionUnitWithDefinition> retval;
	for (auto It = UnitsByType.CreateConstIterator(); It; ++It)
	{
		for (auto InnerIt = It->Value.Units.CreateConstIterator(); InnerIt; ++InnerIt)
		{
			retval.Add(FRTSDKCommanderSelectionUnitWithDefinition(InnerIt->UnitID, InnerIt->Unit, It->Key));
		}
	}
	return retval; 
}

RTSDK_API bool operator==(const FRTSDKCommanderSelectionUnit& LHS, const FRTSDKCommanderSelectionUnit& RHS)
{
	return LHS.UnitID == RHS.UnitID;
}
