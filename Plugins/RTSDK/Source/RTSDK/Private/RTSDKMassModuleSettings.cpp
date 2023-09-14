// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSDKMassModuleSettings.h"
#include "RTSDKGameSimProcessorInterface.h"
#include "RTSDKSimCompositeProcessor.h"
#include "RTSDKConstants.h"

URTSDKMassModuleSettings::URTSDKMassModuleSettings(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SimProcessingPhaseConfig.PhaseName = RTSDK::SimProcessingPhaseName;
	SimProcessingPhaseConfig.PhaseGroupClass = URTSDKSimCompositeProcessor::StaticClass();

	FCoreDelegates::OnPostEngineInit.AddUObject(this, &URTSDKMassModuleSettings::BuildProcessorListAndPhases);
}

void URTSDKMassModuleSettings::BeginDestroy()
{
	FCoreDelegates::OnPostEngineInit.RemoveAll(this);
	Super::BeginDestroy();
}

void URTSDKMassModuleSettings::BuildProcessorListAndPhases()
{
	if (bInitialized)
	{
		return;
	}

	BuildProcessorList();
	BuildPhases();
	bInitialized = true;
}

void URTSDKMassModuleSettings::BuildPhases()
{
#if WITH_EDITOR
	SimProcessingPhaseConfig.PhaseProcessor = NewObject<URTSDKSimCompositeProcessor>(this, SimProcessingPhaseConfig.PhaseGroupClass
		, *FString::Printf(TEXT("ProcessingPhase_%s"), *SimProcessingPhaseConfig.PhaseName.ToString()));
	SimProcessingPhaseConfig.PhaseProcessor->SetGroupName(SimProcessingPhaseConfig.PhaseName);
	SimProcessingPhaseConfig.PhaseProcessor->SetProcessingPhase(EMassProcessingPhase::PrePhysics);
	const FString PhaseDumpDependencyGraphFileName = !DumpDependencyGraphFileName.IsEmpty() ? DumpDependencyGraphFileName + TEXT("_") + SimProcessingPhaseConfig.PhaseName.ToString() : FString();
	SimProcessingPhaseConfig.PhaseProcessor->CopyAndSort(SimProcessingPhaseConfig, PhaseDumpDependencyGraphFileName);

	FStringOutputDevice Ar;
	SimProcessingPhaseConfig.PhaseProcessor->DebugOutputDescription(Ar);
	SimProcessingPhaseConfig.Description = FText::FromString(Ar);
#endif // WITH_EDITOR
}

void URTSDKMassModuleSettings::BuildProcessorList()
{
	ProcessorCDOs.Reset();
	SimProcessingPhaseConfig.ProcessorCDOs.Reset();

	TArray<UClass*> SubClassess;
	GetDerivedClasses(UMassProcessor::StaticClass(), SubClassess);

	for (int i = SubClassess.Num() - 1; i >= 0; --i)
	{
		if (SubClassess[i]->HasAnyClassFlags(CLASS_Abstract))
		{
			continue;
		}

		UMassProcessor* ProcessorCDO = GetMutableDefault<UMassProcessor>(SubClassess[i]);
		if (!SubClassess[i]->ImplementsInterface(URTSDKGameSimProcessorInterface::StaticClass()))
		{
			continue;
		}
		IRTSDKGameSimProcessorInterface* siminterface = Cast<IRTSDKGameSimProcessorInterface>(ProcessorCDO);
		if (siminterface == nullptr)
		{
			continue;
		}
		// we explicitly restrict adding UMassCompositeProcessor. If needed by specific project a derived class can be added
		if (ProcessorCDO && SubClassess[i] != UMassCompositeProcessor::StaticClass()
#if WITH_EDITOR
			&& siminterface->ShouldShowUpInSimSettings()
#endif // WITH_EDITOR
			)
		{
			ProcessorCDOs.Add(ProcessorCDO);
			if (siminterface->AutoIncludeInSimPipeline())
			{
				SimProcessingPhaseConfig.ProcessorCDOs.Add(ProcessorCDO);
			}
		}
	}

	ProcessorCDOs.Sort([](UMassProcessor& LHS, UMassProcessor& RHS) {
		return LHS.GetName().Compare(RHS.GetName()) < 0;
		});
}