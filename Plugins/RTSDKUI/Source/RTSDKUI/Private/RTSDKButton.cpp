// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSDKButton.h"
#include "Widgets/SRTSDKButton.h"
#include "Widgets/SNullWidget.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Components/ButtonSlot.h"
#include "Styling/UMGCoreStyle.h"
#include "Blueprint/WidgetTree.h"
#include "RTSDKButtonSlot.h"

#define LOCTEXT_NAMESPACE "UMG"

/////////////////////////////////////////////////////
// UButton

static FButtonStyle* DefaultButtonStyle = nullptr;

#if WITH_EDITOR
static FButtonStyle* EditorButtonStyle = nullptr;
#endif 

URTSDKButton::URTSDKButton(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	if (DefaultButtonStyle == nullptr)
	{
		DefaultButtonStyle = new FButtonStyle(FUMGCoreStyle::Get().GetWidgetStyle<FButtonStyle>("Button"));

		// Unlink UMG default colors.
		DefaultButtonStyle->UnlinkColors();
	}

	WidgetStyle = *DefaultButtonStyle;

#if WITH_EDITOR 
	if (EditorButtonStyle == nullptr)
	{
		EditorButtonStyle = new FButtonStyle(FCoreStyle::Get().GetWidgetStyle<FButtonStyle>("EditorUtilityButton"));

		// Unlink UMG Editor colors from the editor settings colors.
		EditorButtonStyle->UnlinkColors();
	}

	if (IsEditorWidget())
	{
		WidgetStyle = *EditorButtonStyle;

		// The CDO isn't an editor widget and thus won't use the editor style, call post edit change to mark difference from CDO
		PostEditChange();
	}
#endif // WITH_EDITOR

	ColorAndOpacity = FLinearColor::White;
	BackgroundColor = FLinearColor::White;

	ClickMethod = EButtonClickMethod::DownAndUp;
	TouchMethod = EButtonTouchMethod::DownAndUp;

	IsFocusable = true;

#if WITH_EDITORONLY_DATA
	AccessibleBehavior = ESlateAccessibleBehavior::Summary;
	bCanChildrenBeAccessible = false;
#endif
}

void URTSDKButton::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);

	MyButton.Reset();
}

TSharedRef<SWidget> URTSDKButton::RebuildWidget()
{
	MyButton = SNew(SRTSDKButton)
		.OnClicked(BIND_UOBJECT_DELEGATE(FOnClicked, SlateHandleClicked))
		.OnPressed(BIND_UOBJECT_DELEGATE(FSimpleDelegate, SlateHandlePressed))
		.OnReleased(BIND_UOBJECT_DELEGATE(FSimpleDelegate, SlateHandleReleased))
		.OnLeftMouseClicked(BIND_UOBJECT_DELEGATE(FSimpleDelegate, SlateHandleLeftMouseClicked))
		.OnLeftMousePressed(BIND_UOBJECT_DELEGATE(FSimpleDelegate, SlateHandleLeftMousePressed))
		.OnLeftMouseReleased(BIND_UOBJECT_DELEGATE(FSimpleDelegate, SlateHandleLeftMouseReleased))
		.OnRightMouseClicked(BIND_UOBJECT_DELEGATE(FSimpleDelegate, SlateHandleRightMouseClicked))
		.OnRightMousePressed(BIND_UOBJECT_DELEGATE(FSimpleDelegate, SlateHandleRightMousePressed))
		.OnRightMouseReleased(BIND_UOBJECT_DELEGATE(FSimpleDelegate, SlateHandleRightMouseReleased))
		.OnMiddleMouseClicked(BIND_UOBJECT_DELEGATE(FSimpleDelegate, SlateHandleMiddleMouseClicked))
		.OnMiddleMousePressed(BIND_UOBJECT_DELEGATE(FSimpleDelegate, SlateHandleMiddleMousePressed))
		.OnMiddleMouseReleased(BIND_UOBJECT_DELEGATE(FSimpleDelegate, SlateHandleMiddleMouseReleased))
		.OnHovered_UObject(this, &URTSDKButton::SlateHandleHovered)
		.OnUnhovered_UObject(this, &URTSDKButton::SlateHandleUnhovered)
		.ButtonStyle(&WidgetStyle)
		.ClickMethod(ClickMethod)
		.TouchMethod(TouchMethod)
		.PressMethod(PressMethod)
		.IsFocusable(IsFocusable);

	if (GetChildrenCount() > 0)
	{
		Cast<URTSDKButtonSlot>(GetContentSlot())->BuildSlot(MyButton.ToSharedRef());
	}

	return MyButton.ToSharedRef();
}

void URTSDKButton::SlateHandleLeftMouseClicked()
{
	OnLeftMouseClicked.Broadcast();
}

void URTSDKButton::SlateHandleLeftMousePressed()
{
	OnLeftMousePressed.Broadcast();
}

void URTSDKButton::SlateHandleLeftMouseReleased()
{
	OnLeftMouseReleased.Broadcast();
}

