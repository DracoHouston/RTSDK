// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"
#include "Layout/Margin.h"
#include "Components/PanelSlot.h"

#include "RTSDKButtonSlot.generated.h"

class SRTSDKButton;

/** The Slot for the UButtonSlot, contains the widget displayed in a button's single slot */
UCLASS()
class RTSDKUI_API URTSDKButtonSlot : public UPanelSlot
{
	GENERATED_BODY()

public:

	URTSDKButtonSlot(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	/** The padding area between the slot and the content it contains. */
	UE_DEPRECATED(5.1, "Direct access to Padding is deprecated. Please use the getter or setter.")
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Getter, Setter, BlueprintSetter="SetPadding", Category="Layout|Button Slot")
	FMargin Padding;

	/** The alignment of the object horizontally. */
	UE_DEPRECATED(5.1, "Direct access to HorizontalAlignment is deprecated. Please use the getter or setter.")
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Getter, Setter, BlueprintSetter="SetHorizontalAlignment", Category="Layout|Button Slot")
	TEnumAsByte<EHorizontalAlignment> HorizontalAlignment;

	/** The alignment of the object vertically. */
	UE_DEPRECATED(5.1, "Direct access to VerticalAlignment is deprecated. Please use the getter or setter.")
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Getter, Setter, BlueprintSetter="SetVerticalAlignment", Category="Layout|Button Slot")
	TEnumAsByte<EVerticalAlignment> VerticalAlignment;

public:

	FMargin GetPadding() const;

	UFUNCTION(BlueprintCallable, Category="Layout|Button Slot")
	void SetPadding(FMargin InPadding);

	EHorizontalAlignment GetHorizontalAlignment() const;

	UFUNCTION(BlueprintCallable, Category="Layout|Button Slot")
	void SetHorizontalAlignment(EHorizontalAlignment InHorizontalAlignment);

	EVerticalAlignment GetVerticalAlignment() const;

	UFUNCTION(BlueprintCallable, Category="Layout|Button Slot")
	void SetVerticalAlignment(EVerticalAlignment InVerticalAlignment);

	// UPanelSlot interface
	virtual void SynchronizeProperties() override;
	// End of UPanelSlot interface

	/** Builds the underlying slot for the slate button. */
	void BuildSlot(TSharedRef<SRTSDKButton> InButton);

	virtual void ReleaseSlateResources(bool bReleaseChildren) override;

private:

	/** A pointer to the button to allow us to adjust the size, padding...etc at runtime. */
	TWeakPtr<SRTSDKButton> Button;
};
