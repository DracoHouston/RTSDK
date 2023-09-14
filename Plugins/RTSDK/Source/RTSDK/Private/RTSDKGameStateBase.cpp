// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSDKGameStateBase.h"
#include "Net/UnrealNetwork.h"

void ARTSDKGameStateBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ARTSDKGameStateBase, bRTSDKMatchStarted);
}