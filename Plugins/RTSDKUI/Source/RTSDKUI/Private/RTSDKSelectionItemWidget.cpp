// Fill out your copyright notice in the Description page of Project Settings.

#include "RTSDKSelectionItemWidget.h"
#include "RTSDKSelectionContainerWidget.h"
#include "RTSDKPlayerControllerInterface.h"
#include "RTSDKCommanderState.h"
#include "RTSDKUnitComponent.h"
#include "RTSDKUnitDefinition.h"
#include "RTSDKUIHelpers.h"
/*
void URTSDKSelectionItemWidget::NativeOnInitialized()
{
	if (ParentContainer != nullptr)
	{
		PairWithRTSSelectionContainerWidget(ParentContainer);
	}
	else if (CommanderToDisplay != nullptr)//unpaired we must track this ourselves.
	{
		CommanderToDisplay->OnPlayerSelectionChangedDelegate.AddDynamic(this, &URTSDKSelectionItemWidget::HandlePlayerSelectionChanged);
		CommanderToDisplay->OnPlayerSubSelectionChangedDelegate.AddDynamic(this, &URTSDKSelectionItemWidget::HandlePlayerSubSelectionChanged);

		//trigger the event in here to trigger any initial display setup
		OnPlayerSelectionChanged(CommanderToDisplay, CommanderToDisplay->PlayerSelection);
		OnPlayerSubSelectionChanged(CommanderToDisplay, CommanderToDisplay->PlayerSubSelectionUnit, CommanderToDisplay->PlayerSubSelectionClass);
	}
	else//unpaired and not given a player to display we must find the rts player state of the owner of this widget, who will be the local player.
	{
		APlayerController* owningpc = GetOwningPlayer();
		if (owningpc != nullptr)
		{
			if (owningpc->PlayerState != nullptr)
			{
				ARTSPlayerState* rtsps = Cast<ARTSPlayerState>(owningpc->PlayerState);
				if (rtsps != nullptr)
				{
					CommanderToDisplay = rtsps;
					CommanderToDisplay->OnPlayerSelectionChangedDelegate.AddDynamic(this, &URTSDKSelectionItemWidget::HandlePlayerSelectionChanged);
					CommanderToDisplay->OnPlayerSubSelectionChangedDelegate.AddDynamic(this, &URTSDKSelectionItemWidget::HandlePlayerSubSelectionChanged);

					//trigger the event in here to trigger any initial display setup
					OnPlayerSelectionChanged(CommanderToDisplay, CommanderToDisplay->PlayerSelection);
					OnPlayerSubSelectionChanged(CommanderToDisplay, CommanderToDisplay->PlayerSubSelectionUnit, CommanderToDisplay->PlayerSubSelectionClass);
				}
			}
		}
	}
	Super::NativeOnInitialized();
}
*/
void URTSDKSelectionItemWidget::NativeDestruct()
{
	//unbind from all events, we are being destroyed soon.
	for (int32 i = 0; i < UnitsToDisplay.Items.Num(); i++)
	{
		if (UnitsToDisplay.Items[i].Unit != nullptr)
		{
			UnitsToDisplay.Items[i].Unit->OnUnitDied.RemoveAll(this);
			UnitsToDisplay.Items[i].Unit->OnUnitRemoved.RemoveAll(this);
			UnitsToDisplay.Items[i].Unit->OnUnitIconChanged.RemoveAll(this);
			UnitsToDisplay.Items[i].Unit->OnUnitDisplayNameChanged.RemoveAll(this);
		}
	}
	Super::NativeDestruct();
}

void URTSDKSelectionItemWidget::SetUnitsToDisplay(const FRTSDKUIUnitsCollection& inUnits)
{
	for (int32 i = 0; i < UnitsToDisplay.Items.Num(); i++)
	{
		if (UnitsToDisplay.Items[i].Unit != nullptr)
		{
			UnitsToDisplay.Items[i].Unit->OnUnitDied.RemoveAll(this);
			UnitsToDisplay.Items[i].Unit->OnUnitRemoved.RemoveAll(this);
			UnitsToDisplay.Items[i].Unit->OnUnitIconChanged.RemoveAll(this);
			UnitsToDisplay.Items[i].Unit->OnUnitDisplayNameChanged.RemoveAll(this);
		}
	}
	for (int32 i = 0; i < inUnits.Items.Num(); i++)
	{
		if (inUnits.Items[i].Unit != nullptr)
		{
			inUnits.Items[i].Unit->OnUnitDied.AddDynamic(this, &URTSDKSelectionItemWidget::UnitDied);
			inUnits.Items[i].Unit->OnUnitRemoved.AddDynamic(this, &URTSDKSelectionItemWidget::UnitRemoved);
			inUnits.Items[i].Unit->OnUnitIconChanged.AddDynamic(this, &URTSDKSelectionItemWidget::UnitIconChanged);
			inUnits.Items[i].Unit->OnUnitDisplayNameChanged.AddDynamic(this, &URTSDKSelectionItemWidget::UnitDisplayNameChanged);
		}
	}
	UnitsToDisplay = inUnits;
	OnSetUnitsToDisplay();
}

