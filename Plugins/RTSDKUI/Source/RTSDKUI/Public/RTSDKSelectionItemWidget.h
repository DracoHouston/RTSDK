// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RTSDKUITypes.h"
#include "Blueprint/UserWidget.h"
#include "RTSDKSelectionItemWidget.generated.h"

class URTSDKSelectionContainerWidget;
class ARTSDKCommanderStateBase;
class URTSDKUnitComponent;

/**
 * 
 */
UCLASS(Abstract, editinlinenew, BlueprintType, Blueprintable, meta = (DontUseGenericSpawnObject = "True", DisableNativeTick))
class RTSDKUI_API URTSDKSelectionItemWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	//virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;
public:

	/**
	* The RTSSelectionContainerWidget this item widget is paired with. Set during spawn to have the pairing set up for you.
	* Call PairWithRTSSelectionContainerWidget to set it up after spawn manually, simply setting this property will not do it.
	*/
	UPROPERTY(BlueprintReadWrite, Category = "RTS Selection Item Widget", Meta = (ExposeOnSpawn = "true"))
		TObjectPtr<URTSDKSelectionContainerWidget> ParentContainer;
	
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Transient, Category = "RTS Selection Item Widget|Transient")
		FRTSDKUIUnitsCollection UnitsToDisplay;

	UFUNCTION()
		void SetUnitsToDisplay(const FRTSDKUIUnitsCollection& inUnits);

	UFUNCTION(BlueprintImplementableEvent)
		void OnSetUnitsToDisplay();

	UFUNCTION()
		void SetIsSubselected(bool inIsSubselected);

	UFUNCTION(BlueprintImplementableEvent)
		void OnSetIsSubselected();

	/**
	* If false every item represents 1 unit within selection
	* If true every item represents 1 selection class within selection
	* For example you might have a dozen 'spearmen' and 'swordsmen' in selection,
	* there would be 2 items representing 12 spearmen and 12 swordsmen, not 24
	* items sorted as 12 spearmen then 12 swordsmen.
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS Selection Item Widget")
	bool CombineSameClassUnits;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS Selection Item Widget")
		TObjectPtr<UMaterialInterface> IconToDisplay;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS Selection Item Widget")
		FText NameToDisplay;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS Selection Item Widget")
		bool bIsSubselected;

	/**
	* Native event function that is bound to OnUnitDied events for UnitsToDisplay
	* Calls the blueprint implementable OnUnitDied.
	*/
	UFUNCTION(BlueprintCallable, Category = "RTS Selection Item Widget|Events")
		void UnitDied(FRTSDKScriptExecutionContext ScriptContext, URTSDKUnitComponent* Sender, const FRTSDKDamageInfo& KillingHitDamageEvent);

	/**
	* Event for when one of the UnitsToDisplay dies, allowing the item to react
	* it is intended for UMG designed widget blueprints to override and perform anything
	* that has to happen on child widgets at runtime to update display.
	*/
	UFUNCTION(BlueprintImplementableEvent, Category = "RTS Selection Item Widget|Events")
		void OnUnitDied(URTSDKUnitComponent* Sender, const FRTSDKDamageInfo& KillingHitDamageEvent);

	/**
	* Native event function that is bound to actor OnDestroyed event for UnitsToDisplay
	* Calls the blueprint implementable OnUnitDestroyed. Call it directly instead.
	*/
	UFUNCTION()
		void UnitRemoved(FRTSDKScriptExecutionContext ScriptContext, URTSDKUnitComponent* Sender);

	/**
	* Event for when one of the UnitsToDisplay dies, allowing the item to react
	* it is intended for UMG designed widget blueprints to override and perform anything
	* that has to happen on child widgets at runtime to update display.
	*/
	UFUNCTION(BlueprintImplementableEvent, Category = "RTS Selection Item Widget|Events")
		void OnUnitRemoved(URTSDKUnitComponent* Sender);

	/**
	* Native event function that is bound to unit interface OnUnitDisplayNameChange event for UnitsToDisplay
	* Calls the blueprint implementable OnUnitDisplayNameChange. Call it directly instead.
	*/
	UFUNCTION()
		void UnitDisplayNameChanged(URTSDKUnitComponent* Sender, const FText& NewDisplayName);

	/**
	* Event for when one of the UnitsToDisplay dies, allowing the item to react
	* Native implementation does nothing.
	* it is intended for UMG designed widget blueprints to override and perform anything
	* that has to happen on child widgets at runtime to update display.
	*/
	UFUNCTION(BlueprintImplementableEvent, Category = "RTS Selection Item Widget|Events")
		void OnDisplayNameChanged();

	/**
	* Native event function that is bound to unit interface OnUnitIconChange event for UnitsToDisplay
	* Calls the blueprint implementable OnUnitIconChange. Call it directly instead.
	*/
	UFUNCTION()
		void UnitIconChanged(URTSDKUnitComponent* Sender, UMaterialInterface* NewIcon);

	/**
	* Event for when one of the UnitsToDisplay changes icon, allowing the item to react
	* Native implementation does nothing.
	* it is intended for UMG designed widget blueprints to override and perform anything
	* that has to happen on child widgets at runtime to update display.
	*/
	UFUNCTION(BlueprintImplementableEvent, Category = "RTS Selection Item Widget|Events")
		void OnIconChanged();

	/**
	* Gets the unit count within selection of the selection class being displayed, if combining same class units.
	* While not particularly useful for individual unit items, if called in that case
	* it will return the unit count within selection for the class of the displayed unit.
	*/
	UFUNCTION(BlueprintPure, Category = "RTS Selection Item Widget|Helpers")
	int32 GetUnitCount();

	/**
	* For individual unit items will return true under the following circumstances:
	* Player is not in single sub selection, and the sub selected selection class is the same
	* as the selection class of the unit being displayed by this item
	* Player is in single sub selection, and the above is true and the unit being displayed is also the
	* current sub selected unit
	* For combined classes it will return true if the sub selected selection class is the same
	* as the selection class being displayed, regardless of single sub selection being on or off.
	*/
	UFUNCTION(BlueprintPure, Category = "RTS Selection Item Widget|Helpers")
	bool GetIsSubSelected();

	/**
	* Either gets the unit for this item, or the first unit from a selection class,
	* depending on if you don't combine same class units, or do.
	*/
	UFUNCTION(BlueprintPure, Category = "RTS Selection Item Widget|Helpers")
	AActor* GetFirstUnitForThisItem();

	/**
	* Gets all units of the same selection class as this item, all units of the displayed class
	* in the case of combining same class units. Units of the same class as the unit being displayed otherwise.
	*/
	UFUNCTION(BlueprintPure, Category = "RTS Selection Item Widget|Helpers")
	TArray<URTSDKUnitComponent*> GetAllSameClassUnitsForThisItem();

	/**
	* Gets all units for this item, depending on if we combine same class units.
	* In the case of combining same class units. Just the unit being displayed otherwise.
	*/
	UFUNCTION(BlueprintPure, Category = "RTS Selection Item Widget|Helpers")
		TArray<URTSDKUnitComponent*> GetAllUnitsForThisItem();

	/**
	* Will do the primary action for this item (left clicked) depending on sub selection state and modifiers held.
	*/
	UFUNCTION(BlueprintCallable, Category = "RTS Selection Item Widget|Actions")
		bool DoPrimaryCursorAction();

	/**
	* Will do the secondary action for this item (right clicked) depending on sub selection state and modifiers held.
	*/
	UFUNCTION(BlueprintCallable, Category = "RTS Selection Item Widget|Actions")
		bool DoSecondaryCursorAction();

	/**
	* Will do the tertiary action for this item (middle clicked) depending on sub selection state and modifiers held.
	*/
	UFUNCTION(BlueprintCallable, Category = "RTS Selection Item Widget|Actions")
		bool DoTertiaryCursorAction();

	/**
	* Sets the parent selection container, we will display one item of selection for them.
	* Initializes the widget, it will track what it needs to display from then on,
	* going off the specified selection container's cache of selection.
	* Mutually exclusive with SetPlayerToDisplay, which connects us directly to a player.
	* Calling this will unset CommanderToDisplay and unbind from selection events.
	*/
	UFUNCTION(BlueprintCallable, Category = "RTS Selection Item Widget|Actions")
		void PairWithRTSSelectionContainerWidget(URTSDKSelectionContainerWidget* TargetContainer, int32 NewSelectionIndex);

	/**
	*
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS Selection Item Widget")
		ERTSDKSelectionWidgetUnitLostOption OnUnitDiedOption;

	/**
	*
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS Selection Item Widget")
		ERTSDKSelectionWidgetUnitLostOption OnUnitDestroyedOption;

	/**
	* On left clicking this item, while not sub selected, with no modifers held, we do this to the target
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS Selection Item Widget|Primary Cursor Action|Not Sub Selected")
		ERTSSelectionPanelCursorActions NoModifierNotSubSelectedPrimaryCursorAction;

	/**
	* On left clicking this item, while not sub selected, with shift held, we do this to the target
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS Selection Item Widget|Primary Cursor Action|Not Sub Selected")
		ERTSSelectionPanelCursorActions ShiftNotSubSelectedPrimaryCursorAction;

	/**
	* On left clicking this item, while not sub selected, with ctrl held, we do this to the target
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS Selection Item Widget|Primary Cursor Action|Not Sub Selected")
		ERTSSelectionPanelCursorActions CtrlNotSubSelectedPrimaryCursorAction;

	/**
	* On left clicking this item, while not sub selected, with alt held, we do this to the target
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS Selection Item Widget|Primary Cursor Action|Not Sub Selected")
		ERTSSelectionPanelCursorActions AltNotSubSelectedPrimaryCursorAction;

	/**
	* On left clicking this item, while not sub selected, with shift and ctrl held, we do this to the target
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS Selection Item Widget|Primary Cursor Action|Not Sub Selected")
		ERTSSelectionPanelCursorActions ShiftCtrlNotSubSelectedPrimaryCursorAction;

	/**
	* On left clicking this item, while not sub selected, with shift and alt held, we do this to the target
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS Selection Item Widget|Primary Cursor Action|Not Sub Selected")
		ERTSSelectionPanelCursorActions ShiftAltNotSubSelectedPrimaryCursorAction;

	/**
	* On left clicking this item, while not sub selected, with ctrl and alt held, we do this to the target
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS Selection Item Widget|Primary Cursor Action|Not Sub Selected")
		ERTSSelectionPanelCursorActions CtrlAltNotSubSelectedPrimaryCursorAction;

	/**
	* On left clicking this item, while not sub selected, with shift, ctrl and alt held, we do this to the target
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS Selection Item Widget|Primary Cursor Action|Not Sub Selected")
		ERTSSelectionPanelCursorActions ShiftCtrlAltNotSubSelectedPrimaryCursorAction;


	/**
	* On left clicking this item, while sub selected, with no modifers held, we do this to the target
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS Selection Item Widget|Primary Cursor Action|Is Sub Selected")
		ERTSSelectionPanelCursorActions NoModifierIsSubSelectedPrimaryCursorAction;

	/**
	* On left clicking this item, while sub selected, with shift held, we do this to the target
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS Selection Item Widget|Primary Cursor Action|Is Sub Selected")
		ERTSSelectionPanelCursorActions ShiftIsSubSelectedPrimaryCursorAction;

	/**
	* On left clicking this item, while sub selected, with ctrl held, we do this to the target
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS Selection Item Widget|Primary Cursor Action|Is Sub Selected")
		ERTSSelectionPanelCursorActions CtrlIsSubSelectedPrimaryCursorAction;

	/**
	* On left clicking this item, while sub selected, with alt held, we do this to the target
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS Selection Item Widget|Primary Cursor Action|Is Sub Selected")
		ERTSSelectionPanelCursorActions AltIsSubSelectedPrimaryCursorAction;

	/**
	* On left clicking this item, while sub selected, with shift and ctrl held, we do this to the target
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS Selection Item Widget|Primary Cursor Action|Is Sub Selected")
		ERTSSelectionPanelCursorActions ShiftCtrlIsSubSelectedPrimaryCursorAction;

	/**
	* On left clicking this item, while sub selected, with shift and alt held, we do this to the target
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS Selection Item Widget|Primary Cursor Action|Is Sub Selected")
		ERTSSelectionPanelCursorActions ShiftAltIsSubSelectedPrimaryCursorAction;

	/**
	* On left clicking this item, while sub selected, with ctrl and alt held, we do this to the target
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS Selection Item Widget|Primary Cursor Action|Is Sub Selected")
		ERTSSelectionPanelCursorActions CtrlAltIsSubSelectedPrimaryCursorAction;

	/**
	* On left clicking this item, while sub selected, with shift, ctrl and alt held, we do this to the target
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS Selection Item Widget|Primary Cursor Action|Is Sub Selected")
		ERTSSelectionPanelCursorActions ShiftCtrlAltIsSubSelectedPrimaryCursorAction;


	/**
	* On right clicking this item, while not sub selected, with no modifers held, we do this to the target
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS Selection Item Widget|Secondary Cursor Action|Not Sub Selected")
		ERTSSelectionPanelCursorActions NoModifierNotSubSelectedSecondaryCursorAction;

	/**
	* On right clicking this item, while not sub selected, with shift held, we do this to the target
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS Selection Item Widget|Secondary Cursor Action|Not Sub Selected")
		ERTSSelectionPanelCursorActions ShiftNotSubSelectedSecondaryCursorAction;

	/**
	* On right clicking this item, while not sub selected, with ctrl held, we do this to the target
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS Selection Item Widget|Secondary Cursor Action|Not Sub Selected")
		ERTSSelectionPanelCursorActions CtrlNotSubSelectedSecondaryCursorAction;

	/**
	* On right clicking this item, while not sub selected, with alt held, we do this to the target
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS Selection Item Widget|Secondary Cursor Action|Not Sub Selected")
		ERTSSelectionPanelCursorActions AltNotSubSelectedSecondaryCursorAction;

	/**
	* On right clicking this item, while not sub selected, with shift and ctrl held, we do this to the target
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS Selection Item Widget|Secondary Cursor Action|Not Sub Selected")
		ERTSSelectionPanelCursorActions ShiftCtrlNotSubSelectedSecondaryCursorAction;

	/**
	* On right clicking this item, while not sub selected, with shift and alt held, we do this to the target
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS Selection Item Widget|Secondary Cursor Action|Not Sub Selected")
		ERTSSelectionPanelCursorActions ShiftAltNotSubSelectedSecondaryCursorAction;

	/**
	* On right clicking this item, while not sub selected, with ctrl and alt held, we do this to the target
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS Selection Item Widget|Secondary Cursor Action|Not Sub Selected")
		ERTSSelectionPanelCursorActions CtrlAltNotSubSelectedSecondaryCursorAction;

	/**
	* On right clicking this item, while not sub selected, with shift, ctrl and alt held, we do this to the target
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS Selection Item Widget|Secondary Cursor Action|Not Sub Selected")
		ERTSSelectionPanelCursorActions ShiftCtrlAltNotSubSelectedSecondaryCursorAction;


	/**
	* On right clicking this item, while sub selected, with no modifers held, we do this to the target
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS Selection Item Widget|Secondary Cursor Action|Is Sub Selected")
		ERTSSelectionPanelCursorActions NoModifierIsSubSelectedSecondaryCursorAction;

	/**
	* On right clicking this item, while sub selected, with shift held, we do this to the target
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS Selection Item Widget|Secondary Cursor Action|Is Sub Selected")
		ERTSSelectionPanelCursorActions ShiftIsSubSelectedSecondaryCursorAction;

	/**
	* On right clicking this item, while sub selected, with ctrl held, we do this to the target
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS Selection Item Widget|Secondary Cursor Action|Is Sub Selected")
		ERTSSelectionPanelCursorActions CtrlIsSubSelectedSecondaryCursorAction;

	/**
	* On right clicking this item, while sub selected, with alt held, we do this to the target
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS Selection Item Widget|Secondary Cursor Action|Is Sub Selected")
		ERTSSelectionPanelCursorActions AltIsSubSelectedSecondaryCursorAction;

	/**
	* On right clicking this item, while sub selected, with shift and ctrl held, we do this to the target
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS Selection Item Widget|Secondary Cursor Action|Is Sub Selected")
		ERTSSelectionPanelCursorActions ShiftCtrlIsSubSelectedSecondaryCursorAction;

	/**
	* On right clicking this item, while sub selected, with shift and alt held, we do this to the target
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS Selection Item Widget|Secondary Cursor Action|Is Sub Selected")
		ERTSSelectionPanelCursorActions ShiftAltIsSubSelectedSecondaryCursorAction;

	/**
	* On right clicking this item, while sub selected, with ctrl and alt held, we do this to the target
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS Selection Item Widget|Secondary Cursor Action|Is Sub Selected")
		ERTSSelectionPanelCursorActions CtrlAltIsSubSelectedSecondaryCursorAction;

	/**
	* On right clicking this item, while sub selected, with shift, ctrl and alt held, we do this to the target
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS Selection Item Widget|Secondary Cursor Action|Is Sub Selected")
		ERTSSelectionPanelCursorActions ShiftCtrlAltIsSubSelectedSecondaryCursorAction;


	/**
	* On middle clicking this item, while not sub selected, with no modifers held, we do this to the target
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS Selection Item Widget|Tertiary Cursor Action|Not Sub Selected")
		ERTSSelectionPanelCursorActions NoModifierNotSubSelectedTertiaryCursorAction;

	/**
	* On middle clicking this item, while not sub selected, with shift held, we do this to the target
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS Selection Item Widget|Tertiary Cursor Action|Not Sub Selected")
		ERTSSelectionPanelCursorActions ShiftNotSubSelectedTertiaryCursorAction;

	/**
	* On middle clicking this item, while not sub selected, with ctrl held, we do this to the target
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS Selection Item Widget|Tertiary Cursor Action|Not Sub Selected")
		ERTSSelectionPanelCursorActions CtrlNotSubSelectedTertiaryCursorAction;

	/**
	* On middle clicking this item, while not sub selected, with alt held, we do this to the target
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS Selection Item Widget|Tertiary Cursor Action|Not Sub Selected")
		ERTSSelectionPanelCursorActions AltNotSubSelectedTertiaryCursorAction;

	/**
	* On middle clicking this item, while not sub selected, with shift and ctrl held, we do this to the target
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS Selection Item Widget|Tertiary Cursor Action|Not Sub Selected")
		ERTSSelectionPanelCursorActions ShiftCtrlNotSubSelectedTertiaryCursorAction;

	/**
	* On middle clicking this item, while not sub selected, with shift and alt held, we do this to the target
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS Selection Item Widget|Tertiary Cursor Action|Not Sub Selected")
		ERTSSelectionPanelCursorActions ShiftAltNotSubSelectedTertiaryCursorAction;

	/**
	* On middle clicking this item, while not sub selected, with ctrl and alt held, we do this to the target
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS Selection Item Widget|Tertiary Cursor Action|Not Sub Selected")
		ERTSSelectionPanelCursorActions CtrlAltNotSubSelectedTertiaryCursorAction;

	/**
	* On middle clicking this item, while not sub selected, with shift, ctrl and alt held, we do this to the target
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS Selection Item Widget|Tertiary Cursor Action|Not Sub Selected")
		ERTSSelectionPanelCursorActions ShiftCtrlAltNotSubSelectedTertiaryCursorAction;


	/**
	* On middle clicking this item, while sub selected, with no modifers held, we do this to the target
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS Selection Item Widget|Tertiary Cursor Action|Is Sub Selected")
		ERTSSelectionPanelCursorActions NoModifierIsSubSelectedTertiaryCursorAction;

	/**
	* On middle clicking this item, while sub selected, with shift held, we do this to the target
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS Selection Item Widget|Tertiary Cursor Action|Is Sub Selected")
		ERTSSelectionPanelCursorActions ShiftIsSubSelectedTertiaryCursorAction;

	/**
	* On middle clicking this item, while sub selected, with ctrl held, we do this to the target
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS Selection Item Widget|Tertiary Cursor Action|Is Sub Selected")
		ERTSSelectionPanelCursorActions CtrlIsSubSelectedTertiaryCursorAction;

	/**
	* On middle clicking this item, while sub selected, with alt held, we do this to the target
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS Selection Item Widget|Tertiary Cursor Action|Is Sub Selected")
		ERTSSelectionPanelCursorActions AltIsSubSelectedTertiaryCursorAction;

	/**
	* On middle clicking this item, while sub selected, with shift and ctrl held, we do this to the target
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS Selection Item Widget|Tertiary Cursor Action|Is Sub Selected")
		ERTSSelectionPanelCursorActions ShiftCtrlIsSubSelectedTertiaryCursorAction;

	/**
	* On middle clicking this item, while sub selected, with shift and alt held, we do this to the target
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS Selection Item Widget|Tertiary Cursor Action|Is Sub Selected")
		ERTSSelectionPanelCursorActions ShiftAltIsSubSelectedTertiaryCursorAction;

	/**
	* On middle clicking this item, while sub selected, with ctrl and alt held, we do this to the target
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS Selection Item Widget|Tertiary Cursor Action|Is Sub Selected")
		ERTSSelectionPanelCursorActions CtrlAltIsSubSelectedTertiaryCursorAction;

	/**
	* On middle clicking this item, while sub selected, with shift, ctrl and alt held, we do this to the target
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS Selection Item Widget|Tertiary Cursor Action|Is Sub Selected")
		ERTSSelectionPanelCursorActions ShiftCtrlAltIsSubSelectedTertiaryCursorAction;

};
