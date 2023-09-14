// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RTSDKTypes.h"
#include "FixedPointTypes.h"
#include "RTSDKHealthIndicatorWidget.generated.h"

class URTSDKUnitComponent;
struct FRTSDKScriptExecutionContext;

/**
 * 
 */
UCLASS()
class RTSDKUI_API URTSDKHealthIndicatorWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	/**
	* Units we are tracking health for, in many cases this will be 1 unit
	* but we also must be a health bar for combos of units, hotgroups,
	* combined class items in a selection container, etc.
	* 
	* You should not have to read this manually, Getters take care of 
	* the specifics of getting health values out of units.
	*/
	UPROPERTY(BlueprintReadOnly, Transient, Category = "RTSDK Health Indicator Widget")
		TArray<TObjectPtr<URTSDKUnitComponent>> UnitsToDisplay;

	UPROPERTY(BlueprintReadOnly, Transient)
		FFixed64 Health;

	UPROPERTY(BlueprintReadOnly, Transient)
		FFixed64 MaxHealth;

	UPROPERTY(BlueprintReadOnly, Transient)
		FFixed64 PercentHealth;

	UFUNCTION()
		void SetUnitsToDisplay(const TArray<URTSDKUnitComponent*>& NewUnits);

	UFUNCTION(BlueprintImplementableEvent, Category = "RTSDK Health Indicator Widget")
		void OnUnitsToDisplayChanged();

	UFUNCTION(BlueprintPure, Category = "RTSDK Health Indicator Widget")
		FFixed64 GetTotalHealthToDisplay();

	UFUNCTION(BlueprintPure, Category = "RTSDK Health Indicator Widget")
		FFixed64 GetTotalMaxHealthToDisplay();

	UFUNCTION(BlueprintPure, Category = "RTSDK Health Indicator Widget")
		FFixed64 GetAverageHealthToDisplay();

	UFUNCTION(BlueprintPure, Category = "RTSDK Health Indicator Widget")
		FFixed64 GetAverageMaxHealthToDisplay();

	UFUNCTION(BlueprintPure, Category = "RTSDK Health Indicator Widget")
		FFixed64 GetPercentHealthToDisplay();

	UFUNCTION()
		void CurrentHealthChanged(FRTSDKScriptExecutionContext ScriptContext, URTSDKUnitComponent* Sender, const FFixed64& NewCurrentHealth, const FFixed64& OldCurrentHealth);

	UFUNCTION(BlueprintImplementableEvent, Category = "RTSDK Health Indicator Widget|Events")
		void OnCurrentHealthChanged(URTSDKUnitComponent* Sender, double NewCurrentHealth, double OldCurrentHealth);

	UFUNCTION()
		void MaxHealthChanged(FRTSDKScriptExecutionContext ScriptContext, URTSDKUnitComponent* Sender, const FFixed64& NewMaxHealth, const FFixed64& OldMaxHealth);

	UFUNCTION(BlueprintImplementableEvent, Category = "RTSDK Health Indicator Widget|Events")
		void OnMaxHealthChanged(URTSDKUnitComponent* Sender, double NewMaxHealth, double OldMaxHealth);

	UFUNCTION()
		void UnitTakeDamage(FRTSDKScriptExecutionContext ScriptContext, URTSDKUnitComponent* Sender, const FRTSDKDamageInfo& DamageEvent);

	UFUNCTION(BlueprintImplementableEvent, Category = "RTSDK Health Indicator Widget|Events")
		void OnUnitTakeDamage(URTSDKUnitComponent* Sender, const FRTSDKDamageInfo& DamageEvent);

	UFUNCTION()
		void UnitDied(FRTSDKScriptExecutionContext ScriptContext, URTSDKUnitComponent* Sender, const FRTSDKDamageInfo& KillingHitDamageEvent);

	UFUNCTION(BlueprintImplementableEvent, Category = "RTSDK Health Indicator Widget|Events")
		void OnUnitDied(URTSDKUnitComponent* Sender, const FRTSDKDamageInfo& KillingHitDamageEvent);

	UFUNCTION()
		void UnitRemoved(FRTSDKScriptExecutionContext ScriptContext, URTSDKUnitComponent* Sender);

	UFUNCTION(BlueprintImplementableEvent, Category = "RTSDK Health Indicator Widget|Events")
		void OnUnitRemoved(URTSDKUnitComponent* RemovedUnit);
};