void URTSDKSelectionItemWidget::SetIsSubselected(bool inIsSubselected)
{
	bIsSubselected = inIsSubselected;
	OnSetIsSubselected();
}
//
//void URTSDKSelectionItemWidget::HandlePlayerSelectionChanged(ARTSPlayerState* Sender, const TArray<AActor*>& NewPlayerSelection)
//{
//	OnPlayerSelectionChanged(Sender, NewPlayerSelection);
//}
//
//bool URTSDKSelectionItemWidget::OnPlayerSelectionChanged_Implementation(ARTSPlayerState* Sender, const TArray<AActor*>& NewPlayerSelection)
//{
//	TArray<AActor*> newunitstodisplay = GetAllUnitsForThisItem();// = ParentContainer->PlayerSelectionToDisplay[SelectionIndex];
//	if (newunitstodisplay.Num() > 0)//we have units
//	{
//		//bind new units we arent already monitoring and add them
//		for (int32 i = 0; i < newunitstodisplay.Num(); i++)
//		{
//			if (newunitstodisplay[i] != nullptr)
//			{
//				if (!UnitsToDisplay.Contains(newunitstodisplay[i]))
//				{
//					IRTSUnitInterface::Execute_BindToOnUnitDied(newunitstodisplay[i], this, FName(TEXT("HandleUnitDied")));
//					newunitstodisplay[i]->OnDestroyed.AddDynamic(this, &URTSDKSelectionItemWidget::HandleUnitDestroyed);
//					IRTSUnitInterface::Execute_BindToOnUnitDisplayNameChanged(newunitstodisplay[i], this, FName(TEXT("HandleUnitDisplayNameChanged")));
//					IRTSUnitInterface::Execute_BindToOnUnitDisplayNameChanged(newunitstodisplay[i], this, FName(TEXT("HandleUnitIconChanged")));
//					UnitsToDisplay.Add(newunitstodisplay[i]);
//				}
//			}
//		}
//		//unbind units we are monitoring that are not in the new units array
//		for (int32 i = 0; i < UnitsToDisplay.Num(); i++)
//		{
//			if (UnitsToDisplay[i] != nullptr)
//			{
//				if (!newunitstodisplay.Contains(UnitsToDisplay[i]))
//				{
//					IRTSUnitInterface::Execute_UnbindAllFromOnUnitDied(UnitsToDisplay[i], this);
//					UnitsToDisplay[i]->OnDestroyed.RemoveAll(this);
//					IRTSUnitInterface::Execute_UnbindAllFromOnUnitDisplayNameChanged(UnitsToDisplay[i], this);
//					IRTSUnitInterface::Execute_UnbindAllFromOnUnitIconChanged(UnitsToDisplay[i], this);
//					UnitsToDisplay.RemoveAt(i);
//					i--;//decrement to counter the increment ('i' will be the same next loop)
//				}
//			}
//		}
//	}
//	else
//	{
//		//unbind units we are monitoring, we have nothing to display for now.
//		for (int32 i = 0; i < UnitsToDisplay.Num(); i++)
//		{
//			if (UnitsToDisplay[i] != nullptr)
//			{
//				IRTSUnitInterface::Execute_UnbindAllFromOnUnitDied(UnitsToDisplay[i], this);
//				UnitsToDisplay[i]->OnDestroyed.RemoveAll(this);
//				IRTSUnitInterface::Execute_UnbindAllFromOnUnitDisplayNameChanged(UnitsToDisplay[i], this);
//				IRTSUnitInterface::Execute_UnbindAllFromOnUnitIconChanged(UnitsToDisplay[i], this);
//				UnitsToDisplay.RemoveAt(i);
//				i--;//decrement to counter the increment ('i' will be the same next loop)
//			}
//		}
//	}
//	if (CombineSameClassUnits)
//	{
//		AActor* itemunit = GetFirstUnitForThisItem();
//		if (itemunit != nullptr)
//		{
//			TSubclassOf<AActor> itemunitselectionclass = IRTSUnitInterface::Execute_GetUnitSelectionClass(itemunit);
//			if (itemunitselectionclass != nullptr)
//			{
//				UObject* cdo = itemunitselectionclass->GetDefaultObject();
//				if (cdo != nullptr)
//				{
//					OnUnitDisplayNameChanged(itemunit, IRTSUnitInterface::Execute_GetUnitDisplayName(cdo), FText::GetEmpty());
//					OnUnitIconChanged(itemunit, IRTSUnitInterface::Execute_GetUnitIcon(cdo), nullptr, false);
//				}
//			}
//		}
//	}
//	else
//	{
//		AActor* itemunit = GetFirstUnitForThisItem();
//		if (itemunit != nullptr)
//		{
//			OnUnitDisplayNameChanged(itemunit, IRTSUnitInterface::Execute_GetUnitDisplayName(itemunit), FText::GetEmpty());
//			OnUnitIconChanged(itemunit, IRTSUnitInterface::Execute_GetUnitIcon(itemunit), nullptr, false);
//		}
//	}
//	return true;
//}

