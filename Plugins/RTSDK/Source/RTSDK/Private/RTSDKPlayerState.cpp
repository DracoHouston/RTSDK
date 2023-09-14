// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSDKPlayerState.h"
#include "Net/UnrealNetwork.h"



//void ARTSDKPlayerState::AddCurrentFrameToFrameData(int32 inTurnNumber)
//{
//	if (GetLocalRole() != ROLE_Authority)
//	{
//		return;
//	}
//	FRTSDKTurnData turn;
//	turn.Turn = inTurnNumber;
//	turn.Commands = InputCommandBuffer;
//	TurnData.AddTurn(turn);
//	InputCommandBuffer.Empty();
//}

void ARTSDKPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ARTSDKPlayerState, PlayerIndex);
	DOREPLIFETIME(ARTSDKPlayerState, bIsReady);
}
