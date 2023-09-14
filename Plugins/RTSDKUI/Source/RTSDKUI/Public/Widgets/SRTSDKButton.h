// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Misc/Attribute.h"
#include "Input/Reply.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Styling/SlateColor.h"
#include "Layout/Margin.h"
#include "Sound/SlateSound.h"
#include "Styling/SlateTypes.h"
#include "Styling/CoreStyle.h"
#include "Framework/SlateDelegates.h"
#include "Styling/SlateWidgetStyleAsset.h"
#include "Widgets/Layout/SBorder.h"

class FPaintArgs;
class FSlateWindowElementList;
enum class ETextFlowDirection : uint8;
enum class ETextShapingMethod : uint8;

/**
 * Button widget that has events for left, right and middle press/release/click
 */
class RTSDKUI_API SRTSDKButton : public SBorder
{
	SLATE_DECLARE_WIDGET(SRTSDKButton, SBorder)
#if WITH_ACCESSIBILITY
		// Allow the accessible button to "click" this button
		friend class FSlateAccessibleButton;
#endif
public:
	SLATE_BEGIN_ARGS(SRTSDKButton)
		: _Content()
		, _ButtonStyle(&FCoreStyle::Get().GetWidgetStyle< FButtonStyle >("Button"))
		, _TextStyle(&FCoreStyle::Get().GetWidgetStyle< FTextBlockStyle >("NormalText"))
		, _HAlign(HAlign_Fill)
		, _VAlign(VAlign_Fill)
		, _ContentPadding(FMargin(4.0, 2.0))
		, _Text()
		, _ClickMethod(EButtonClickMethod::DownAndUp)
		, _TouchMethod(EButtonTouchMethod::DownAndUp)
		, _PressMethod(EButtonPressMethod::DownAndUp)
		, _DesiredSizeScale(FVector2D(1, 1))
		, _ContentScale(FVector2D(1, 1))
		, _ButtonColorAndOpacity(FLinearColor::White)
		, _ForegroundColor(FCoreStyle::Get().GetSlateColor("InvertedForeground"))
		, _IsFocusable(true)
	{
	}

		/** Slot for this button's content (optional) */
		SLATE_DEFAULT_SLOT(FArguments, Content)

		/** The visual style of the button */
		SLATE_STYLE_ARGUMENT(FButtonStyle, ButtonStyle)

		/** The text style of the button */
		SLATE_STYLE_ARGUMENT(FTextBlockStyle, TextStyle)

		/** Horizontal alignment */
		SLATE_ARGUMENT(EHorizontalAlignment, HAlign)

		/** Vertical alignment */
		SLATE_ARGUMENT(EVerticalAlignment, VAlign)

		/** Spacing between button's border and the content. */
		SLATE_ATTRIBUTE(FMargin, ContentPadding)

		/** The text to display in this button, if no custom content is specified */
		SLATE_ATTRIBUTE(FText, Text)

		/** Called when the button is clicked */
		SLATE_EVENT(FOnClicked, OnClicked)

		/** Called when the button is pressed */
		SLATE_EVENT(FSimpleDelegate, OnPressed)

		/** Called when the button is released */
		SLATE_EVENT(FSimpleDelegate, OnReleased)

		/** Called when the left mouse button is pressed */
		SLATE_EVENT(FSimpleDelegate, OnLeftMouseClicked)

		/** Called when the left mouse button is pressed */
		SLATE_EVENT(FSimpleDelegate, OnLeftMousePressed)

		/** Called when the left mouse button is released */
		SLATE_EVENT(FSimpleDelegate, OnLeftMouseReleased)

		/** Called when the right mouse button is pressed */
		SLATE_EVENT(FSimpleDelegate, OnRightMouseClicked)

		/** Called when the right mouse button is pressed */
		SLATE_EVENT(FSimpleDelegate, OnRightMousePressed)

		/** Called when the right mouse button is released */
		SLATE_EVENT(FSimpleDelegate, OnRightMouseReleased)

		/** Called when the middle mouse button is pressed */
		SLATE_EVENT(FSimpleDelegate, OnMiddleMouseClicked)

		/** Called when the middle mouse button is pressed */
		SLATE_EVENT(FSimpleDelegate, OnMiddleMousePressed)

		/** Called when the middle mousebutton is released */
		SLATE_EVENT(FSimpleDelegate, OnMiddleMouseReleased)

		SLATE_EVENT(FSimpleDelegate, OnHovered)

		SLATE_EVENT(FSimpleDelegate, OnUnhovered)