//void URTSDKSelectionItemWidget::SubselectionChanged(URTSDKUnitComponent* inSubselection)
//{
//	OnPlayerSubSelectionChanged(Sender, NewPlayerSubSelectionUnit, NewPlayerSubSelectionClass);
//}
//
//bool URTSDKSelectionItemWidget::OnPlayerSubSelectionChanged_Implementation(ARTSPlayerState* Sender, AActor* NewPlayerSubSelectionUnit, TSubclassOf<AActor> NewPlayerSubSelectionClass)
//{
//	if (Sender != nullptr)
//	{
//		if (CommanderToDisplay != nullptr)
//		{
//			if (CommanderToDisplay != Sender)
//			{
//				return false;
//			}
//		}
//		else if (ParentContainer != nullptr)
//		{
//			if (ParentContainer->CommanderToDisplay != nullptr)
//			{
//				if (ParentContainer->CommanderToDisplay != Sender)
//				{
//					return false;
//				}
//			}
//		}
//		else
//		{
//			return false;
//		}
//
//		if (Sender->PlayerInSingleSubSelection)
//		{
//			AActor* itemunit = GetFirstUnitForThisItem();
//			if (itemunit != nullptr)
//			{
//				if (itemunit == NewPlayerSubSelectionUnit)
//				{
//					return true;
//				}
//			}
//		}
//		else
//		{
//			AActor* itemunit = GetFirstUnitForThisItem();
//			if (itemunit != nullptr)
//			{
//				if (IRTSUnitInterface::Execute_GetUnitSelectionClass(itemunit) == NewPlayerSubSelectionClass)
//				{
//					return true;
//				}
//			}
//		}
//	}
//	return false;
//}

void URTSDKSelectionItemWidget::UnitDied(FRTSDKScriptExecutionContext ScriptContext, URTSDKUnitComponent* Sender, const FRTSDKDamageInfo& KillingHitDamageEvent)
{
	OnUnitDied(Sender, KillingHitDamageEvent);
}

void URTSDKSelectionItemWidget::UnitRemoved(FRTSDKScriptExecutionContext ScriptContext, URTSDKUnitComponent* Sender)
{
	OnUnitRemoved(Sender);
}

void URTSDKSelectionItemWidget::UnitDisplayNameChanged(URTSDKUnitComponent* Sender, const FText& NewDisplayName)
{
	if (!CombineSameClassUnits)
	{
		NameToDisplay = NewDisplayName;
		OnDisplayNameChanged();
	}
}

void URTSDKSelectionItemWidget::UnitIconChanged(URTSDKUnitComponent* Sender, UMaterialInterface* NewIcon)
{
	if (!CombineSameClassUnits)
	{
		IconToDisplay = NewIcon;
		OnIconChanged();
	}
}

int32 URTSDKSelectionItemWidget::GetUnitCount()
{
	return URTSDKUIHelpers::GetAliveCountFromUnitCollection(UnitsToDisplay);
}

bool URTSDKSelectionItemWidget::GetIsSubSelected()
{
	return bIsSubselected;
}

AActor* URTSDKSelectionItemWidget::GetFirstUnitForThisItem()
{
	return nullptr;
	//return URTSHelpers::GetFirstValidUnitFromUnitArray(UnitsToDisplay, false, false);
}

TArray<URTSDKUnitComponent*> URTSDKSelectionItemWidget::GetAllSameClassUnitsForThisItem()
{
	/*if (CombineSameClassUnits)
	{
		return UnitsToDisplay.Items;
	}
	else
	{
		AActor* itemunit = GetFirstUnitForThisItem();
		if (itemunit != nullptr)
		{
			if (ParentContainer != nullptr)
			{
				return URTSHelpers::GetUnitsBySelectionClassFromUnitArray(ParentContainer->PlayerSelectionToDisplay, IRTSUnitInterface::Execute_GetUnitSelectionClass(itemunit));
			}
			else if (CommanderToDisplay != nullptr)
			{
				return CommanderToDisplay->GetPlayerSelectionUnitsBySelectionClass(IRTSUnitInterface::Execute_GetUnitSelectionClass(itemunit));
			}
		}
	}*/
	return TArray<URTSDKUnitComponent*>();
}

