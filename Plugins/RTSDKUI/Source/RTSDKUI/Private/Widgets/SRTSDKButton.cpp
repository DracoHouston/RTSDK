// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/SRTSDKButton.h"
#include "Rendering/DrawElements.h"
#include "Framework/Application/SlateApplication.h"
#include "Widgets/Text/STextBlock.h"
#if WITH_ACCESSIBILITY
#include "Widgets/Accessibility/SlateAccessibleWidgets.h"
#include "Widgets/Accessibility/SlateAccessibleMessageHandler.h"
#endif

static FName SRTSDKButtonTypeName("SRTSDKButton");

SLATE_IMPLEMENT_WIDGET(SRTSDKButton)
void SRTSDKButton::PrivateRegisterAttributes(FSlateAttributeInitializer& AttributeInitializer)
{
	SLATE_ADD_MEMBER_ATTRIBUTE_DEFINITION_WITH_NAME(AttributeInitializer, "BaseBorderForegroundColor", BorderForegroundColorAttribute, EInvalidateWidgetReason::Paint)
		.OnValueChanged(FSlateAttributeDescriptor::FAttributeValueChangedDelegate::CreateLambda([](SWidget& Widget)
			{
				static_cast<SRTSDKButton&>(Widget).UpdateForegroundColor();
			}));

	SLATE_ADD_MEMBER_ATTRIBUTE_DEFINITION_WITH_NAME(AttributeInitializer, "ContentPadding", ContentPaddingAttribute, EInvalidateWidgetReason::Layout)
		.OnValueChanged(FSlateAttributeDescriptor::FAttributeValueChangedDelegate::CreateLambda([](SWidget& Widget)
		{
			static_cast<SRTSDKButton&>(Widget).UpdatePadding();
		}));
	
	SLATE_ADD_MEMBER_ATTRIBUTE_DEFINITION_WITH_NAME(AttributeInitializer, "AppearPressed", AppearPressedAttribute, EInvalidateWidgetReason::Paint)
		.OnValueChanged(FSlateAttributeDescriptor::FAttributeValueChangedDelegate::CreateLambda([](SWidget& Widget)
			{
				static_cast<SRTSDKButton&>(Widget).UpdatePressStateChanged();
			}));

	AttributeInitializer.OverrideInvalidationReason("EnabledState", FSlateAttributeDescriptor::FInvalidateWidgetReasonAttribute{EInvalidateWidgetReason::Layout|EInvalidateWidgetReason::Paint});
	AttributeInitializer.OverrideInvalidationReason("Hovered", FSlateAttributeDescriptor::FInvalidateWidgetReasonAttribute{EInvalidateWidgetReason::Layout|EInvalidateWidgetReason::Paint});

	AttributeInitializer.OverrideOnValueChanged("EnabledState"
		, FSlateAttributeDescriptor::ECallbackOverrideType::ExecuteAfterPrevious
		, FSlateAttributeDescriptor::FAttributeValueChangedDelegate::CreateLambda([](SWidget& Widget)
			{
				static_cast<SRTSDKButton&>(Widget).UpdateBorderImage();
			}));

	AttributeInitializer.OverrideOnValueChanged("Hovered"
		, FSlateAttributeDescriptor::ECallbackOverrideType::ExecuteAfterPrevious
		, FSlateAttributeDescriptor::FAttributeValueChangedDelegate::CreateLambda([](SWidget& Widget)
			{
				static_cast<SRTSDKButton&>(Widget).UpdateBorderImage();
				static_cast<SRTSDKButton&>(Widget).UpdateForegroundColor();
			}));
}

SRTSDKButton::SRTSDKButton()
	: BorderForegroundColorAttribute(*this)
	, ContentPaddingAttribute(*this)
	, AppearPressedAttribute(*this)
{
#if WITH_ACCESSIBILITY
	AccessibleBehavior = EAccessibleBehavior::Summary;
	bCanChildrenBeAccessible = false;
#endif
}

/**
 * Construct this widget
 *
 * @param	InArgs	The declaration data for this widget
 */