		/** Sets the rules to use for determining whether the button was clicked.  This is an advanced setting and generally should be left as the default. */
		SLATE_ARGUMENT(EButtonClickMethod::Type, ClickMethod)

		/** How should the button be clicked with touch events? */
		SLATE_ARGUMENT(EButtonTouchMethod::Type, TouchMethod)

		/** How should the button be clicked with keyboard/controller button events? */
		SLATE_ARGUMENT(EButtonPressMethod::Type, PressMethod)

		SLATE_ATTRIBUTE(FVector2D, DesiredSizeScale)

		SLATE_ATTRIBUTE(FVector2D, ContentScale)

		SLATE_ATTRIBUTE(FSlateColor, ButtonColorAndOpacity)

		SLATE_ATTRIBUTE(FSlateColor, ForegroundColor)

		/** Sometimes a button should only be mouse-clickable and never keyboard focusable. */
		SLATE_ARGUMENT(bool, IsFocusable)

		/** The sound to play when the button is pressed */
		SLATE_ARGUMENT(TOptional<FSlateSound>, PressedSoundOverride)

		/** The sound to play when the button is hovered */
		SLATE_ARGUMENT(TOptional<FSlateSound>, HoveredSoundOverride)

		/** Which text shaping method should we use? (unset to use the default returned by GetDefaultTextShapingMethod) */
		SLATE_ARGUMENT(TOptional<ETextShapingMethod>, TextShapingMethod)

		/** Which text flow direction should we use? (unset to use the default returned by GetDefaultTextFlowDirection) */
		SLATE_ARGUMENT(TOptional<ETextFlowDirection>, TextFlowDirection)

	SLATE_END_ARGS()

	SRTSDKButton();

	/** @return the Foreground color that this widget sets when this widget or any of its ancestors are disabled; unset options if the widget does not set a foreground color */
	virtual FSlateColor GetDisabledForegroundColor() const override;

	/**
	 * Construct this widget
	 *
	 * @param	InArgs	The declaration data for this widget
	 */
	void Construct(const FArguments& InArgs);

	/** See ContentPadding attribute */
	void SetContentPadding(TAttribute<FMargin> InContentPadding);

	/** See HoveredSound attribute */
	void SetHoveredSound(TOptional<FSlateSound> InHoveredSound);

	/** See PressedSound attribute */
	void SetPressedSound(TOptional<FSlateSound> InPressedSound);

	/** See OnClicked event */
	void SetOnClicked(FOnClicked InOnClicked);

	/** Set OnHovered event */
	void SetOnHovered(FSimpleDelegate InOnHovered);

	/** Set OnUnhovered event */
	void SetOnUnhovered(FSimpleDelegate InOnUnhovered);

	/** See ButtonStyle attribute */
	void SetButtonStyle(const FButtonStyle* ButtonStyle);

	void SetClickMethod(EButtonClickMethod::Type InClickMethod);
	void SetTouchMethod(EButtonTouchMethod::Type InTouchMethod);
	void SetPressMethod(EButtonPressMethod::Type InPressMethod);

	virtual bool IsPressed() const
	{
		return bIsPressed || IsRightMousePressed || IsMiddleMousePressed;
	}

#if !UE_BUILD_SHIPPING
	void SimulateClick();
#endif // !UE_BUILD_SHIPPING