TArray<URTSDKUnitComponent*> URTSDKSelectionItemWidget::GetAllUnitsForThisItem()
{
	/*if (ParentContainer != nullptr)
	{
		if (ParentContainer->CommanderToDisplay != nullptr)
		{
			if (CombineSameClassUnits)
			{
				if ((SelectionIndex >= 0) && (SelectionIndex < ParentContainer->CommanderToDisplay->GetPlayerSelectionClassesCount()))
				{
					return URTSHelpers::GetUnitsBySelectionClassFromUnitArray(ParentContainer->PlayerSelectionToDisplay, ParentContainer->CommanderToDisplay->GetPlayerSelectionClass(SelectionIndex));
				}
			}
			else
			{
				if ((SelectionIndex >= 0) && (SelectionIndex < ParentContainer->PlayerSelectionToDisplay.Num()))
				{
					TArray<AActor*> retval;
					retval.Add(ParentContainer->PlayerSelectionToDisplay[SelectionIndex]);
					return retval;
				}
			}
		}
	}
	else if (CommanderToDisplay != nullptr)
	{
		if (CombineSameClassUnits)
		{
			if ((SelectionIndex >= 0) && (SelectionIndex < CommanderToDisplay->GetPlayerSelectionClassesCount()))
			{
				return CommanderToDisplay->GetPlayerSelectionUnitsBySelectionClass(CommanderToDisplay->GetPlayerSelectionClass(SelectionIndex));
			}
		}
		else
		{
			if ((SelectionIndex >= 0) && (SelectionIndex < CommanderToDisplay->GetPlayerSelectionCount()))
			{
				TArray<AActor*> retval;
				retval.Add(CommanderToDisplay->GetPlayerSelectionUnit(SelectionIndex));
				return retval;
			}
		}
	}*/
	return TArray<URTSDKUnitComponent*>();
}

bool URTSDKSelectionItemWidget::DoPrimaryCursorAction()
{
	/*ERTSSelectionPanelCursorActions actiontouse = ERTSSelectionPanelCursorActions::DoNothing;
	bool issubselected = GetIsSubSelected();
	APlayerController* owningpc = GetOwningPlayer();
	if (owningpc == nullptr)
	{
		return false;
	}
	if (owningpc->PlayerInput == nullptr)
	{
		return false;
	}
	if (CommanderToDisplay == nullptr)
	{
		return false;
	}

	bool shiftheld = owningpc->PlayerInput->IsShiftPressed();
	bool ctrlheld = owningpc->PlayerInput->IsCtrlPressed();
	bool altheld = owningpc->PlayerInput->IsAltPressed();
	if (shiftheld && ctrlheld && altheld)
	{
		if (issubselected)
		{
			actiontouse = ShiftCtrlAltIsSubSelectedPrimaryCursorAction;
		}
		else
		{
			actiontouse = ShiftCtrlAltNotSubSelectedPrimaryCursorAction;
		}
	}
	else if (shiftheld && ctrlheld)
	{
		if (issubselected)
		{
			actiontouse = ShiftCtrlIsSubSelectedPrimaryCursorAction;
		}
		else
		{
			actiontouse = ShiftCtrlNotSubSelectedPrimaryCursorAction;
		}
	}
	else if (shiftheld && altheld)
	{
		if (issubselected)
		{
			actiontouse = ShiftAltIsSubSelectedPrimaryCursorAction;
		}
		else
		{
			actiontouse = ShiftAltNotSubSelectedPrimaryCursorAction;
		}
	}
	else if (ctrlheld && altheld)
	{
		if (issubselected)
		{
			actiontouse = CtrlAltIsSubSelectedPrimaryCursorAction;
		}
		else
		{
			actiontouse = CtrlAltNotSubSelectedPrimaryCursorAction;
		}
	}
	else if (shiftheld)
	{
		if (issubselected)
		{
			actiontouse = ShiftIsSubSelectedPrimaryCursorAction;
		}
		else
		{
			actiontouse = ShiftNotSubSelectedPrimaryCursorAction;
		}
	}
	else if (ctrlheld)
	{
		if (issubselected)
		{
			actiontouse = CtrlIsSubSelectedPrimaryCursorAction;
		}
		else
		{
			actiontouse = CtrlNotSubSelectedPrimaryCursorAction;
		}
	}
	else if (altheld)
	{
		if (issubselected)
		{
			actiontouse = AltIsSubSelectedPrimaryCursorAction;
		}
		else
		{
			actiontouse = AltNotSubSelectedPrimaryCursorAction;
		}
	}
	else
	{
		if (issubselected)
		{
			actiontouse = NoModifierIsSubSelectedPrimaryCursorAction;
		}
		else
		{
			actiontouse = NoModifierNotSubSelectedPrimaryCursorAction;
		}
	}
	AActor* targetunit = nullptr;
	TArray<AActor*>targetunits;
	switch (actiontouse)
	{
	case ERTSSelectionPanelCursorActions::DoNothing:
		return true;
	case ERTSSelectionPanelCursorActions::SetSubSelection:
		if (!issubselected)
		{
			targetunit = GetFirstUnitForThisItem();
			if (targetunit != nullptr)
			{
				CommanderToDisplay->SetPlayerSubSelection(IRTSUnitInterface::Execute_GetUnitSelectionClass(targetunit), targetunit);
				return true;
			}
		}
		return false;
	case ERTSSelectionPanelCursorActions::RemoveOneUnit:
		targetunit = GetFirstUnitForThisItem();
		targetunits.Empty();
		targetunits.Add(targetunit);
		CommanderToDisplay->RemoveUnitsFromPlayerSelection(targetunits);
		return true;
	case ERTSSelectionPanelCursorActions::RemoveAllUnitsOfClass:
		targetunits = GetAllSameClassUnitsForThisItem();
		CommanderToDisplay->RemoveUnitsFromPlayerSelection(targetunits);
		return true;
	case ERTSSelectionPanelCursorActions::SelectOneUnit:
		targetunit = GetFirstUnitForThisItem();
		targetunits.Empty();
		targetunits.Add(targetunit);
		CommanderToDisplay->SetPlayerSelection(targetunits);
		return true;
	case ERTSSelectionPanelCursorActions::SelectAllUnitsOfClass:
		targetunits = GetAllSameClassUnitsForThisItem();
		CommanderToDisplay->SetPlayerSelection(targetunits);
		return true;
	}*/
	return false;
}