void URTSDKButton::SlateHandleRightMouseClicked()
{
	OnRightMouseClicked.Broadcast();
}

void URTSDKButton::SlateHandleRightMousePressed()
{
	OnRightMousePressed.Broadcast();
}

void URTSDKButton::SlateHandleRightMouseReleased()
{
	OnRightMouseReleased.Broadcast();
}

void URTSDKButton::SlateHandleMiddleMouseClicked()
{
	OnMiddleMouseClicked.Broadcast();
}

void URTSDKButton::SlateHandleMiddleMousePressed()
{
	OnMiddleMousePressed.Broadcast();
}

void URTSDKButton::SlateHandleMiddleMouseReleased()
{
	OnMiddleMouseReleased.Broadcast();
}

void URTSDKButton::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	MyButton->SetColorAndOpacity(ColorAndOpacity);
	MyButton->SetBorderBackgroundColor(BackgroundColor);
}

UClass* URTSDKButton::GetSlotClass() const
{
	return URTSDKButtonSlot::StaticClass();
}

void URTSDKButton::OnSlotAdded(UPanelSlot* InSlot)
{
	// Add the child to the live slot if it already exists
	if (MyButton.IsValid())
	{
		CastChecked<URTSDKButtonSlot>(InSlot)->BuildSlot(MyButton.ToSharedRef());
	}
}

void URTSDKButton::OnSlotRemoved(UPanelSlot* InSlot)
{
	// Remove the widget from the live slot if it exists.
	if (MyButton.IsValid())
	{
		MyButton->SetContent(SNullWidget::NullWidget);
	}
}

void URTSDKButton::SetStyle(const FButtonStyle& InStyle)
{
	WidgetStyle = InStyle;
	if (MyButton.IsValid())
	{
		MyButton->SetButtonStyle(&WidgetStyle);
	}
}

void URTSDKButton::SetColorAndOpacity(FLinearColor InColorAndOpacity)
{
	ColorAndOpacity = InColorAndOpacity;
	if (MyButton.IsValid())
	{
		MyButton->SetColorAndOpacity(InColorAndOpacity);
	}
}

void URTSDKButton::SetBackgroundColor(FLinearColor InBackgroundColor)
{
	BackgroundColor = InBackgroundColor;
	if (MyButton.IsValid())
	{
		MyButton->SetBorderBackgroundColor(InBackgroundColor);
	}
}

bool URTSDKButton::IsPressed() const
{
	if (MyButton.IsValid())
	{
		return MyButton->IsPressed();
	}

	return false;
}

void URTSDKButton::SetClickMethod(EButtonClickMethod::Type InClickMethod)
{
	ClickMethod = InClickMethod;
	if (MyButton.IsValid())
	{
		MyButton->SetClickMethod(ClickMethod);
	}
}

void URTSDKButton::SetTouchMethod(EButtonTouchMethod::Type InTouchMethod)
{
	TouchMethod = InTouchMethod;
	if (MyButton.IsValid())
	{
		MyButton->SetTouchMethod(TouchMethod);
	}
}

void URTSDKButton::SetPressMethod(EButtonPressMethod::Type InPressMethod)
{
	PressMethod = InPressMethod;
	if (MyButton.IsValid())
	{
		MyButton->SetPressMethod(PressMethod);
	}
}

void URTSDKButton::PostLoad()
{
	Super::PostLoad();

	if (GetChildrenCount() > 0)
	{
		//TODO UMG Pre-Release Upgrade, now buttons have slots of their own.  Convert existing slot to new slot.
		if (UPanelSlot* PanelSlot = GetContentSlot())
		{
			URTSDKButtonSlot* ButtonSlot = Cast<URTSDKButtonSlot>(PanelSlot);
			if (ButtonSlot == NULL)
			{
				ButtonSlot = NewObject<URTSDKButtonSlot>(this);
				ButtonSlot->Content = GetContentSlot()->Content;
				ButtonSlot->Content->Slot = ButtonSlot;
				Slots[0] = ButtonSlot;
			}
		}
	}
}

FReply URTSDKButton::SlateHandleClicked()
{
	OnClicked.Broadcast();

	return FReply::Handled();
}

void URTSDKButton::SlateHandlePressed()
{
	OnPressed.Broadcast();
}

void URTSDKButton::SlateHandleReleased()
{
	OnReleased.Broadcast();
}

void URTSDKButton::SlateHandleHovered()
{
	OnHovered.Broadcast();
}

void URTSDKButton::SlateHandleUnhovered()
{
	OnUnhovered.Broadcast();
}

#if WITH_ACCESSIBILITY
TSharedPtr<SWidget> URTSDKButton::GetAccessibleWidget() const
{
	return MyButton;
}
#endif

#if WITH_EDITOR

const FText URTSDKButton::GetPaletteCategory()
{
	return LOCTEXT("Common", "Common");
}

#endif

/////////////////////////////////////////////////////

#undef LOCTEXT_NAMESPACE