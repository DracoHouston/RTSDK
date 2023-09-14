// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MassSettings.h"
#include "MassEntitySettings.h"
#include "RTSDKMassModuleSettings.generated.h"

/**
 * 
 */
UCLASS(config = Mass, defaultconfig, DisplayName = "RTSDK Mass Settings")
class RTSDK_API URTSDKMassModuleSettings : public UMassModuleSettings
{
	GENERATED_BODY()
	
public:
#if WITH_EDITORONLY_DATA
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnSettingsChange, const FPropertyChangedEvent& /*PropertyChangedEvent*/);
#endif // WITH_EDITORONLY_DATA

	URTSDKMassModuleSettings(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	virtual void BeginDestroy() override;

	/**
	* Processing phase config, this phase will be triggered by the RTS Game Sim subsystem 
	* when the game sim does a fixed timestep tick
	*/
	UPROPERTY(EditDefaultsOnly, Category = Mass, config)
	FMassProcessingPhaseConfig SimProcessingPhaseConfig;

	/** This list contains all the sim interface implementing processors available in the given binary (including plugins). The contents are sorted by display name.*/
	UPROPERTY(VisibleAnywhere, Category = Mass, Transient, Instanced, EditFixedSize)
		TArray<TObjectPtr<UMassProcessor>> ProcessorCDOs;

	/**
	 * The name of the file to dump the processor dependency graph. T
	 * The dot file will be put in the project log folder.
	 * To generate a svg out of that file, simply run dot executable with following parameters: -Tsvg -O filename.dot
	 */
	UPROPERTY(EditDefaultsOnly, Category = Mass, Transient)
		FString DumpDependencyGraphFileName;

	void BuildProcessorListAndPhases();

	void BuildPhases();
	void BuildProcessorList();

protected:
	bool bInitialized = false;
};