	//~ SWidget overrides
	virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;
	virtual bool SupportsKeyboardFocus() const override;
	virtual void OnFocusLost(const FFocusEvent& InFocusEvent) override;
	virtual FReply OnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent) override;
	virtual FReply OnKeyUp(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent) override;
	virtual FReply OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual FReply OnMouseButtonDoubleClick(const FGeometry& InMyGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual FReply OnMouseMove(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual void OnMouseEnter(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual void OnMouseLeave(const FPointerEvent& MouseEvent) override;
	virtual void OnMouseCaptureLost(const FCaptureLostEvent& CaptureLostEvent) override;
	virtual bool IsInteractable() const override;
#if WITH_ACCESSIBILITY
	virtual TSharedRef<FSlateAccessibleWidget> CreateAccessibleWidget() override;
#endif
private:
	virtual FVector2D ComputeDesiredSize(float) const override;
protected:
	/** Press the button */
	virtual void Press();

	/** Release the button */
	virtual void Release();

	virtual void RightMousePress();
	virtual void RightMouseRelease();

	virtual void MiddleMousePress();
	virtual void MiddleMouseRelease();

	//~ SWidget
	/** Execute the "OnClicked" delegate, and get the reply */
	FReply ExecuteOnClick();

	/** @return combines the user-specified margin and the button's internal margin. */
	FMargin GetCombinedPadding() const;

	/** @return True if the disabled effect should be shown. */
	bool GetShowDisabledEffect() const;

	/** Utility function to translate other input click methods to regular ones. */
	TEnumAsByte<EButtonClickMethod::Type> GetClickMethodFromInputType(const FPointerEvent& MouseEvent) const;

	/** Utility function to determine if the incoming mouse event is for a precise tap or click */
	bool IsPreciseTapOrClick(const FPointerEvent& MouseEvent) const;

	/** Play the pressed sound */
	void PlayPressedSound() const;

	/** Play the hovered sound */
	void PlayHoverSound() const;

	/** Set if this button can be focused */
	void SetIsFocusable(bool bInIsFocusable)
	{
		bIsFocusable = bInIsFocusable;
	}

	void ExecuteHoverStateChanged(bool bPlaySound);

	/** @return the BorderForegroundColor attribute. */
	TSlateAttributeRef<FSlateColor> GetBorderForegroundColorAttribute() const { return TSlateAttributeRef<FSlateColor>(SharedThis(this), BorderForegroundColorAttribute); }

	/** @return the ContentPadding attribute. */
	TSlateAttributeRef<FMargin> GetContentPaddingAttribute() const { return TSlateAttributeRef<FMargin>(SharedThis(this), ContentPaddingAttribute); }

	/** Set the AppearPressed look. */
	void SetAppearPressed(TAttribute<bool> InValue)
	{
		AppearPressedAttribute.Assign(*this, MoveTemp(InValue));
	}

	/** @return the AppearPressed attribute. */
	TSlateAttributeRef<bool> GetAppearPressedAttribute() const { return TSlateAttributeRef<bool>(SharedThis(this), AppearPressedAttribute); }

	void UpdatePressStateChanged();

	void UpdatePadding();
	void UpdateShowDisabledEffect();
	void UpdateBorderImage();
	void UpdateForegroundColor();
	void UpdateDisabledForegroundColor();

	/** The location in screenspace the button was pressed */
	FVector2D PressedScreenSpacePosition;

	/** Style resource for the button */
	const FButtonStyle* Style;
	/** The delegate to execute when the button is clicked */
	FOnClicked OnClicked;

	/** The delegate to execute when the button is pressed */
	FSimpleDelegate OnPressed;

	/** The delegate to execute when the button is released */
	FSimpleDelegate OnReleased;

	/** The delegate to execute when the button is hovered */
	FSimpleDelegate OnHovered;

	/** The delegate to execute when the button exit the hovered state */
	FSimpleDelegate OnUnhovered;

	/** The Sound to play when the button is hovered  */
	FSlateSound HoveredSound;

	/** The Sound to play when the button is pressed */
	FSlateSound PressedSound;

	/** Sets whether a click should be triggered on mouse down, mouse up, or that both a mouse down and up are required. */
	TEnumAsByte<EButtonClickMethod::Type> ClickMethod;

	/** How should the button be clicked with touch events? */
	TEnumAsByte<EButtonTouchMethod::Type> TouchMethod;

	/** How should the button be clicked with keyboard/controller button events? */
	TEnumAsByte<EButtonPressMethod::Type> PressMethod;

	/** Can this button be focused? */
	uint8 bIsFocusable : 1;

	/** True if this button is currently in a pressed state */
	uint8 bIsPressed : 1;

	/** Optional foreground color that will be inherited by all of this widget's contents */
	TSlateAttribute<FSlateColor> BorderForegroundColorAttribute;
	/** Padding specified by the user; it will be combind with the button's internal padding. */
	TSlateAttribute<FMargin> ContentPaddingAttribute;
	/** Optional foreground color that will be inherited by all of this widget's contents */
	TSlateAttribute<bool> AppearPressedAttribute;

	bool IsRightMousePressed;
	bool IsMiddleMousePressed;

	FSimpleDelegate OnLeftMouseReleased;
	FSimpleDelegate OnRightMouseReleased;
	FSimpleDelegate OnMiddleMouseReleased;
	
	FSimpleDelegate OnLeftMousePressed;
	FSimpleDelegate OnRightMousePressed;
	FSimpleDelegate OnMiddleMousePressed;

	FSimpleDelegate OnLeftMouseClicked;
	FSimpleDelegate OnRightMouseClicked;
	FSimpleDelegate OnMiddleMouseClicked;
};