void SRTSDKButton::Construct(const FArguments& InArgs)
{
	bIsPressed = false;
	IsRightMousePressed = false;
	IsMiddleMousePressed = false;
	bIsFocusable = InArgs._IsFocusable;

	BorderForegroundColorAttribute.Assign(*this, InArgs._ForegroundColor);

	OnClicked = InArgs._OnClicked;
	OnPressed = InArgs._OnPressed;
	OnReleased = InArgs._OnReleased;
	OnLeftMouseClicked = InArgs._OnLeftMouseClicked;
	OnLeftMousePressed = InArgs._OnLeftMousePressed;
	OnLeftMouseReleased = InArgs._OnLeftMouseReleased;
	
	OnRightMouseClicked = InArgs._OnRightMouseClicked;
	OnRightMousePressed = InArgs._OnRightMousePressed;
	OnRightMouseReleased = InArgs._OnRightMouseReleased;

	OnMiddleMouseClicked = InArgs._OnMiddleMouseClicked;
	OnMiddleMousePressed = InArgs._OnMiddleMousePressed;
	OnMiddleMouseReleased = InArgs._OnMiddleMouseReleased;
	OnHovered = InArgs._OnHovered;
	OnUnhovered = InArgs._OnUnhovered;

	ClickMethod = InArgs._ClickMethod;
	TouchMethod = InArgs._TouchMethod;
	PressMethod = InArgs._PressMethod;

	HoveredSound = InArgs._HoveredSoundOverride.Get(InArgs._ButtonStyle->HoveredSlateSound);
	PressedSound = InArgs._PressedSoundOverride.Get(InArgs._ButtonStyle->PressedSlateSound);

	// Text overrides button content. If nothing is specified, put an null widget in the button.
	// Null content makes the button enter a special mode where it will ask to be as big as the image used for its border.
	struct
	{
		TSharedRef<SWidget> operator()(const FArguments& InOpArgs) const
		{
			if ((InOpArgs._Content.Widget == SNullWidget::NullWidget) && (InOpArgs._Text.IsBound() || !InOpArgs._Text.Get().IsEmpty()))
			{
				return SNew(STextBlock)
					.Visibility(EVisibility::HitTestInvisible)
					.Text(InOpArgs._Text)
					.TextStyle(InOpArgs._TextStyle)
					.TextShapingMethod(InOpArgs._TextShapingMethod)
					.TextFlowDirection(InOpArgs._TextFlowDirection);
			}
			else
			{
				return InOpArgs._Content.Widget;
			}
		}
	} DetermineContent;

	SBorder::Construct(SBorder::FArguments()
		.ContentScale(InArgs._ContentScale)
		.DesiredSizeScale(InArgs._DesiredSizeScale)
		.BorderBackgroundColor(InArgs._ButtonColorAndOpacity)
		.HAlign(InArgs._HAlign)
		.VAlign(InArgs._VAlign)
		[
			DetermineContent(InArgs)
		]
	);

	SetContentPadding(InArgs._ContentPadding);
	SetButtonStyle(InArgs._ButtonStyle);

	// Only do this if we're exactly an SButton
	if (GetType() == SRTSDKButtonTypeName)
	{
		SetCanTick(false);
	}
}

int32 SRTSDKButton::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	bool bEnabled = ShouldBeEnabled(bParentEnabled);
	bool bShowDisabledEffect = GetShowDisabledEffect();

	const FSlateBrush* BrushResource = !bShowDisabledEffect && !bEnabled ? &Style->Disabled : GetBorderImage();

	ESlateDrawEffect DrawEffects = bShowDisabledEffect && !bEnabled ? ESlateDrawEffect::DisabledEffect : ESlateDrawEffect::None;

	if (BrushResource && BrushResource->DrawAs != ESlateBrushDrawType::NoDrawType)
	{
		FSlateDrawElement::MakeBox(
			OutDrawElements,
			LayerId,
			AllottedGeometry.ToPaintGeometry(),
			BrushResource,
			DrawEffects,
			BrushResource->GetTint(InWidgetStyle) * InWidgetStyle.GetColorAndOpacityTint() * GetBorderBackgroundColor().GetColor(InWidgetStyle)
		);
	}

	return SCompoundWidget::OnPaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bEnabled);
}

FMargin SRTSDKButton::GetCombinedPadding() const
{
	return (IsPressed())
		? ContentPaddingAttribute.Get() + Style->PressedPadding
		: ContentPaddingAttribute.Get() + Style->NormalPadding;
}

//~ Update when { ContentPaddingAttribute, IsPressed, Style }
void SRTSDKButton::UpdatePadding()
{
	SetPadding(GetCombinedPadding());
}

bool SRTSDKButton::GetShowDisabledEffect() const
{
	return Style->Disabled.DrawAs == ESlateBrushDrawType::NoDrawType;
}

