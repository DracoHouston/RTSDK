// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"
#include "Layout/Margin.h"
#include "Widgets/SRTSDKScrollBox.h"
#include "Components/PanelSlot.h"

#include "RTSDKScrollBoxSlot.generated.h"

/** The Slot for the URTSDKScrollBox, contains the widget that are scrollable */
UCLASS()
class RTSDKUI_API URTSDKScrollBoxSlot : public UPanelSlot
{
	GENERATED_UCLASS_BODY()

public:
	
	/** The padding area between the slot and the content it contains. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Layout|ScrollBox Slot")
	FMargin Padding;

	/** The alignment of the object horizontally. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Layout|ScrollBox Slot")
	TEnumAsByte<EHorizontalAlignment> HorizontalAlignment;

	/** The alignment of the object vertically. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Layout|ScrollBox Slot")
	TEnumAsByte<EVerticalAlignment> VerticalAlignment;

public:

	FMargin GetPadding() const;

	UFUNCTION(BlueprintCallable, Category = "Layout|ScrollBox Slot")
		void SetPadding(FMargin InPadding);

	EHorizontalAlignment GetHorizontalAlignment() const;

	UFUNCTION(BlueprintCallable, Category = "Layout|ScrollBox Slot")
		void SetHorizontalAlignment(EHorizontalAlignment InHorizontalAlignment);

	EVerticalAlignment GetVerticalAlignment() const;

	UFUNCTION(BlueprintCallable, Category = "Layout|ScrollBox Slot")
		void SetVerticalAlignment(EVerticalAlignment InVerticalAlignment);

public:

	// UPanelSlot interface
	virtual void SynchronizeProperties() override;
	// End of UPanelSlot interface

	/** Builds the underlying FSlot for the Slate layout panel. */
	void BuildSlot(TSharedRef<SRTSDKScrollBox> ScrollBox);

	virtual void ReleaseSlateResources(bool bReleaseChildren) override;

private:
	//TODO UMG Slots should hold weak or shared refs to slots.

	/** A raw pointer to the slot to allow us to adjust the size, padding...etc at runtime. */
	SRTSDKScrollBox::FSlot* Slot;
};
