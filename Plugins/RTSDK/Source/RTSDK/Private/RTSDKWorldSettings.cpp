// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSDKWorldSettings.h"
#include "RTSDKModManager.h"

#if WITH_EDITORONLY_DATA
TArray<FString> ARTSDKWorldSettings::GetGameModNames()
{
    URTSDKModManager* modmanager = GEngine->GetEngineSubsystem<URTSDKModManager>();
    TArray<FName> names = modmanager->GetAllValidGameModNames();
    TArray<FString> retval;
    for (int32 i = 0; i < names.Num(); i++)
    {
        retval.Add(names[i].ToString());
    }
    return retval;
}

TArray<FString> ARTSDKWorldSettings::GetMapModNames()
{
    URTSDKModManager* modmanager = GEngine->GetEngineSubsystem<URTSDKModManager>();
    TArray<FName> names = modmanager->GetAllValidMapModNames();
    TArray<FString> retval;
    for (int32 i = 0; i < names.Num(); i++)
    {
        retval.Add(names[i].ToString());
    }
    return retval;
}

TArray<FString> ARTSDKWorldSettings::GetFactionModNames()
{
    URTSDKModManager* modmanager = GEngine->GetEngineSubsystem<URTSDKModManager>();
    TArray<FName> names = modmanager->GetAllValidFactionModNames();
    TArray<FString> retval;
    for (int32 i = 0; i < names.Num(); i++)
    {
        retval.Add(names[i].ToString());
    }
    return retval;
}

TArray<FString> ARTSDKWorldSettings::GetMutatorNames()
{
    URTSDKModManager* modmanager = GEngine->GetEngineSubsystem<URTSDKModManager>();
    TArray<FName> names = modmanager->GetAllValidMutatorNames();
    TArray<FString> retval;
    for (int32 i = 0; i < names.Num(); i++)
    {
        retval.Add(names[i].ToString());
    }
    return retval;
}
#endif //WITH_EDITORONLY_DATA

#if WITH_EDITOR
void ARTSDKWorldSettings::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
    //property name is the one that is changed
    //member property name is the struct/array of structs property it is inside
    //so we can override adds and removes to the options and enforce the mod selections options
    //and monitor changes to options
    //additionally, we can monitor adds of teams, forces and commanders and fill out their options from defaults
    //note: we should have different names for the properties within the team/forces/commanders structs to ensure
    //we can filter based on where the option is etc
    if (PropertyChangedEvent.ChangeType == EPropertyChangeType::ArrayAdd)
    {
        return;
    }

}
#endif // WITH_EDITOR
