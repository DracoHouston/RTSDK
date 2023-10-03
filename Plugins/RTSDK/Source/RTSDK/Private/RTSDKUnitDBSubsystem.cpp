// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSDKUnitDBSubsystem.h"
#include "RTSDKUnitDefinition.h"

void URTSDKUnitDBSubsystem::AddUnit(TSubclassOf<URTSDKUnitDefinition> inDefinition)
{
}

//void URTSDKUnitDBSubsystem::AddUnitModifier(URTSDKModDefinitionBase* Caller, TSubclassOf<URTSDKUnitDefinition> inDefinition, URTSDKUnitDefinitionModifier* inModifier)
//{
//}
//
//void URTSDKUnitDBSubsystem::RemoveUnitModifier(URTSDKModDefinitionBase* Caller, TSubclassOf<URTSDKUnitDefinition> inDefinition, URTSDKUnitDefinitionModifier* inModifier)
//{
//}

void URTSDKUnitDBSubsystem::RemoveUnit(TSubclassOf<URTSDKUnitDefinition> inDefinition)
{
}

URTSDKUnitDefinition* URTSDKUnitDBSubsystem::GetFinalUnitDefinition(TSubclassOf<URTSDKUnitDefinition> inDefinition)
{
	return nullptr;
}
