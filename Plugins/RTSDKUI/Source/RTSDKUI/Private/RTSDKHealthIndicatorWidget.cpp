// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSDKHealthIndicatorWidget.h"
#include "RTSDKUnitComponent.h"
#include "RTSDKScriptExecutionContext.h"

void URTSDKHealthIndicatorWidget::SetUnitsToDisplay(const TArray<URTSDKUnitComponent*>& NewUnits)
{
    //first we iterate the current units and find ones that arent in the new units, unbinding events and discarding them
    for (int32 i = 0; i < UnitsToDisplay.Num(); i++)
    {
        //is it valid?
        if (UnitsToDisplay[i] != nullptr)
        {
            //not in the new units?
            if (!NewUnits.Contains(UnitsToDisplay[i]))
            {
                UnitsToDisplay[i]->OnUnitHealthChanged.RemoveAll(this);
                UnitsToDisplay[i]->OnUnitMaxHealthChanged.RemoveAll(this);
                UnitsToDisplay[i]->OnUnitDamaged.RemoveAll(this);
                UnitsToDisplay[i]->OnUnitDied.RemoveAll(this);
                UnitsToDisplay[i]->OnUnitRemoved.RemoveAll(this);
                UnitsToDisplay.RemoveAt(i);
                i--;//counters increment so we iterate to same value of i next loop.
            }
        }
        else
        {
            //if it was already a null pointer then we remove it now
            UnitsToDisplay.RemoveAt(i);
            i--;
        }
    }
    //now we add new units that arent already in the current units, binding their events.
    for (int32 i = 0; i < NewUnits.Num(); i++)
    {
        //valid?
        if (NewUnits[i] != nullptr)
        {
            //not in current units?
            if (!UnitsToDisplay.Contains(NewUnits[i]))
            {
                UnitsToDisplay[i]->OnUnitHealthChanged.AddDynamic(this, &URTSDKHealthIndicatorWidget::CurrentHealthChanged);
                UnitsToDisplay[i]->OnUnitMaxHealthChanged.AddDynamic(this, &URTSDKHealthIndicatorWidget::MaxHealthChanged);
                UnitsToDisplay[i]->OnUnitDamaged.AddDynamic(this, &URTSDKHealthIndicatorWidget::UnitTakeDamage);
                UnitsToDisplay[i]->OnUnitDied.AddDynamic(this, &URTSDKHealthIndicatorWidget::UnitDied);
                UnitsToDisplay[i]->OnUnitRemoved.AddDynamic(this, &URTSDKHealthIndicatorWidget::UnitRemoved);
                UnitsToDisplay.Add(NewUnits[i]);
            }
        }
    }

    Health = GetTotalHealthToDisplay();
    MaxHealth = GetTotalMaxHealthToDisplay();
    PercentHealth = GetPercentHealthToDisplay();

    if (UnitsToDisplay.Num() <= 0)
    {
        SetVisibility(ESlateVisibility::Collapsed);
    }
    else if (PercentHealth >= FixedPoint::Constants::Fixed64::One)
    {
        SetVisibility(ESlateVisibility::Collapsed);
    }
    else
    {
        SetVisibility(ESlateVisibility::SelfHitTestInvisible);
    }
    OnUnitsToDisplayChanged();
}

FFixed64 URTSDKHealthIndicatorWidget::GetTotalHealthToDisplay()
{
    FFixed64 retval = FFixed64::MakeFromRawInt(0);
    for (int32 i = 0; i < UnitsToDisplay.Num(); i++)
    {
        if (UnitsToDisplay[i] != nullptr)
        {
            retval += UnitsToDisplay[i]->GetUnitHealth();
        }
    }
    return retval;
}

FFixed64 URTSDKHealthIndicatorWidget::GetTotalMaxHealthToDisplay()
{
    FFixed64 retval = 0.0f;
    for (int32 i = 0; i < UnitsToDisplay.Num(); i++)
    {
        if (UnitsToDisplay[i] != nullptr)
        {
            retval += UnitsToDisplay[i]->GetUnitMaxHealth();
        }
    }
    return retval;
}

FFixed64 URTSDKHealthIndicatorWidget::GetAverageHealthToDisplay()
{
    FFixed64 retval = Health;
    if (UnitsToDisplay.Num() > 0)
    {
        retval /= UnitsToDisplay.Num();
    }
    return retval;
}

FFixed64 URTSDKHealthIndicatorWidget::GetAverageMaxHealthToDisplay()
{
    float retval = MaxHealth;
    if (UnitsToDisplay.Num() > 0)
    {
        retval /= UnitsToDisplay.Num();
    }
    return retval;
}

