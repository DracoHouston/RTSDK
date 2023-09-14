// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Input/Reply.h"
#include "Styling/SlateTypes.h"
#include "Widgets/SWidget.h"
#include "Components/ContentWidget.h"
#include "RTSDKButton.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnButtonLeftMouseClickedEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnButtonLeftMousePressedEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnButtonLeftMouseReleasedEvent);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnButtonRightMouseClickedEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnButtonRightMousePressedEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnButtonRightMouseReleasedEvent);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnButtonMiddleMouseClickedEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnButtonMiddleMousePressedEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnButtonMiddleMouseReleasedEvent);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnButtonClickedEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnButtonPressedEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnButtonReleasedEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnButtonHoverEvent);

class SRTSDKButton;
class USlateWidgetStyleAsset;

/**
 * 
 */
UCLASS()
class RTSDKUI_API URTSDKButton : public UContentWidget
{
	GENERATED_BODY()
public:
	URTSDKButton(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/** The button style used at runtime */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Appearance", meta = (DisplayName = "Style"))
		FButtonStyle WidgetStyle;

	/** The color multiplier for the button content */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Appearance", meta = (sRGB = "true"))
		FLinearColor ColorAndOpacity;

	/** The color multiplier for the button background */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Appearance", meta = (sRGB = "true"))
		FLinearColor BackgroundColor;

	/** The type of mouse action required by the user to trigger the buttons 'Click' */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction", AdvancedDisplay)
		TEnumAsByte<EButtonClickMethod::Type> ClickMethod;

	/** The type of touch action required by the user to trigger the buttons 'Click' */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction", AdvancedDisplay)
		TEnumAsByte<EButtonTouchMethod::Type> TouchMethod;

	/** The type of keyboard/gamepad button press action required by the user to trigger the buttons 'Click' */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction", AdvancedDisplay)
		TEnumAsByte<EButtonPressMethod::Type> PressMethod;

	/** Sometimes a button should only be mouse-clickable and never keyboard focusable. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction")
		bool IsFocusable;

	/** Called when the button is clicked */
	UPROPERTY(BlueprintAssignable, Category = "Button|Event")
		FOnButtonClickedEvent OnClicked;

	/** Called when the button is pressed */
	UPROPERTY(BlueprintAssignable, Category = "Button|Event")
		FOnButtonPressedEvent OnPressed;

	/** Called when the button is released */
	UPROPERTY(BlueprintAssignable, Category = "Button|Event")
		FOnButtonReleasedEvent OnReleased;

	UPROPERTY(BlueprintAssignable, Category = "Button|Event")
		FOnButtonHoverEvent OnHovered;

	UPROPERTY(BlueprintAssignable, Category = "Button|Event")
		FOnButtonHoverEvent OnUnhovered;

	UPROPERTY(BlueprintAssignable, Category = "Button|Event")
		FOnButtonLeftMouseClickedEvent OnLeftMouseClicked;

	UPROPERTY(BlueprintAssignable, Category = "Button|Event")
		FOnButtonLeftMousePressedEvent OnLeftMousePressed;

	UPROPERTY(BlueprintAssignable, Category = "Button|Event")
		FOnButtonLeftMouseReleasedEvent OnLeftMouseReleased;

	UPROPERTY(BlueprintAssignable, Category = "Button|Event")
		FOnButtonRightMouseClickedEvent OnRightMouseClicked;

	UPROPERTY(BlueprintAssignable, Category = "Button|Event")
		FOnButtonRightMousePressedEvent OnRightMousePressed;

	UPROPERTY(BlueprintAssignable, Category = "Button|Event")
		FOnButtonRightMouseReleasedEvent OnRightMouseReleased;

	UPROPERTY(BlueprintAssignable, Category = "Button|Event")
		FOnButtonMiddleMouseClickedEvent OnMiddleMouseClicked;

	UPROPERTY(BlueprintAssignable, Category = "Button|Event")
		FOnButtonMiddleMousePressedEvent OnMiddleMousePressed;

	UPROPERTY(BlueprintAssignable, Category = "Button|Event")
		FOnButtonMiddleMouseReleasedEvent OnMiddleMouseReleased;

	/** Sets the color multiplier for the button background */
	UFUNCTION(BlueprintCallable, Category = "Button|Appearance")
		void SetStyle(const FButtonStyle& InStyle);

	/** Sets the color multiplier for the button content */
	UFUNCTION(BlueprintCallable, Category = "Button|Appearance")
		void SetColorAndOpacity(FLinearColor InColorAndOpacity);

	/** Sets the color multiplier for the button background */
	UFUNCTION(BlueprintCallable, Category = "Button|Appearance")
		void SetBackgroundColor(FLinearColor InBackgroundColor);

	/**
	 * Returns true if the user is actively pressing the button.  Do not use this for detecting 'Clicks', use the OnClicked event instead.
	 *
	 * @return true if the user is actively pressing the button otherwise false.
	 */
	UFUNCTION(BlueprintCallable, Category = "Button")
		bool IsPressed() const;

	UFUNCTION(BlueprintCallable, Category = "Button")
		void SetClickMethod(EButtonClickMethod::Type InClickMethod);

	UFUNCTION(BlueprintCallable, Category = "Button")
		void SetTouchMethod(EButtonTouchMethod::Type InTouchMethod);

	UFUNCTION(BlueprintCallable, Category = "Button")
		void SetPressMethod(EButtonPressMethod::Type InPressMethod);

	//~ Begin UWidget Interface
	virtual void SynchronizeProperties() override;
	//~ End UWidget Interface

	//~ Begin UVisual Interface
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;
	//~ End UVisual Interface

	//~ Begin UObject Interface
	virtual void PostLoad() override;
	//~ End UObject Interface

#if WITH_EDITOR
	virtual const FText GetPaletteCategory() override;
#endif

protected:

	// UPanelWidget
	virtual UClass* GetSlotClass() const override;
	virtual void OnSlotAdded(UPanelSlot* Slot) override;
	virtual void OnSlotRemoved(UPanelSlot* Slot) override;
	// End UPanelWidget

	/** Handle the actual click event from slate and forward it on */
	FReply SlateHandleClicked();
	void SlateHandlePressed();
	void SlateHandleReleased();
	void SlateHandleHovered();
	void SlateHandleUnhovered();
	void SlateHandleLeftMouseClicked();
	void SlateHandleLeftMousePressed();
	void SlateHandleLeftMouseReleased();
	void SlateHandleRightMouseClicked();
	void SlateHandleRightMousePressed();
	void SlateHandleRightMouseReleased();
	void SlateHandleMiddleMouseClicked();
	void SlateHandleMiddleMousePressed();
	void SlateHandleMiddleMouseReleased();

	//~ Begin UWidget Interface
	virtual TSharedRef<SWidget> RebuildWidget() override;
#if WITH_EDITOR
	virtual TSharedRef<SWidget> RebuildDesignWidget(TSharedRef<SWidget> Content) override { return Content; }
#endif
	//~ End UWidget Interface

#if WITH_ACCESSIBILITY
	virtual TSharedPtr<SWidget> GetAccessibleWidget() const override;
#endif

	TSharedPtr<SRTSDKButton> MyButton;
};