bool URTSDKSelectionItemWidget::DoSecondaryCursorAction()
{
	/*ERTSSelectionPanelCursorActions actiontouse = ERTSSelectionPanelCursorActions::DoNothing;
	bool issubselected = GetIsSubSelected();
	APlayerController* owningpc = GetOwningPlayer();
	if (owningpc == nullptr)
	{
		return false;
	}
	if (owningpc->PlayerInput == nullptr)
	{
		return false;
	}
	if (CommanderToDisplay == nullptr)
	{
		return false;
	}

	bool shiftheld = owningpc->PlayerInput->IsShiftPressed();
	bool ctrlheld = owningpc->PlayerInput->IsCtrlPressed();
	bool altheld = owningpc->PlayerInput->IsAltPressed();
	if (shiftheld && ctrlheld && altheld)
	{
		if (issubselected)
		{
			actiontouse = ShiftCtrlAltIsSubSelectedSecondaryCursorAction;
		}
		else
		{
			actiontouse = ShiftCtrlAltNotSubSelectedSecondaryCursorAction;
		}
	}
	else if (shiftheld && ctrlheld)
	{
		if (issubselected)
		{
			actiontouse = ShiftCtrlIsSubSelectedSecondaryCursorAction;
		}
		else
		{
			actiontouse = ShiftCtrlNotSubSelectedSecondaryCursorAction;
		}
	}
	else if (shiftheld && altheld)
	{
		if (issubselected)
		{
			actiontouse = ShiftAltIsSubSelectedSecondaryCursorAction;
		}
		else
		{
			actiontouse = ShiftAltNotSubSelectedSecondaryCursorAction;
		}
	}
	else if (ctrlheld && altheld)
	{
		if (issubselected)
		{
			actiontouse = CtrlAltIsSubSelectedSecondaryCursorAction;
		}
		else
		{
			actiontouse = CtrlAltNotSubSelectedSecondaryCursorAction;
		}
	}
	else if (shiftheld)
	{
		if (issubselected)
		{
			actiontouse = ShiftIsSubSelectedSecondaryCursorAction;
		}
		else
		{
			actiontouse = ShiftNotSubSelectedSecondaryCursorAction;
		}
	}
	else if (ctrlheld)
	{
		if (issubselected)
		{
			actiontouse = CtrlIsSubSelectedSecondaryCursorAction;
		}
		else
		{
			actiontouse = CtrlNotSubSelectedSecondaryCursorAction;
		}
	}
	else if (altheld)
	{
		if (issubselected)
		{
			actiontouse = AltIsSubSelectedSecondaryCursorAction;
		}
		else
		{
			actiontouse = AltNotSubSelectedSecondaryCursorAction;
		}
	}
	else
	{
		if (issubselected)
		{
			actiontouse = NoModifierIsSubSelectedSecondaryCursorAction;
		}
		else
		{
			actiontouse = NoModifierNotSubSelectedSecondaryCursorAction;
		}
	}
	AActor* targetunit = nullptr;
	TArray<AActor*>targetunits;
	switch (actiontouse)
	{
	case ERTSSelectionPanelCursorActions::DoNothing:
		return true;
	case ERTSSelectionPanelCursorActions::SetSubSelection:
		if (!issubselected)
		{
			targetunit = GetFirstUnitForThisItem();
			if (targetunit != nullptr)
			{
				CommanderToDisplay->SetPlayerSubSelection(IRTSUnitInterface::Execute_GetUnitSelectionClass(targetunit), targetunit);
				return true;
			}
		}
		return false;
	case ERTSSelectionPanelCursorActions::RemoveOneUnit:
		targetunit = GetFirstUnitForThisItem();
		targetunits.Empty();
		targetunits.Add(targetunit);
		CommanderToDisplay->RemoveUnitsFromPlayerSelection(targetunits);
		return true;
	case ERTSSelectionPanelCursorActions::RemoveAllUnitsOfClass:
		targetunits = GetAllSameClassUnitsForThisItem();
		CommanderToDisplay->RemoveUnitsFromPlayerSelection(targetunits);
		return true;
	case ERTSSelectionPanelCursorActions::SelectOneUnit:
		targetunit = GetFirstUnitForThisItem();
		targetunits.Empty();
		targetunits.Add(targetunit);
		CommanderToDisplay->SetPlayerSelection(targetunits);
		return true;
	case ERTSSelectionPanelCursorActions::SelectAllUnitsOfClass:
		targetunits = GetAllSameClassUnitsForThisItem();
		CommanderToDisplay->SetPlayerSelection(targetunits);
		return true;
	}*/
	return false;
}

