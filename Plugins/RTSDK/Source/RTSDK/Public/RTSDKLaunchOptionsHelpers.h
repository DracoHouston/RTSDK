// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once
#include "CoreMinimal.h"
#include "RTSDKLaunchOptionsHelpers.generated.h"

struct FRTSDKPIETeamSetupInfo;

USTRUCT()
struct RTSDK_API FRTSDKStateSetupInfo
{
	GENERATED_BODY()
public:
	UPROPERTY()
		TMap<FString, FString> OptionsMap;

	FRTSDKStateSetupInfo() {}
	FRTSDKStateSetupInfo(TMap<FString, FString>& inOptions) : OptionsMap(inOptions) {}
};

USTRUCT()
struct RTSDK_API FRTSDKLaunchOptionsHelpers
{
	GENERATED_BODY()
public:

#if WITH_EDITORONLY_DATA
	static FString GetLaunchOptionsFromPIEMatchSetup(const TArray<FRTSDKPIETeamSetupInfo>& inPIETeams, bool inPIEMatchIsLAN);
#endif
	static bool ExtractOptions(
		const FString& inOptionsString, 
		TMap<FString, FString>& outOtherOptionsMap,
		TMap<int32, FRTSDKStateSetupInfo>& outTeamOptionsMap, 
		TMap<int32, FRTSDKStateSetupInfo>& outForceOptionsMap, 
		TMap<int32, FRTSDKStateSetupInfo>& outCommanderOptionsMap);

	//Takes an unreal url options key/value and extracts our suboptions and the index suffix in the key.
	//if successful will set outSubOptionsMap to the options held in inValue, and return the index in inKey
	//if it fails to extract the index from the key it will return -1 and outSubOptionMap will be empty.
	static int32 ExtractSubOptions(const FString& inKey, const FString& inValue, int32 inPrefixLength, FRTSDKStateSetupInfo& outSubOptions);

	static bool ValidateStateSetupInfos(TMap<int32, FRTSDKStateSetupInfo>& outOptions);
};