//~ Update when { Style }
void SRTSDKButton::UpdateShowDisabledEffect()
{
	// Needs to be called when the style changed
	SetShowEffectWhenDisabled(GetShowDisabledEffect());
}

//~ Update when { GetShowDisabledEffect(Style), IsEnable(EnabledState), Pressed, Hovered, Style }
void SRTSDKButton::UpdateBorderImage()
{
	if (!GetShowDisabledEffect() && !IsInteractable())
	{
		SetBorderImage(&Style->Disabled);
	}
	else if (IsPressed())
	{
		SetBorderImage(&Style->Pressed);
	}
	else if (IsHovered())
	{
		SetBorderImage(&Style->Hovered);
	}
	else
	{
		SetBorderImage(&Style->Normal);
	}
}

//~ Update when { DefaultForegroundColorAttribute, Pressed, Hovered, Style }
void SRTSDKButton::UpdateForegroundColor()
{
	if (BorderForegroundColorAttribute.Get() == FSlateColor::UseStyle())
	{
		if (IsPressed())
		{
			SetForegroundColor(Style->PressedForeground);
		}
		else if (IsHovered())
		{
			SetForegroundColor(Style->HoveredForeground);
		}
		else
		{
			SetForegroundColor(Style->NormalForeground);
		}
	}
	else
	{
		SetForegroundColor(BorderForegroundColorAttribute.Get());
	}
}

//~ Update when { Style }
void SRTSDKButton::UpdateDisabledForegroundColor()
{
	Invalidate(EInvalidateWidgetReason::Paint);
}

bool SRTSDKButton::SupportsKeyboardFocus() const
{
	// Buttons are focusable by default
	return bIsFocusable;
}

void SRTSDKButton::OnFocusLost(const FFocusEvent& InFocusEvent)
{
	SBorder::OnFocusLost(InFocusEvent);

	Release();
}

FReply SRTSDKButton::OnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent)
{
	FReply Reply = FReply::Unhandled();
	if (IsEnabled() && FSlateApplication::Get().GetNavigationActionFromKey(InKeyEvent) == EUINavigationAction::Accept)
	{
		Press();

		if (PressMethod == EButtonPressMethod::ButtonPress)
		{
			//execute our "OnClicked" delegate, and get the reply
			Reply = ExecuteOnClick();

			//You should ALWAYS handle the OnClicked event.
			ensure(Reply.IsEventHandled() == true);
		}
		else
		{
			Reply = FReply::Handled();
		}
	}
	else
	{
		Reply = SBorder::OnKeyDown(MyGeometry, InKeyEvent);
	}

	//return the constructed reply
	return Reply;
}

FReply SRTSDKButton::OnKeyUp(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent)
{
	FReply Reply = FReply::Unhandled();

	if (IsEnabled() && FSlateApplication::Get().GetNavigationActionFromKey(InKeyEvent) == EUINavigationAction::Accept)
	{
		const bool bWasPressed = bIsPressed;

		Release();

		//@Todo Slate: This should check focus, however we don't have that API yet, will be easier when focus is unified.
		if (PressMethod == EButtonPressMethod::ButtonRelease || (PressMethod == EButtonPressMethod::DownAndUp && bWasPressed))
		{
			//execute our "OnClicked" delegate, and get the reply
			Reply = ExecuteOnClick();

			//You should ALWAYS handle the OnClicked event.
			ensure(Reply.IsEventHandled() == true);
		}
		else
		{
			Reply = FReply::Handled();
		}
	}

	//return the constructed reply
	return Reply;
}

