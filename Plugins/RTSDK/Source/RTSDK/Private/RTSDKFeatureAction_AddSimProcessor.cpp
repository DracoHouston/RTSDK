// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSDKFeatureAction_AddSimProcessor.h"
#include "RTSDKModManager.h"
#include "RTSDKGameFeatureData.h"
#include "RTSDKModDefinitionBase.h"

void URTSDKFeatureAction_AddSimProcessor::OnGameFeatureActivating(UWorld* WorldContext)
{
	if (WorldContext == nullptr)
	{
		return;
	}
	URTSDKModManager* modmanager = GEngine->GetEngineSubsystem<URTSDKModManager>();
	URTSDKModDefinitionBase* mod = Cast<URTSDKModDefinitionBase>(GetOuter());
	if (mod == nullptr)
	{
		return;
	}

	for (int32 i = 0; i < ProcessorClasses.Num(); i++)
	{
		UClass* procclass = ProcessorClasses[i].Get();

		if (procclass == nullptr)
		{
			continue;
		}

		UMassProcessor* proccdo = procclass->GetDefaultObject<UMassProcessor>();

		if (proccdo == nullptr)
		{
			continue;
		}
		modmanager->AddMassProcessorToActiveSimPhase(WorldContext, mod, proccdo);
	}		
}

void URTSDKFeatureAction_AddSimProcessor::OnGameFeatureDeactivating(UWorld* WorldContext)
{
}