bool URTSDKSelectionItemWidget::DoTertiaryCursorAction()
{
	/*ERTSSelectionPanelCursorActions actiontouse = ERTSSelectionPanelCursorActions::DoNothing;
	bool issubselected = GetIsSubSelected();
	APlayerController* owningpc = GetOwningPlayer();
	if (owningpc == nullptr)
	{
		return false;
	}
	if (owningpc->PlayerInput == nullptr)
	{
		return false;
	}
	if (CommanderToDisplay == nullptr)
	{
		return false;
	}

	bool shiftheld = owningpc->PlayerInput->IsShiftPressed();
	bool ctrlheld = owningpc->PlayerInput->IsCtrlPressed();
	bool altheld = owningpc->PlayerInput->IsAltPressed();
	if (shiftheld && ctrlheld && altheld)
	{
		if (issubselected)
		{
			actiontouse = ShiftCtrlAltIsSubSelectedTertiaryCursorAction;
		}
		else
		{
			actiontouse = ShiftCtrlAltNotSubSelectedTertiaryCursorAction;
		}
	}
	else if (shiftheld && ctrlheld)
	{
		if (issubselected)
		{
			actiontouse = ShiftCtrlIsSubSelectedTertiaryCursorAction;
		}
		else
		{
			actiontouse = ShiftCtrlNotSubSelectedTertiaryCursorAction;
		}
	}
	else if (shiftheld && altheld)
	{
		if (issubselected)
		{
			actiontouse = ShiftAltIsSubSelectedTertiaryCursorAction;
		}
		else
		{
			actiontouse = ShiftAltNotSubSelectedTertiaryCursorAction;
		}
	}
	else if (ctrlheld && altheld)
	{
		if (issubselected)
		{
			actiontouse = CtrlAltIsSubSelectedTertiaryCursorAction;
		}
		else
		{
			actiontouse = CtrlAltNotSubSelectedTertiaryCursorAction;
		}
	}
	else if (shiftheld)
	{
		if (issubselected)
		{
			actiontouse = ShiftIsSubSelectedTertiaryCursorAction;
		}
		else
		{
			actiontouse = ShiftNotSubSelectedTertiaryCursorAction;
		}
	}
	else if (ctrlheld)
	{
		if (issubselected)
		{
			actiontouse = CtrlIsSubSelectedTertiaryCursorAction;
		}
		else
		{
			actiontouse = CtrlNotSubSelectedTertiaryCursorAction;
		}
	}
	else if (altheld)
	{
		if (issubselected)
		{
			actiontouse = AltIsSubSelectedTertiaryCursorAction;
		}
		else
		{
			actiontouse = AltNotSubSelectedTertiaryCursorAction;
		}
	}
	else
	{
		if (issubselected)
		{
			actiontouse = NoModifierIsSubSelectedTertiaryCursorAction;
		}
		else
		{
			actiontouse = NoModifierNotSubSelectedTertiaryCursorAction;
		}
	}
	AActor* targetunit = nullptr;
	TArray<AActor*>targetunits;
	switch (actiontouse)
	{
	case ERTSSelectionPanelCursorActions::DoNothing:
		return true;
	case ERTSSelectionPanelCursorActions::SetSubSelection:
		if (!issubselected)
		{
			targetunit = GetFirstUnitForThisItem();
			if (targetunit != nullptr)
			{
				CommanderToDisplay->SetPlayerSubSelection(IRTSUnitInterface::Execute_GetUnitSelectionClass(targetunit), targetunit);
				return true;
			}
		}
		return false;
	case ERTSSelectionPanelCursorActions::RemoveOneUnit:
		targetunit = GetFirstUnitForThisItem();
		targetunits.Empty();
		targetunits.Add(targetunit);
		CommanderToDisplay->RemoveUnitsFromPlayerSelection(targetunits);
		return true;
	case ERTSSelectionPanelCursorActions::RemoveAllUnitsOfClass:
		targetunits = GetAllSameClassUnitsForThisItem();
		CommanderToDisplay->RemoveUnitsFromPlayerSelection(targetunits);
		return true;
	case ERTSSelectionPanelCursorActions::SelectOneUnit:
		targetunit = GetFirstUnitForThisItem();
		targetunits.Empty();
		targetunits.Add(targetunit);
		CommanderToDisplay->SetPlayerSelection(targetunits);
		return true;
	case ERTSSelectionPanelCursorActions::SelectAllUnitsOfClass:
		targetunits = GetAllSameClassUnitsForThisItem();
		CommanderToDisplay->SetPlayerSelection(targetunits);
		return true;
	}*/
	return false;
}