FReply SRTSDKButton::OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	FReply Reply = FReply::Unhandled();
	if (IsEnabled() && (((MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton) || (MouseEvent.GetEffectingButton() == EKeys::RightMouseButton) || (MouseEvent.GetEffectingButton() == EKeys::MiddleMouseButton)) || MouseEvent.IsTouchEvent()))
	{
		if ((MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton) || MouseEvent.IsTouchEvent())
		{
			Press();
		}
		else if (MouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
		{
			RightMousePress();
		}
		else if (MouseEvent.GetEffectingButton() == EKeys::MiddleMouseButton)
		{
			MiddleMousePress();
		}

		PressedScreenSpacePosition = MouseEvent.GetScreenSpacePosition();

		EButtonClickMethod::Type InputClickMethod = GetClickMethodFromInputType(MouseEvent);

		if (InputClickMethod == EButtonClickMethod::MouseDown)
		{
			//get the reply from the execute function
			Reply = ExecuteOnClick();

			//You should ALWAYS handle the OnClicked event.
			ensure(Reply.IsEventHandled() == true);

			if (MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
			{
				OnLeftMouseClicked.ExecuteIfBound();
			}
			else if (MouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
			{
				OnRightMouseClicked.ExecuteIfBound();
			}
			else if (MouseEvent.GetEffectingButton() == EKeys::MiddleMouseButton)
			{
				OnMiddleMouseClicked.ExecuteIfBound();
			}
		}
		else if (InputClickMethod == EButtonClickMethod::PreciseClick)
		{
			// do not capture the pointer for precise taps or clicks
			// 
			Reply = FReply::Handled();
		}
		else
		{
			//we need to capture the mouse for MouseUp events
			Reply = FReply::Handled().CaptureMouse(AsShared());
		}
	}

	//return the constructed reply
	return Reply;
}



FReply SRTSDKButton::OnMouseButtonDoubleClick(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	FReply Reply = SBorder::OnMouseButtonDoubleClick(MyGeometry, MouseEvent);
	if (Reply.IsEventHandled())
	{
		return Reply;
	}

	// We didn't handle the double click, treat it as single click
	return OnMouseButtonDown(MyGeometry, MouseEvent);
}

FReply SRTSDKButton::OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	FReply Reply = FReply::Unhandled();
	
	const EButtonClickMethod::Type InputClickMethod = GetClickMethodFromInputType(MouseEvent);
	
	const bool bMustBePressed = InputClickMethod == EButtonClickMethod::DownAndUp || InputClickMethod == EButtonClickMethod::PreciseClick;
	const bool bMeetsPressedRequirements = (!bMustBePressed || (bIsPressed && bMustBePressed));
	const bool MeetsRightPressedRequirements = (!bMustBePressed || (IsRightMousePressed && bMustBePressed));
	const bool MeetsMiddlePressedRequirements = (!bMustBePressed || (IsMiddleMousePressed && bMustBePressed));
	bool bEventOverButton = IsHovered();

	if (!bEventOverButton && MouseEvent.IsTouchEvent())
	{
		bEventOverButton = MyGeometry.IsUnderLocation(MouseEvent.GetScreenSpacePosition());
	}

	// If we asked for a precise tap, all we need is for the user to have not moved their pointer very far.
	const bool bTriggerForTouchEvent = InputClickMethod == EButtonClickMethod::PreciseClick;

	// If we were asked to allow the button to be clicked on mouse up, regardless of whether the user
	// pressed the button down first, then we'll allow the click to proceed without an active capture
	const bool bTriggerForMouseEvent = (InputClickMethod == EButtonClickMethod::MouseUp || HasMouseCapture());

	if (bMeetsPressedRequirements && ((MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton || MouseEvent.IsTouchEvent())))
	{
		Release();

		if (IsEnabled())
		{
			if (InputClickMethod != EButtonClickMethod::MouseDown)
			{
				if (bEventOverButton)
				{	
					if ((bTriggerForTouchEvent || bTriggerForMouseEvent))
					{
						Reply = ExecuteOnClick();
						OnLeftMouseClicked.ExecuteIfBound();
					}
				}
			}
		}

		//If the user of the button didn't handle this click, then the button's
		//default behavior handles it.
		if (Reply.IsEventHandled() == false)
		{
			Reply = FReply::Handled();
		}
	}
	else if (MeetsRightPressedRequirements && (MouseEvent.GetEffectingButton() == EKeys::RightMouseButton))
	{
		RightMouseRelease();

		if (IsEnabled())
		{
			if (InputClickMethod != EButtonClickMethod::MouseDown)
			{
				if (bEventOverButton)
				{
					if ((bTriggerForTouchEvent || bTriggerForMouseEvent))
					{
						OnRightMouseClicked.ExecuteIfBound();
					}
				}
			}
		}
	}

	else if (MeetsMiddlePressedRequirements && (MouseEvent.GetEffectingButton() == EKeys::MiddleMouseButton))
	{
		MiddleMouseRelease();

		if (IsEnabled())
		{
			if (InputClickMethod != EButtonClickMethod::MouseDown)
			{
				if (bEventOverButton)
				{
					if ((bTriggerForTouchEvent || bTriggerForMouseEvent))
					{
						OnMiddleMouseClicked.ExecuteIfBound();
					}
				}
			}
		}

		//If the user of the button didn't handle this click, then the button's
		//default behavior handles it.
		if (Reply.IsEventHandled() == false)
		{
			Reply = FReply::Handled();
		}
	}

	//If the user hasn't requested a new mouse captor and the button still has mouse capture,
	//then the default behavior of the button is to release mouse capture.
	if (Reply.GetMouseCaptor().IsValid() == false && HasMouseCapture())
	{
		Reply.ReleaseMouseCapture();
	}

	return Reply;
}

FReply SRTSDKButton::OnMouseMove(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	if (IsPressed() && IsPreciseTapOrClick(MouseEvent) && FSlateApplication::Get().HasTraveledFarEnoughToTriggerDrag(MouseEvent, PressedScreenSpacePosition))
	{
		Release();
	}

	return FReply::Unhandled();
}

void SRTSDKButton::OnMouseEnter(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	const bool bWasHovered = IsHovered();

	SBorder::OnMouseEnter(MyGeometry, MouseEvent);

	if (!bWasHovered && IsHovered())
	{
		ExecuteHoverStateChanged(true);
	}
}

void SRTSDKButton::OnMouseLeave(const FPointerEvent& MouseEvent)
{
	const bool bWasHovered = IsHovered();

	// Call parent implementation
	SBorder::OnMouseLeave(MouseEvent);

	// If we're setup to click on mouse-down, then we never capture the mouse and may not receive a
	// mouse up event, so we need to make sure our pressed state is reset properly here
	if (ClickMethod == EButtonClickMethod::MouseDown || IsPreciseTapOrClick(MouseEvent))
	{
		Release();
	}

	if (bWasHovered && !IsHovered())
	{
		ExecuteHoverStateChanged(true);
	}
}

void SRTSDKButton::OnMouseCaptureLost(const FCaptureLostEvent& CaptureLostEvent)
{
	Release();
}

FReply SRTSDKButton::ExecuteOnClick()
{
	if (OnClicked.IsBound())
	{
		FReply Reply = OnClicked.Execute();
#if WITH_ACCESSIBILITY
		// @TODOAccessibility: This should pass the Id of the user that clicked the button but we don't want to change the regular Slate API just yet
		FSlateApplicationBase::Get().GetAccessibleMessageHandler()->OnWidgetEventRaised(FSlateAccessibleMessageHandler::FSlateWidgetAccessibleEventArgs(AsShared(), EAccessibleEvent::Activate));
#endif
		return Reply;
	}
	else
	{
		return FReply::Handled();
	}
}

void SRTSDKButton::Press()
{
	if (!bIsPressed)
	{
		bIsPressed = true;
		PlayPressedSound();
		OnPressed.ExecuteIfBound();
		OnLeftMousePressed.ExecuteIfBound();
		UpdatePressStateChanged();
	}
}

void SRTSDKButton::Release()
{
	if (bIsPressed)
	{
		bIsPressed = false;
		OnReleased.ExecuteIfBound();
		OnLeftMouseReleased.ExecuteIfBound();
		UpdatePressStateChanged();
	}
}

void SRTSDKButton::RightMousePress()
{
	if (!IsRightMousePressed)
	{
		IsRightMousePressed = true;
		PlayPressedSound();
		OnPressed.ExecuteIfBound();
		OnRightMousePressed.ExecuteIfBound();
		UpdatePressStateChanged();
	}
}

void SRTSDKButton::RightMouseRelease()
{
	if (IsRightMousePressed)
	{
		IsRightMousePressed = false;
		OnReleased.ExecuteIfBound();
		OnRightMouseReleased.ExecuteIfBound();
		UpdatePressStateChanged();
	}
}

void SRTSDKButton::MiddleMousePress()
{
	if (!IsMiddleMousePressed)
	{
		IsMiddleMousePressed = true;
		PlayPressedSound();
		OnPressed.ExecuteIfBound();
		OnMiddleMousePressed.ExecuteIfBound();
		UpdatePressStateChanged();
	}
}

void SRTSDKButton::MiddleMouseRelease()
{
	if (IsMiddleMousePressed)
	{
		IsMiddleMousePressed = false;
		OnReleased.ExecuteIfBound();
		OnMiddleMouseReleased.ExecuteIfBound();
		UpdatePressStateChanged();
	}
}

void SRTSDKButton::UpdatePressStateChanged()
{
	UpdatePadding();
	UpdateBorderImage();
	UpdateForegroundColor();
}

bool SRTSDKButton::IsInteractable() const
{
	return IsEnabled();
}

TEnumAsByte<EButtonClickMethod::Type> SRTSDKButton::GetClickMethodFromInputType(const FPointerEvent& MouseEvent) const
{
	if (MouseEvent.IsTouchEvent())
	{
		switch (TouchMethod)
		{
		case EButtonTouchMethod::Down:
			return EButtonClickMethod::MouseDown;
		case EButtonTouchMethod::DownAndUp:
			return EButtonClickMethod::DownAndUp;
		case EButtonTouchMethod::PreciseTap:
			return EButtonClickMethod::PreciseClick;
		}
	}

	return ClickMethod;
}

bool SRTSDKButton::IsPreciseTapOrClick(const FPointerEvent& MouseEvent) const
{
	return GetClickMethodFromInputType(MouseEvent) == EButtonClickMethod::PreciseClick;
}

void SRTSDKButton::PlayPressedSound() const
{
	FSlateApplication::Get().PlaySound(PressedSound);
}

void SRTSDKButton::PlayHoverSound() const
{
	FSlateApplication::Get().PlaySound(HoveredSound);
}

FVector2D SRTSDKButton::ComputeDesiredSize(float LayoutScaleMultiplier) const
{
	// When there is no widget in the button, it sizes itself based on
	// the border image specified by the style.
	if (ChildSlot.GetWidget() == SNullWidget::NullWidget)
	{
		return GetBorderImage()->ImageSize;
	}
	else
	{
		return SBorder::ComputeDesiredSize(LayoutScaleMultiplier);
	}
}

void SRTSDKButton::SetContentPadding(TAttribute<FMargin> InContentPadding)
{
	ContentPaddingAttribute.Assign(*this, MoveTemp(InContentPadding));
}

void SRTSDKButton::SetHoveredSound(TOptional<FSlateSound> InHoveredSound)
{
	HoveredSound = InHoveredSound.Get(Style->HoveredSlateSound);
}

void SRTSDKButton::SetPressedSound(TOptional<FSlateSound> InPressedSound)
{
	PressedSound = InPressedSound.Get(Style->PressedSlateSound);
}

void SRTSDKButton::SetOnClicked(FOnClicked InOnClicked)
{
	OnClicked = InOnClicked;
}

void SRTSDKButton::SetOnHovered(FSimpleDelegate InOnHovered)
{
	OnHovered = InOnHovered;
}

void SRTSDKButton::SetOnUnhovered(FSimpleDelegate InOnUnhovered)
{
	OnUnhovered = InOnUnhovered;
}

void SRTSDKButton::ExecuteHoverStateChanged(bool bPlaySound)
{
	if (IsHovered())
	{
		if (bPlaySound)
		{
			PlayHoverSound();
		}
		OnHovered.ExecuteIfBound();
	}
	else
	{
		OnUnhovered.ExecuteIfBound();
	}
}

void SRTSDKButton::SetButtonStyle(const FButtonStyle* InButtonStyle)
{
	if (InButtonStyle == nullptr)
	{
		ensureAlwaysMsgf(false, TEXT("The Style is not valid."));
		return;
	}

	/* Get pointer to the button Style */
	Style = InButtonStyle;

	HoveredSound = Style->HoveredSlateSound;
	PressedSound = Style->PressedSlateSound;

	UpdatePadding();
	UpdateShowDisabledEffect();
	UpdateBorderImage(); // Must be after UpdateShowDisabledEffect()
	UpdateForegroundColor();
	UpdateDisabledForegroundColor();
}

void SRTSDKButton::SetClickMethod(EButtonClickMethod::Type InClickMethod)
{
	ClickMethod = InClickMethod;
}

void SRTSDKButton::SetTouchMethod(EButtonTouchMethod::Type InTouchMethod)
{
	TouchMethod = InTouchMethod;
}

void SRTSDKButton::SetPressMethod(EButtonPressMethod::Type InPressMethod)
{
	PressMethod = InPressMethod;
}

#if !UE_BUILD_SHIPPING
void SRTSDKButton::SimulateClick()
{
	ExecuteOnClick();
}
#endif // !UE_BUILD_SHIPPING

FSlateColor SRTSDKButton::GetDisabledForegroundColor() const
{
	return Style->DisabledForeground;
}

#if WITH_ACCESSIBILITY
TSharedRef<FSlateAccessibleWidget> SRTSDKButton::CreateAccessibleWidget()
{
	return MakeShareable<FSlateAccessibleWidget>(new FSlateAccessibleButton(SharedThis(this)));
}
#endif
