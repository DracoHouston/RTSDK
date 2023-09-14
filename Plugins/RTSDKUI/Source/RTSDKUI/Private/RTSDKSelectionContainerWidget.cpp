// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "RTSDKSelectionContainerWidget.h"
#include "RTSDKCommanderState.h"
#include "RTSDKSelectionItemWidget.h"
#include "RTSDKPlayerControllerInterface.h"
/*
void URTSDKSelectionContainerWidget::NativeOnInitialized()
{
	if (CommanderToDisplay != nullptr)
	{
		SetPlayerToDisplay(CommanderToDisplay);
	}
	else
	{
		APlayerController* owningpc = GetOwningPlayer();
		if (owningpc != nullptr)
		{
			if (owningpc->PlayerState != nullptr)
			{
				ARTSPlayerState* rtsps = Cast<ARTSPlayerState>(owningpc->PlayerState);
				if (rtsps != nullptr)
				{
					SetPlayerToDisplay(rtsps);
				}
			}
		}
	}
	Super::NativeOnInitialized();
}
*/


void URTSDKSelectionContainerWidget::NativeDestruct()
{
	if (CommanderToDisplay != nullptr)
	{
		CommanderToDisplay->OnSelectionChanged.RemoveAll(this);
		CommanderToDisplay->OnSubSelectionChanged.RemoveAll(this);
	}
	Super::NativeDestruct();
}

void URTSDKSelectionContainerWidget::SetCommanderToDisplay(ARTSDKCommanderStateBase* inCommander)
{
	if (CommanderToDisplay != nullptr)
	{
		CommanderToDisplay->OnSelectionChanged.RemoveAll(this);
		CommanderToDisplay->OnSubSelectionChanged.RemoveAll(this);
	}
	if (inCommander != nullptr)
	{
		CommanderToDisplay = inCommander;
		CommanderToDisplay->OnSelectionChanged.AddDynamic(this, &URTSDKSelectionContainerWidget::SelectionChanged);
		CommanderToDisplay->OnSubSelectionChanged.AddDynamic(this, &URTSDKSelectionContainerWidget::SubSelectionChanged);
		SelectionToDisplay = CommanderToDisplay->GetCurrentSelection();
	}
}

void URTSDKSelectionContainerWidget::SetCommanderToDisplayToLocalPlayer()
{
	APlayerController* owningpc = GetOwningPlayer();
	if (owningpc != nullptr)
	{
		IRTSDKPlayerControllerInterface* rtsdkpc = Cast<IRTSDKPlayerControllerInterface>(owningpc);
		if (rtsdkpc != nullptr)
		{
			ARTSDKCommanderStateBase* commanderstate = rtsdkpc->GetCommanderState();
			if (commanderstate != nullptr)
			{
				SetCommanderToDisplay(commanderstate);
				return;
			}
		}
	}
	SetCommanderToDisplay(nullptr);
}

void URTSDKSelectionContainerWidget::SelectionChanged(ARTSDKCommanderStateBase* Sender, const FRTSDKCommanderSelectionInfo& NewSelection)
{
	SelectionToDisplay = NewSelection;

	TArray<FRTSDKUIUnitsCollection> unitcollections;
	if (CombineSameClassUnits)
	{
		for (auto It = NewSelection.UnitsByType.CreateConstIterator(); It; ++It)
		{
			FRTSDKUIUnitsCollection units;
			units.Definition = It->Key;
			for (auto InnerIt = It->Value.Units.CreateConstIterator(); InnerIt; ++InnerIt)
			{
				units.Items.Add(*InnerIt);
			}
			unitcollections.Add(units);
		}	
	}
	else
	{
		TArray<FRTSDKCommanderSelectionUnitWithDefinition> allunits = NewSelection.GetAllUnitsWithDefinitions();
		for (auto It = allunits.CreateConstIterator(); It; ++It)
		{
			FRTSDKUIUnitsCollection units;
			units.Definition = It->UnitDefinition;
			units.Items.Add(FRTSDKCommanderSelectionUnit(It->UnitID, It->Unit));
			unitcollections.Add(units);
		}
	}

	int32 diff = unitcollections.Num() - SelectionItemWidgets.Num();
	if (diff > 0)
	{
		OnRequireMoreWidgets(diff);
	}
	else if (diff < 0)
	{
		OnRequireLessWidgets(-diff);
	}

	for (int32 i = 0; i < SelectionItemWidgets.Num(); i++)
	{
		SelectionItemWidgets[i]->SetUnitsToDisplay(unitcollections[i]);
	}
	OnSelectionChanged(Sender, NewSelection);
}

void URTSDKSelectionContainerWidget::SubSelectionChanged(ARTSDKCommanderStateBase* Sender, const FRTSDKCommanderSelectionUnit& NewSubSelection)
{
	for (int32 i = 0; i < SelectionItemWidgets.Num(); i++)
	{
		if (SelectionItemWidgets[i] != nullptr)
		{
			//SelectionItemWidgets[i]->SetUnitsToDisplay(Sender, NewSubSelection);
		}
	}
	OnSubSelectionChanged(Sender, NewSubSelection);
}

int32 URTSDKSelectionContainerWidget::GetSelectionCount()
{
	if (CommanderToDisplay == nullptr)
	{
		return -1;
	}
	if (CombineSameClassUnits)
	{
		return SelectionToDisplay.UnitsByType.Num();
	}
	else
	{
		return SortedSelectionUnits.Num();
	}
}
