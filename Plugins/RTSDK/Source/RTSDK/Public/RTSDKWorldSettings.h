// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/WorldSettings.h"
#include "RTSDKWorldSettings.generated.h"

USTRUCT()
struct FRTSDKPIEMatchBoolOption
{
	GENERATED_BODY()

public:

	UPROPERTY()
		FString OptionName;

	UPROPERTY()
		bool Value;
};

USTRUCT()
struct FRTSDKPIEMatchDiscreteOption
{
	GENERATED_BODY()

public:

	UPROPERTY()
		FString OptionName;

	UPROPERTY()
		FString SelectedOption;

	UPROPERTY()
		TArray<FString> Options;
};

USTRUCT()
struct FRTSDKPIEMatchScalarOption
{
	GENERATED_BODY()

public:

	UPROPERTY()
		FString OptionName;

	UPROPERTY()
		double Value;

	UPROPERTY()
		double MinValue;

	UPROPERTY()
		double MaxValue;
};

USTRUCT()
struct FRTSDKPIEMatchVector2dOption
{
	GENERATED_BODY()

public:

	UPROPERTY()
		FString OptionName;

	UPROPERTY()
		FVector2D Value;

	UPROPERTY()
		FVector2D MinValue;

	UPROPERTY()
		FVector2D MaxValue;
};

USTRUCT()
struct FRTSDKPIEMatchVector3dOption
{
	GENERATED_BODY()

public:

	UPROPERTY()
		FString OptionName;

	UPROPERTY()
		FVector Value;

	UPROPERTY()
		FVector MinValue;

	UPROPERTY()
		FVector MaxValue;
};

USTRUCT()
struct FRTSDKPIECommanderSetupInfo
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	FText CommanderDisplayName;

	UPROPERTY(EditAnywhere)
		TArray<FRTSDKPIEMatchBoolOption> CommanderBoolOptions;

	UPROPERTY(EditAnywhere)
		TArray<FRTSDKPIEMatchDiscreteOption> CommanderDiscreteOptions;

	UPROPERTY(EditAnywhere)
		TArray<FRTSDKPIEMatchScalarOption> CommanderScalarOptions;

	UPROPERTY(EditAnywhere)
		TArray<FRTSDKPIEMatchVector2dOption> CommanderVector2dOptions;

	UPROPERTY(EditAnywhere)
		TArray<FRTSDKPIEMatchVector3dOption> CommanderVector3dOptions;

	UPROPERTY(EditAnywhere)
	bool bIsBot;

	//Constructor
	FRTSDKPIECommanderSetupInfo()
	{
		CommanderDisplayName = FText::GetEmpty();
		bIsBot = false;
	}
};

USTRUCT()
struct FRTSDKPIEForceSetupInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FText ForceDisplayName;

	UPROPERTY(EditAnywhere)
		TArray<FRTSDKPIEMatchBoolOption> ForceBoolOptions;

	UPROPERTY(EditAnywhere)
		TArray<FRTSDKPIEMatchDiscreteOption> ForceDiscreteOptions;

	UPROPERTY(EditAnywhere)
		TArray<FRTSDKPIEMatchScalarOption> ForceScalarOptions;

	UPROPERTY(EditAnywhere)
		TArray<FRTSDKPIEMatchVector2dOption> ForceVector2dOptions;

	UPROPERTY(EditAnywhere)
		TArray<FRTSDKPIEMatchVector3dOption> ForceVector3dOptions;
	
	UPROPERTY(EditAnywhere)
	TArray<FRTSDKPIECommanderSetupInfo> PIECommanders;

	//Constructor
	FRTSDKPIEForceSetupInfo()
	{
		ForceDisplayName = FText::GetEmpty();
	}
};

USTRUCT()
struct FRTSDKPIETeamSetupInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FText TeamDisplayName;

	UPROPERTY(EditAnywhere)
		TArray<FRTSDKPIEMatchBoolOption> TeamBoolOptions;

	UPROPERTY(EditAnywhere)
		TArray<FRTSDKPIEMatchDiscreteOption> TeamDiscreteOptions;

	UPROPERTY(EditAnywhere)
		TArray<FRTSDKPIEMatchScalarOption> TeamScalarOptions;

	UPROPERTY(EditAnywhere)
		TArray<FRTSDKPIEMatchVector2dOption> TeamVector2dOptions;

	UPROPERTY(EditAnywhere)
		TArray<FRTSDKPIEMatchVector3dOption> TeamVector3dOptions;

	UPROPERTY(EditAnywhere)
	TArray<FRTSDKPIEForceSetupInfo> PIEForces;

	//Constructor
	FRTSDKPIETeamSetupInfo()
	{
		TeamDisplayName = FText::GetEmpty();
	}
};

//UPROPERTY(meta=(GetOptions="abc"))
//for the mod name properties, a function returning TArray<FString>, no args
//UFUNCTION(CallInEditor)
//for the refresh mod list function, no args, returns nothing


/**
 * 
 */
UCLASS()
class RTSDK_API ARTSDKWorldSettings : public AWorldSettings
{
	GENERATED_BODY()
public:
#if WITH_EDITORONLY_DATA

	UFUNCTION()
		TArray<FString> GetGameModNames();

	UFUNCTION()
		TArray<FString> GetMapModNames();

	UFUNCTION()
		TArray<FString> GetFactionModNames();

	UFUNCTION()
		TArray<FString> GetMutatorNames();

	UPROPERTY(EditAnywhere, meta = (GetOptions = "GetMapModNames"))
		FName PIEMapModName;

	UPROPERTY(EditAnywhere, meta = (GetOptions = "GetGameModNames"))
		FName PIEGameModName;

	UPROPERTY(EditAnywhere, meta = (GetOptions = "GetFactionModNames"))
		TArray<FName> PIEFactionModNames;

	UPROPERTY(EditAnywhere, meta = (GetOptions = "GetMutatorNames"))
		TArray<FName> PIEMutatorNames;

	UPROPERTY(EditAnywhere)
		TArray<FRTSDKPIETeamSetupInfo> PIEMatchSetup;

	UPROPERTY(EditAnywhere)
		bool PIEMatchIsLAN;
#endif //WITH_EDITORONLY_DATA

#if WITH_EDITOR
protected:
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif // WITH_EDITOR
};
