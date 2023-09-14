// Fill out your copyright notice in the Description page of Project Settings.

#include "RTSDKLaunchOptionsHelpers.h"
#include "RTSDKWorldSettings.h"
#include "Kismet/GameplayStatics.h"

FString FRTSDKLaunchOptionsHelpers::GetLaunchOptionsFromPIEMatchSetup(const TArray<FRTSDKPIETeamSetupInfo>& inPIETeams, bool inPIEMatchIsLAN)
{
    FString PIELaunchOptions;
    int32 commanderidx = 0;
    int32 forceidx = 0;
    int32 humanidx = 0;
    //construct the new url options to pass into sim state setup
    if (inPIEMatchIsLAN)
    {
        PIELaunchOptions += TEXT("?bIsLANMatch");
    }
    //teams
    for (int32 t = 0; t < inPIETeams.Num(); t++)
    {
        PIELaunchOptions += FString::Printf(TEXT("?Team%i=|Name=%s"), t, *inPIETeams[t].TeamDisplayName.ToString());
        /*PIELaunchOptions = inPIETeams[t].AdditionalOptions.IsEmpty() ?
            PIELaunchOptions :
            PIELaunchOptions + FString::Printf(TEXT("%s"), t, *inPIETeams[t].AdditionalOptions);*/
        //forces
        for (int32 f = 0; f < inPIETeams[t].PIEForces.Num(); f++)
        {
            PIELaunchOptions += FString::Printf(TEXT("?Force%i=|Name=%s|Team=%i"), forceidx, *inPIETeams[t].PIEForces[f].ForceDisplayName.ToString(), t);
            /*PIELaunchOptions = inPIETeams[t].PIEForces[f].AdditionalOptions.IsEmpty() ?
                PIELaunchOptions :
                PIELaunchOptions + FString::Printf(TEXT("%s"), t, *inPIETeams[t].PIEForces[f].AdditionalOptions);*/
            //commanders
            for (int32 c = 0; c < inPIETeams[t].PIEForces[f].PIECommanders.Num(); c++)
            {
                PIELaunchOptions = inPIETeams[t].PIEForces[f].PIECommanders[c].bIsBot ?
                    PIELaunchOptions + FString::Printf(TEXT("?Commander%i=|Name=%s|Force=%i|IsPlayer=false"), commanderidx, *inPIETeams[t].PIEForces[f].PIECommanders[c].CommanderDisplayName.ToString(), f) :
                    PIELaunchOptions + FString::Printf(TEXT("?Commander%i=|Name=%s|Force=%i|IsPlayer=true|PlayerID=%i"), commanderidx, *inPIETeams[t].PIEForces[f].PIECommanders[c].CommanderDisplayName.ToString(), f, humanidx);
                /*PIELaunchOptions = inPIETeams[t].PIEForces[f].PIECommanders[c].AdditionalOptions.IsEmpty() ?
                    PIELaunchOptions :
                    PIELaunchOptions + FString::Printf(TEXT("%s"), t, *inPIETeams[t].PIEForces[f].PIECommanders[c].AdditionalOptions);*/
                commanderidx++;
                humanidx = inPIETeams[t].PIEForces[f].PIECommanders[c].bIsBot ? humanidx : humanidx + 1;
            }
            forceidx++;
        }
    }
    return PIELaunchOptions;
}

bool FRTSDKLaunchOptionsHelpers::ExtractOptions(const FString& inOptionsString, TMap<FString, FString>& outOtherOptionsMap, TMap<int32, FRTSDKStateSetupInfo>& outTeamOptionsMap, TMap<int32, FRTSDKStateSetupInfo>& outForceOptionsMap, TMap<int32, FRTSDKStateSetupInfo>& outCommanderOptionsMap)
{
    const FString TeamToken = TEXT("Team");
    const FString ForceToken = TEXT("Force");
    const FString CommanderToken = TEXT("Commander");
    
    FString LaunchOptions = inOptionsString;
    FString Pair, PairKey, PairValue;
    while (UGameplayStatics::GrabOption(LaunchOptions, Pair))
    {
        UGameplayStatics::GetKeyValue(Pair, PairKey, PairValue);
        if (PairKey.StartsWith(TeamToken))
        {
            FRTSDKStateSetupInfo SubParams;
            int32 idx = ExtractSubOptions(PairKey, PairValue, TeamToken.Len(), SubParams);
            if (idx != -1)
            {
                outTeamOptionsMap.Add(idx, SubParams);
            }
        }
        else if (PairKey.StartsWith(ForceToken))
        {
            FRTSDKStateSetupInfo SubParams;
            int32 idx = ExtractSubOptions(PairKey, PairValue, ForceToken.Len(), SubParams);
            if (idx != -1)
            {
                outForceOptionsMap.Add(idx, SubParams);
            }
        }
        else if (PairKey.StartsWith(CommanderToken))
        {
            FRTSDKStateSetupInfo SubParams;
            int32 idx = ExtractSubOptions(PairKey, PairValue, CommanderToken.Len(), SubParams);
            if (idx != -1)
            {
                outCommanderOptionsMap.Add(idx, SubParams);
            }
        }
        else
        {
            outOtherOptionsMap.Add(PairKey, PairValue);
        }
    }
    //now we can go through each declaration, they are sorted by index number first
    bool teamsarevalid = ValidateStateSetupInfos(outTeamOptionsMap);
    bool forcesarevalid = ValidateStateSetupInfos(outForceOptionsMap);
    bool commandersarevalid = ValidateStateSetupInfos(outCommanderOptionsMap);
    return teamsarevalid && forcesarevalid && commandersarevalid;
}

int32 FRTSDKLaunchOptionsHelpers::ExtractSubOptions(const FString& inKey, const FString& inValue, int32 inPrefixLength, FRTSDKStateSetupInfo& outSubOptions)
{
    const FString OurDelimiter = TEXT("|");
    const FString EpicsDelimiter = TEXT("?");

    FString idxstring = inKey.RightChop(inPrefixLength);
    if (idxstring.IsNumeric())
    {
        int32 idx = FCString::Atoi(*idxstring);
        FString SubOptions = inValue.Replace(*OurDelimiter, *EpicsDelimiter);
        FString SubPair, SubPairKey, SubPairValue;
        FRTSDKStateSetupInfo SubParams;
        while (UGameplayStatics::GrabOption(SubOptions, SubPair))
        {
            UGameplayStatics::GetKeyValue(SubPair, SubPairKey, SubPairValue);
            SubParams.OptionsMap.Add(SubPairKey, SubPairValue);
        }
        outSubOptions = SubParams;
        return idx;
    }
    return -1;
}

bool FRTSDKLaunchOptionsHelpers::ValidateStateSetupInfos(TMap<int32, FRTSDKStateSetupInfo>& outOptions)
{
    outOptions.KeySort(TLess<int32>());
    int32 idx = 0;
    for (auto It = outOptions.CreateConstIterator(); It; ++It)
    {
        if (It->Key == idx)
        {
            idx++;
        }
        else
        {
            return false;
        }
    }
    return true;
}
