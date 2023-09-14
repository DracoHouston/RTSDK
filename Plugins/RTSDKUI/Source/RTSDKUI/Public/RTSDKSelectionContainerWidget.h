// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RTSDKUITypes.h"
#include "RTSDKTypes.h"
#include "Blueprint/UserWidget.h"
#include "RTSDKCommanderState.h"
#include "RTSDKSelectionContainerWidget.generated.h"

class ARTSDKCommanderStateBase;
class URTSDKSelectionItemWidget;
struct FRTSDKScriptExecutionContext;
struct FRTSDKCommanderSelectionInfo;
struct FRTSDKCommanderSelectionUnit;

/**
 * 
 */
UCLASS(Abstract, editinlinenew, BlueprintType, Blueprintable, meta = (DontUseGenericSpawnObject = "True", DisableNativeTick))
class RTSDKUI_API URTSDKSelectionContainerWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	//virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;
public:
	/**
	* The player state this container is displaying selection for, does not need to be the local player, eg for observers.
	* If set on spawn, before the widget calls OnInitialize, such as on blueprint create widget nodes,
	* the widget will automatically pair up with the desired player state, and bind to its events.
	* If not set it will do this for the owning player of the widget, which is the local player. It will also do this if CommanderToDisplay is invalid on spawn.
	* If you need to change this after spawn, call SetPlayerToDisplay to unbind from a previous valid CommanderToDisplay and bind to a new one.
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Transient, Category = "RTS Selection Container Widget|Transient", Meta = (ExposeOnSpawn = "true"))
		TObjectPtr<ARTSDKCommanderStateBase> CommanderToDisplay;

	UFUNCTION(BlueprintCallable, Category = "RTS Selection Container Widget|Events")
		void SetCommanderToDisplay(ARTSDKCommanderStateBase* inCommander);

	UFUNCTION(BlueprintCallable, Category = "RTS Selection Container Widget|Events")
		void SetCommanderToDisplayToLocalPlayer();

	UFUNCTION(BlueprintImplementableEvent, Category = "RTS Selection Item Widget|Events")
		bool OnRequireMoreWidgets(int32 inNum);

	UFUNCTION(BlueprintImplementableEvent, Category = "RTS Selection Item Widget|Events")
		bool OnRequireLessWidgets(int32 inNum);

	/**
	* Native event function that is bound to rts player state's selection changed event
	* Calls the blueprint implementable OnPlayerSelectionChanged. Call it directly instead.
	*/
	UFUNCTION()
		void SelectionChanged(ARTSDKCommanderStateBase* Sender, const FRTSDKCommanderSelectionInfo& NewSelection);

	/**
	* Event for when the player being displayed changes selection, allowing the container to react
	* Native implementation sets up the PlayerSelectionToDisplay, it then calls the event hander on
	* each item in SelectionItemWidgets, to trigger anything those need to do on selection change
	* Call the parent function at the end of any overriding function to have the events triggered 
	* and the internal selection array set up for you in native code. Calling it before creating 
	* widgets and adding them to SelectionItemWidgets will mean they are not included when 
	* the native implementation is called!
	*/
	UFUNCTION(BlueprintImplementableEvent, Category = "RTS Selection Item Widget|Events")
		bool OnSelectionChanged(ARTSDKCommanderStateBase* Sender, const FRTSDKCommanderSelectionInfo& NewSelection);

	/**
	* Native event function that is bound to rts player state's sub selection changed event
	* Calls the blueprint implementable OnPlayerSubSelectionChanged. Call it directly instead.
	*/
	UFUNCTION()
		void SubSelectionChanged(ARTSDKCommanderStateBase* Sender, const FRTSDKCommanderSelectionUnit& NewSubSelection);

	/**
	* Event for when the player being displayed changes sub selection, allowing the container to react
	* Native implementation passes this event on to each item in SelectionItemWidgets.
	* it is intended for UMG designed widget blueprints to override and perform anything
	* that has to happen on child widgets at runtime to update display.
	* Call the parent function to have the events triggered for you in native code.
	*/
	UFUNCTION(BlueprintImplementableEvent, Category = "RTS Selection Item Widget|Events")
		void OnSubSelectionChanged(ARTSDKCommanderStateBase* Sender, const FRTSDKCommanderSelectionUnit& NewSubSelection);

	/**
	* An array for storing references to instances of the selection item widgets this container is managing.
	* Adding an item here will have it be part of any future communication between item and container
	*/
	UPROPERTY(BlueprintReadWrite, Transient, Category = "RTS Selection Container Widget|Transient")
		TArray<TObjectPtr<URTSDKSelectionItemWidget>> SelectionItemWidgets;

	/**
	* Local copy of the player selection, which items managed by this container will use to draw display information from
	* Because we need to support the 'blizzcraft' style of selection panel where dead units leave gaps in current selection
	* the player state does not purge invalid and dead units. However, this is not always desirable, so to support other behaviour
	* we must work off a copy within the widget. This way we can purge invalid and dead units as they happen if the designer wishes.
	*/
	UPROPERTY(BlueprintReadWrite, Transient, Category = "RTS Selection Container Widget|Transient")
		FRTSDKCommanderSelectionInfo SelectionToDisplay;

	UPROPERTY(BlueprintReadWrite, Transient, Category = "RTS Selection Container Widget|Transient")
		FRTSDKCommanderSelectionUnit SubSelectionToDisplay;

	UPROPERTY(BlueprintReadWrite, Transient, Category = "RTS Selection Container Widget|Transient")
		TArray<FRTSDKCommanderSelectionUnit> SortedSelectionUnits;

	/**
	*
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS Selection Container Widget")
		bool CombineSameClassUnits;

	/**
	*
	*/
	UFUNCTION(BlueprintPure, Category = "RTS Selection Container Widget|Helpers")
	int32 GetSelectionCount();

	/**
	*
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS Selection Container Widget")
		ERTSDKSelectionWidgetUnitLostOption OnUnitDiedOption;

	/**
	*
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS Selection Container Widget")
		ERTSDKSelectionWidgetUnitLostOption OnUnitDestroyedOption;
	
	/**
	* On left clicking on of our items, while not sub selected, with no modifers held, we do this to the target
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS Selection Container Widget|Primary Cursor Action|Not Sub Selected")
		ERTSSelectionPanelCursorActions NoModifierNotSubSelectedPrimaryCursorAction;

	/**
	* On left clicking on of our items, while not sub selected, with shift held, we do this to the target
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS Selection Container Widget|Primary Cursor Action|Not Sub Selected")
		ERTSSelectionPanelCursorActions ShiftNotSubSelectedPrimaryCursorAction;

	/**
	* On left clicking on of our items, while not sub selected, with ctrl held, we do this to the target
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS Selection Container Widget|Primary Cursor Action|Not Sub Selected")
		ERTSSelectionPanelCursorActions CtrlNotSubSelectedPrimaryCursorAction;

	/**
	* On left clicking on of our items, while not sub selected, with alt held, we do this to the target
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS Selection Container Widget|Primary Cursor Action|Not Sub Selected")
		ERTSSelectionPanelCursorActions AltNotSubSelectedPrimaryCursorAction;

	/**
	* On left clicking on of our items, while not sub selected, with shift and ctrl held, we do this to the target
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS Selection Container Widget|Primary Cursor Action|Not Sub Selected")
		ERTSSelectionPanelCursorActions ShiftCtrlNotSubSelectedPrimaryCursorAction;

	/**
	* On left clicking on of our items, while not sub selected, with shift and alt held, we do this to the target
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS Selection Container Widget|Primary Cursor Action|Not Sub Selected")
		ERTSSelectionPanelCursorActions ShiftAltNotSubSelectedPrimaryCursorAction;

	/**
	* On left clicking on of our items, while not sub selected, with ctrl and alt held, we do this to the target
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS Selection Container Widget|Primary Cursor Action|Not Sub Selected")
		ERTSSelectionPanelCursorActions CtrlAltNotSubSelectedPrimaryCursorAction;

	/**
	* On left clicking on of our items, while not sub selected, with shift, ctrl and alt held, we do this to the target
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS Selection Container Widget|Primary Cursor Action|Not Sub Selected")
		ERTSSelectionPanelCursorActions ShiftCtrlAltNotSubSelectedPrimaryCursorAction;


	/**
	* On left clicking on of our items, while sub selected, with no modifers held, we do this to the target
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS Selection Container Widget|Primary Cursor Action|Is Sub Selected")
		ERTSSelectionPanelCursorActions NoModifierIsSubSelectedPrimaryCursorAction;

	/**
	* On left clicking on of our items, while sub selected, with shift held, we do this to the target
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS Selection Container Widget|Primary Cursor Action|Is Sub Selected")
		ERTSSelectionPanelCursorActions ShiftIsSubSelectedPrimaryCursorAction;

	/**
	* On left clicking on of our items, while sub selected, with ctrl held, we do this to the target
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS Selection Container Widget|Primary Cursor Action|Is Sub Selected")
		ERTSSelectionPanelCursorActions CtrlIsSubSelectedPrimaryCursorAction;

	/**
	* On left clicking on of our items, while sub selected, with alt held, we do this to the target
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS Selection Container Widget|Primary Cursor Action|Is Sub Selected")
		ERTSSelectionPanelCursorActions AltIsSubSelectedPrimaryCursorAction;

	/**
	* On left clicking on of our items, while sub selected, with shift and ctrl held, we do this to the target
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS Selection Container Widget|Primary Cursor Action|Is Sub Selected")
		ERTSSelectionPanelCursorActions ShiftCtrlIsSubSelectedPrimaryCursorAction;

	/**
	* On left clicking on of our items, while sub selected, with shift and alt held, we do this to the target
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS Selection Container Widget|Primary Cursor Action|Is Sub Selected")
		ERTSSelectionPanelCursorActions ShiftAltIsSubSelectedPrimaryCursorAction;

	/**
	* On left clicking on of our items, while sub selected, with ctrl and alt held, we do this to the target
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS Selection Container Widget|Primary Cursor Action|Is Sub Selected")
		ERTSSelectionPanelCursorActions CtrlAltIsSubSelectedPrimaryCursorAction;

	/**
	* On left clicking on of our items, while sub selected, with shift, ctrl and alt held, we do this to the target
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS Selection Container Widget|Primary Cursor Action|Is Sub Selected")
		ERTSSelectionPanelCursorActions ShiftCtrlAltIsSubSelectedPrimaryCursorAction;


	/**
	* On right clicking on of our items, while not sub selected, with no modifers held, we do this to the target
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS Selection Container Widget|Secondary Cursor Action|Not Sub Selected")
		ERTSSelectionPanelCursorActions NoModifierNotSubSelectedSecondaryCursorAction;

	/**
	* On right clicking on of our items, while not sub selected, with shift held, we do this to the target
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS Selection Container Widget|Secondary Cursor Action|Not Sub Selected")
		ERTSSelectionPanelCursorActions ShiftNotSubSelectedSecondaryCursorAction;

	/**
	* On right clicking on of our items, while not sub selected, with ctrl held, we do this to the target
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS Selection Container Widget|Secondary Cursor Action|Not Sub Selected")
		ERTSSelectionPanelCursorActions CtrlNotSubSelectedSecondaryCursorAction;

	/**
	* On right clicking on of our items, while not sub selected, with alt held, we do this to the target
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS Selection Container Widget|Secondary Cursor Action|Not Sub Selected")
		ERTSSelectionPanelCursorActions AltNotSubSelectedSecondaryCursorAction;

	/**
	* On right clicking on of our items, while not sub selected, with shift and ctrl held, we do this to the target
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS Selection Container Widget|Secondary Cursor Action|Not Sub Selected")
		ERTSSelectionPanelCursorActions ShiftCtrlNotSubSelectedSecondaryCursorAction;

	/**
	* On right clicking on of our items, while not sub selected, with shift and alt held, we do this to the target
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS Selection Container Widget|Secondary Cursor Action|Not Sub Selected")
		ERTSSelectionPanelCursorActions ShiftAltNotSubSelectedSecondaryCursorAction;

	/**
	* On right clicking on of our items, while not sub selected, with ctrl and alt held, we do this to the target
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS Selection Container Widget|Secondary Cursor Action|Not Sub Selected")
		ERTSSelectionPanelCursorActions CtrlAltNotSubSelectedSecondaryCursorAction;

	/**
	* On right clicking on of our items, while not sub selected, with shift, ctrl and alt held, we do this to the target
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS Selection Container Widget|Secondary Cursor Action|Not Sub Selected")
		ERTSSelectionPanelCursorActions ShiftCtrlAltNotSubSelectedSecondaryCursorAction;


	/**
	* On right clicking on of our items, while sub selected, with no modifers held, we do this to the target
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS Selection Container Widget|Secondary Cursor Action|Is Sub Selected")
		ERTSSelectionPanelCursorActions NoModifierIsSubSelectedSecondaryCursorAction;

	/**
	* On right clicking on of our items, while sub selected, with shift held, we do this to the target
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS Selection Container Widget|Secondary Cursor Action|Is Sub Selected")
		ERTSSelectionPanelCursorActions ShiftIsSubSelectedSecondaryCursorAction;

	/**
	* On right clicking on of our items, while sub selected, with ctrl held, we do this to the target
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS Selection Container Widget|Secondary Cursor Action|Is Sub Selected")
		ERTSSelectionPanelCursorActions CtrlIsSubSelectedSecondaryCursorAction;

	/**
	* On right clicking on of our items, while sub selected, with alt held, we do this to the target
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS Selection Container Widget|Secondary Cursor Action|Is Sub Selected")
		ERTSSelectionPanelCursorActions AltIsSubSelectedSecondaryCursorAction;

	/**
	* On right clicking on of our items, while sub selected, with shift and ctrl held, we do this to the target
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS Selection Container Widget|Secondary Cursor Action|Is Sub Selected")
		ERTSSelectionPanelCursorActions ShiftCtrlIsSubSelectedSecondaryCursorAction;

	/**
	* On right clicking on of our items, while sub selected, with shift and alt held, we do this to the target
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS Selection Container Widget|Secondary Cursor Action|Is Sub Selected")
		ERTSSelectionPanelCursorActions ShiftAltIsSubSelectedSecondaryCursorAction;

	/**
	* On right clicking on of our items, while sub selected, with ctrl and alt held, we do this to the target
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS Selection Container Widget|Secondary Cursor Action|Is Sub Selected")
		ERTSSelectionPanelCursorActions CtrlAltIsSubSelectedSecondaryCursorAction;

	/**
	* On right clicking on of our items, while sub selected, with shift, ctrl and alt held, we do this to the target
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS Selection Container Widget|Secondary Cursor Action|Is Sub Selected")
		ERTSSelectionPanelCursorActions ShiftCtrlAltIsSubSelectedSecondaryCursorAction;


	/**
	* On middle clicking on of our items, while not sub selected, with no modifers held, we do this to the target
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS Selection Container Widget|Tertiary Cursor Action|Not Sub Selected")
		ERTSSelectionPanelCursorActions NoModifierNotSubSelectedTertiaryCursorAction;

	/**
	* On middle clicking on of our items, while not sub selected, with shift held, we do this to the target
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS Selection Container Widget|Tertiary Cursor Action|Not Sub Selected")
		ERTSSelectionPanelCursorActions ShiftNotSubSelectedTertiaryCursorAction;

	/**
	* On middle clicking on of our items, while not sub selected, with ctrl held, we do this to the target
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS Selection Container Widget|Tertiary Cursor Action|Not Sub Selected")
		ERTSSelectionPanelCursorActions CtrlNotSubSelectedTertiaryCursorAction;

	/**
	* On middle clicking on of our items, while not sub selected, with alt held, we do this to the target
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS Selection Container Widget|Tertiary Cursor Action|Not Sub Selected")
		ERTSSelectionPanelCursorActions AltNotSubSelectedTertiaryCursorAction;

	/**
	* On middle clicking on of our items, while not sub selected, with shift and ctrl held, we do this to the target
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS Selection Container Widget|Tertiary Cursor Action|Not Sub Selected")
		ERTSSelectionPanelCursorActions ShiftCtrlNotSubSelectedTertiaryCursorAction;

	/**
	* On middle clicking on of our items, while not sub selected, with shift and alt held, we do this to the target
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS Selection Container Widget|Tertiary Cursor Action|Not Sub Selected")
		ERTSSelectionPanelCursorActions ShiftAltNotSubSelectedTertiaryCursorAction;

	/**
	* On middle clicking on of our items, while not sub selected, with ctrl and alt held, we do this to the target
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS Selection Container Widget|Tertiary Cursor Action|Not Sub Selected")
		ERTSSelectionPanelCursorActions CtrlAltNotSubSelectedTertiaryCursorAction;

	/**
	* On middle clicking on of our items, while not sub selected, with shift, ctrl and alt held, we do this to the target
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS Selection Container Widget|Tertiary Cursor Action|Not Sub Selected")
		ERTSSelectionPanelCursorActions ShiftCtrlAltNotSubSelectedTertiaryCursorAction;


	/**
	* On middle clicking on of our items, while sub selected, with no modifers held, we do this to the target
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS Selection Container Widget|Tertiary Cursor Action|Is Sub Selected")
		ERTSSelectionPanelCursorActions NoModifierIsSubSelectedTertiaryCursorAction;

	/**
	* On middle clicking on of our items, while sub selected, with shift held, we do this to the target
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS Selection Container Widget|Tertiary Cursor Action|Is Sub Selected")
		ERTSSelectionPanelCursorActions ShiftIsSubSelectedTertiaryCursorAction;

	/**
	* On middle clicking on of our items, while sub selected, with ctrl held, we do this to the target
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS Selection Container Widget|Tertiary Cursor Action|Is Sub Selected")
		ERTSSelectionPanelCursorActions CtrlIsSubSelectedTertiaryCursorAction;

	/**
	* On middle clicking on of our items, while sub selected, with alt held, we do this to the target
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS Selection Container Widget|Tertiary Cursor Action|Is Sub Selected")
		ERTSSelectionPanelCursorActions AltIsSubSelectedTertiaryCursorAction;

	/**
	* On middle clicking on of our items, while sub selected, with shift and ctrl held, we do this to the target
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS Selection Container Widget|Tertiary Cursor Action|Is Sub Selected")
		ERTSSelectionPanelCursorActions ShiftCtrlIsSubSelectedTertiaryCursorAction;

	/**
	* On middle clicking on of our items, while sub selected, with shift and alt held, we do this to the target
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS Selection Container Widget|Tertiary Cursor Action|Is Sub Selected")
		ERTSSelectionPanelCursorActions ShiftAltIsSubSelectedTertiaryCursorAction;

	/**
	* On middle clicking on of our items, while sub selected, with ctrl and alt held, we do this to the target
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS Selection Container Widget|Tertiary Cursor Action|Is Sub Selected")
		ERTSSelectionPanelCursorActions CtrlAltIsSubSelectedTertiaryCursorAction;

	/**
	* On middle clicking on of our items, while sub selected, with shift, ctrl and alt held, we do this to the target
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS Selection Container Widget|Tertiary Cursor Action|Is Sub Selected")
		ERTSSelectionPanelCursorActions ShiftCtrlAltIsSubSelectedTertiaryCursorAction;
};
