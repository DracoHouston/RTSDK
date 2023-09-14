// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/LocalPlayerSubsystem.h"
#include "RTSDKPlayerSettingsSubsystem.generated.h"

class URTSDKSharedUserSettingsBase;
class UGameUserSettings;
class URTSDKGameSettingRegistry;

/**
 * 
 */
UCLASS()
class RTSDK_API URTSDKPlayerSettingsSubsystem : public ULocalPlayerSubsystem
{
	GENERATED_BODY()
	
public:
	/** Implement this for initialization of instances of the system */
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	/** Implement this for deinitialization of instances of the system */
	virtual void Deinitialize() override;

	UFUNCTION()
		URTSDKGameSettingRegistry* GetSettingsRegistry();

	UFUNCTION()
		UGameUserSettings* GetLocalSettings() const;

	UFUNCTION()
		URTSDKSharedUserSettingsBase* GetSharedSettings() const;

protected:
	UPROPERTY(Transient)
		mutable TObjectPtr<URTSDKSharedUserSettingsBase> SharedSettings;

	UPROPERTY(Transient)
		mutable TObjectPtr<URTSDKGameSettingRegistry> SettingsRegistry;
};