FFixed64 URTSDKHealthIndicatorWidget::GetPercentHealthToDisplay()
{
    FFixed64 current = Health;
    FFixed64 max = FFixedPointMath::Max(MaxHealth,FixedPoint::Constants::Fixed64::One);
    return current / max;
}

void URTSDKHealthIndicatorWidget::CurrentHealthChanged(FRTSDKScriptExecutionContext ScriptContext, URTSDKUnitComponent* Sender, const FFixed64& NewCurrentHealth, const FFixed64& OldCurrentHealth)
{
    Health -= (OldCurrentHealth - NewCurrentHealth);
    PercentHealth = GetPercentHealthToDisplay();
    if (UnitsToDisplay.Contains(Sender))
    {
        if (UnitsToDisplay.Num() <= 0)
        {
            SetVisibility(ESlateVisibility::Collapsed);
        }
        else if (PercentHealth >= FixedPoint::Constants::Fixed64::One)//todo: make this an option, why was this hard coded? lazy past me
        {
            SetVisibility(ESlateVisibility::Collapsed);
        }
        else
        {
            SetVisibility(ESlateVisibility::SelfHitTestInvisible);
        }
    }
    OnCurrentHealthChanged(Sender, NewCurrentHealth, OldCurrentHealth);
}

void URTSDKHealthIndicatorWidget::MaxHealthChanged(FRTSDKScriptExecutionContext ScriptContext, URTSDKUnitComponent* Sender, const FFixed64& NewMaxHealth, const FFixed64& OldMaxHealth)
{
    MaxHealth -= (OldMaxHealth - NewMaxHealth);
    PercentHealth = GetPercentHealthToDisplay();
    if (UnitsToDisplay.Contains(Sender))
    {
        if (UnitsToDisplay.Num() <= 0)
        {
            SetVisibility(ESlateVisibility::Collapsed);
        }
        else if (PercentHealth >= FixedPoint::Constants::Fixed64::One)
        {
            SetVisibility(ESlateVisibility::Collapsed);
        }
        else
        {
            SetVisibility(ESlateVisibility::SelfHitTestInvisible);
        }
    }
    OnMaxHealthChanged(Sender, NewMaxHealth, OldMaxHealth);
}

void URTSDKHealthIndicatorWidget::UnitTakeDamage(FRTSDKScriptExecutionContext ScriptContext, URTSDKUnitComponent* Sender, const FRTSDKDamageInfo& DamageEvent)
{
    OnUnitTakeDamage(Sender, DamageEvent);
}

void URTSDKHealthIndicatorWidget::UnitDied(FRTSDKScriptExecutionContext ScriptContext, URTSDKUnitComponent* Sender, const FRTSDKDamageInfo& KillingHitDamageEvent)
{
    if (UnitsToDisplay.Contains(Sender))
    {
        UnitsToDisplay.Remove(Sender);
        Sender->OnUnitHealthChanged.RemoveAll(this);
        Sender->OnUnitMaxHealthChanged.RemoveAll(this);
        Sender->OnUnitDamaged.RemoveAll(this);
        Sender->OnUnitDied.RemoveAll(this);
        Sender->OnUnitRemoved.RemoveAll(this);
        Health = GetTotalHealthToDisplay();
        MaxHealth = GetTotalMaxHealthToDisplay();
        PercentHealth = GetPercentHealthToDisplay();
        if (UnitsToDisplay.Num() <= 0)
        {
            SetVisibility(ESlateVisibility::Collapsed);
        }
        else if (PercentHealth >= FixedPoint::Constants::Fixed64::One)
        {
            SetVisibility(ESlateVisibility::Collapsed);
        }
        else
        {
            SetVisibility(ESlateVisibility::SelfHitTestInvisible);
        }
    }
    OnUnitDied(Sender, KillingHitDamageEvent);
}

void URTSDKHealthIndicatorWidget::UnitRemoved(FRTSDKScriptExecutionContext ScriptContext, URTSDKUnitComponent* RemovedUnit)
{
    if (UnitsToDisplay.Contains(RemovedUnit))
    {
        UnitsToDisplay.Remove(RemovedUnit);
        Health = GetTotalHealthToDisplay();
        MaxHealth = GetTotalMaxHealthToDisplay();
        PercentHealth = GetPercentHealthToDisplay();
        if (UnitsToDisplay.Num() <= 0)
        {
            SetVisibility(ESlateVisibility::Collapsed);
        }
        else if (GetPercentHealthToDisplay() >= FixedPoint::Constants::Fixed64::One)
        {
            SetVisibility(ESlateVisibility::Collapsed);
        }
        else
        {
            SetVisibility(ESlateVisibility::SelfHitTestInvisible);
        }
    }
    OnUnitRemoved(RemovedUnit);
}