void URTSDKSelectionItemWidget::PairWithRTSSelectionContainerWidget(URTSDKSelectionContainerWidget* TargetContainer, int32 NewSelectionIndex)
{
	//SelectionIndex = NewSelectionIndex;
	//if (TargetContainer != nullptr)
	//{
	//	if (CommanderToDisplay != nullptr)//If this was already set, we potentially already bound to it in OnInitialize, unbind now
	//	{
	//		CommanderToDisplay->OnPlayerSelectionChangedDelegate.RemoveAll(this);
	//		CommanderToDisplay->OnPlayerSubSelectionChangedDelegate.RemoveAll(this);
	//	}
	//	CommanderToDisplay = nullptr;
	//	//we don't bind up events from the player state now, the parent container we are pairing with will tell us about this directly.
	//	ParentContainer = TargetContainer;
	//	CombineSameClassUnits = TargetContainer->CombineSameClassUnits;

	//	OnUnitDiedOption = ParentContainer->OnUnitDiedOption;
	//	OnUnitDestroyedOption = ParentContainer->OnUnitDestroyedOption;

	//	NoModifierNotSubSelectedPrimaryCursorAction = TargetContainer->NoModifierNotSubSelectedPrimaryCursorAction;
	//	ShiftNotSubSelectedPrimaryCursorAction = TargetContainer->ShiftNotSubSelectedPrimaryCursorAction;
	//	CtrlNotSubSelectedPrimaryCursorAction = TargetContainer->CtrlNotSubSelectedPrimaryCursorAction;
	//	AltNotSubSelectedPrimaryCursorAction = TargetContainer->AltNotSubSelectedPrimaryCursorAction;
	//	ShiftCtrlNotSubSelectedPrimaryCursorAction = TargetContainer->ShiftCtrlNotSubSelectedPrimaryCursorAction;
	//	ShiftAltNotSubSelectedPrimaryCursorAction = TargetContainer->ShiftAltNotSubSelectedPrimaryCursorAction;
	//	CtrlAltNotSubSelectedPrimaryCursorAction = TargetContainer->CtrlAltNotSubSelectedPrimaryCursorAction;
	//	ShiftCtrlAltNotSubSelectedPrimaryCursorAction = TargetContainer->ShiftCtrlAltNotSubSelectedPrimaryCursorAction;

	//	NoModifierIsSubSelectedPrimaryCursorAction = TargetContainer->NoModifierIsSubSelectedPrimaryCursorAction;
	//	ShiftIsSubSelectedPrimaryCursorAction = TargetContainer->ShiftIsSubSelectedPrimaryCursorAction;
	//	CtrlIsSubSelectedPrimaryCursorAction = TargetContainer->CtrlIsSubSelectedPrimaryCursorAction;
	//	AltIsSubSelectedPrimaryCursorAction = TargetContainer->AltIsSubSelectedPrimaryCursorAction;
	//	ShiftCtrlIsSubSelectedPrimaryCursorAction = TargetContainer->ShiftCtrlIsSubSelectedPrimaryCursorAction;
	//	ShiftAltIsSubSelectedPrimaryCursorAction = TargetContainer->ShiftAltIsSubSelectedPrimaryCursorAction;
	//	CtrlAltIsSubSelectedPrimaryCursorAction = TargetContainer->CtrlAltIsSubSelectedPrimaryCursorAction;
	//	ShiftCtrlAltIsSubSelectedPrimaryCursorAction = TargetContainer->ShiftCtrlAltIsSubSelectedPrimaryCursorAction;

	//	NoModifierNotSubSelectedSecondaryCursorAction = TargetContainer->NoModifierNotSubSelectedSecondaryCursorAction;
	//	ShiftNotSubSelectedSecondaryCursorAction = TargetContainer->ShiftNotSubSelectedSecondaryCursorAction;
	//	CtrlNotSubSelectedSecondaryCursorAction = TargetContainer->CtrlNotSubSelectedSecondaryCursorAction;
	//	AltNotSubSelectedSecondaryCursorAction = TargetContainer->AltNotSubSelectedSecondaryCursorAction;
	//	ShiftCtrlNotSubSelectedSecondaryCursorAction = TargetContainer->ShiftCtrlNotSubSelectedSecondaryCursorAction;
	//	ShiftAltNotSubSelectedSecondaryCursorAction = TargetContainer->ShiftAltNotSubSelectedSecondaryCursorAction;
	//	CtrlAltNotSubSelectedSecondaryCursorAction = TargetContainer->CtrlAltNotSubSelectedSecondaryCursorAction;
	//	ShiftCtrlAltNotSubSelectedSecondaryCursorAction = TargetContainer->ShiftCtrlAltNotSubSelectedSecondaryCursorAction;

	//	NoModifierIsSubSelectedSecondaryCursorAction = TargetContainer->NoModifierIsSubSelectedSecondaryCursorAction;
	//	ShiftIsSubSelectedSecondaryCursorAction = TargetContainer->ShiftIsSubSelectedSecondaryCursorAction;
	//	CtrlIsSubSelectedSecondaryCursorAction = TargetContainer->CtrlIsSubSelectedSecondaryCursorAction;
	//	AltIsSubSelectedSecondaryCursorAction = TargetContainer->AltIsSubSelectedSecondaryCursorAction;
	//	ShiftCtrlIsSubSelectedSecondaryCursorAction = TargetContainer->ShiftCtrlIsSubSelectedSecondaryCursorAction;
	//	ShiftAltIsSubSelectedSecondaryCursorAction = TargetContainer->ShiftAltIsSubSelectedSecondaryCursorAction;
	//	CtrlAltIsSubSelectedSecondaryCursorAction = TargetContainer->CtrlAltIsSubSelectedSecondaryCursorAction;
	//	ShiftCtrlAltIsSubSelectedSecondaryCursorAction = TargetContainer->ShiftCtrlAltIsSubSelectedSecondaryCursorAction;

	//	NoModifierNotSubSelectedTertiaryCursorAction = TargetContainer->NoModifierNotSubSelectedTertiaryCursorAction;
	//	ShiftNotSubSelectedTertiaryCursorAction = TargetContainer->ShiftNotSubSelectedTertiaryCursorAction;
	//	CtrlNotSubSelectedTertiaryCursorAction = TargetContainer->CtrlNotSubSelectedTertiaryCursorAction;
	//	AltNotSubSelectedTertiaryCursorAction = TargetContainer->AltNotSubSelectedTertiaryCursorAction;
	//	ShiftCtrlNotSubSelectedTertiaryCursorAction = TargetContainer->ShiftCtrlNotSubSelectedTertiaryCursorAction;
	//	ShiftAltNotSubSelectedTertiaryCursorAction = TargetContainer->ShiftAltNotSubSelectedTertiaryCursorAction;
	//	CtrlAltNotSubSelectedTertiaryCursorAction = TargetContainer->CtrlAltNotSubSelectedTertiaryCursorAction;
	//	ShiftCtrlAltNotSubSelectedTertiaryCursorAction = TargetContainer->ShiftCtrlAltNotSubSelectedTertiaryCursorAction;

	//	NoModifierIsSubSelectedTertiaryCursorAction = TargetContainer->NoModifierIsSubSelectedTertiaryCursorAction;
	//	ShiftIsSubSelectedTertiaryCursorAction = TargetContainer->ShiftIsSubSelectedTertiaryCursorAction;
	//	CtrlIsSubSelectedTertiaryCursorAction = TargetContainer->CtrlIsSubSelectedTertiaryCursorAction;
	//	AltIsSubSelectedTertiaryCursorAction = TargetContainer->AltIsSubSelectedTertiaryCursorAction;
	//	ShiftCtrlIsSubSelectedTertiaryCursorAction = TargetContainer->ShiftCtrlIsSubSelectedTertiaryCursorAction;
	//	ShiftAltIsSubSelectedTertiaryCursorAction = TargetContainer->ShiftAltIsSubSelectedTertiaryCursorAction;
	//	CtrlAltIsSubSelectedTertiaryCursorAction = TargetContainer->CtrlAltIsSubSelectedTertiaryCursorAction;
	//	ShiftCtrlAltIsSubSelectedTertiaryCursorAction = TargetContainer->ShiftCtrlAltIsSubSelectedTertiaryCursorAction;

	//	if (ParentContainer->CommanderToDisplay != nullptr)
	//	{
	//		OnPlayerSelectionChanged(ParentContainer->CommanderToDisplay, ParentContainer->PlayerSelectionToDisplay);
	//		OnPlayerSubSelectionChanged(ParentContainer->CommanderToDisplay, ParentContainer->CommanderToDisplay->PlayerSubSelectionUnit, ParentContainer->CommanderToDisplay->PlayerSubSelectionClass);
	//	}
	//	else
	//	{
	//		OnPlayerSelectionChanged(nullptr, TArray<AActor*>());
	//		OnPlayerSubSelectionChanged(nullptr, nullptr, nullptr);
	//	}
	//}
}
