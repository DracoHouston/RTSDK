// Copyright Epic Games, Inc. All Rights Reserved.

#include "RTSDKScrollBox.h"
#include "RTSDKScrollBoxSlot.h"
#include "UObject/EditorObjectVersion.h"
#include "Styling/UMGCoreStyle.h"

#define LOCTEXT_NAMESPACE "UMG"

/////////////////////////////////////////////////////
// URTSDKScrollBox

static FScrollBoxStyle* DefaultRTSUMGScrollBoxStyle = nullptr;
static FScrollBarStyle* DefaultRTSUMGScrollBoxBarStyle = nullptr;

/////////////////////////////////////////////////////
// UScrollBox

static FScrollBoxStyle* DefaultScrollBoxStyle = nullptr;
static FScrollBarStyle* DefaultScrollBoxBarStyle = nullptr;

#if WITH_EDITOR
static FScrollBoxStyle* EditorScrollBoxStyle = nullptr;
static FScrollBarStyle* EditorScrollBoxBarStyle = nullptr;
#endif 

URTSDKScrollBox::URTSDKScrollBox(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, Orientation(Orient_Vertical)
	, ScrollBarVisibility(ESlateVisibility::Visible)
	, ConsumeMouseWheel(EConsumeMouseWheel::WhenScrollingPossible)
	, ScrollbarThickness(9.0f, 9.0f)
	, ScrollbarPadding(2.0f)
	, AlwaysShowScrollbar(false)
	, AlwaysShowScrollbarTrack(false)
	, AllowOverscroll(true)
	, BackPadScrolling(false)
	, FrontPadScrolling(false)
	, NavigationDestination(ERTSDKDescendantScrollDestination::IntoView)
	, NavigationScrollPadding(0.0f)
	, ScrollWhenFocusChanges(ERTSDKScrollWhenFocusChanges::NoScroll)
	, NotchDistance(0.0f)
	, MovesNotched(false)
{
	bIsVariable = false;

	SetVisibilityInternal(ESlateVisibility::Visible);
	SetClipping(EWidgetClipping::ClipToBounds);

	if (DefaultScrollBoxStyle == nullptr)
	{
		DefaultScrollBoxStyle = new FScrollBoxStyle(FUMGCoreStyle::Get().GetWidgetStyle<FScrollBoxStyle>("ScrollBox"));

		// Unlink UMG default colors.
		DefaultScrollBoxStyle->UnlinkColors();
	}

	if (DefaultScrollBoxBarStyle == nullptr)
	{
		DefaultScrollBoxBarStyle = new FScrollBarStyle(FUMGCoreStyle::Get().GetWidgetStyle<FScrollBarStyle>("ScrollBar"));

		// Unlink UMG default colors.
		DefaultScrollBoxBarStyle->UnlinkColors();
	}

	WidgetStyle = *DefaultScrollBoxStyle;
	WidgetBarStyle = *DefaultScrollBoxBarStyle;

#if WITH_EDITOR 
	if (EditorScrollBoxStyle == nullptr)
	{
		EditorScrollBoxStyle = new FScrollBoxStyle(FCoreStyle::Get().GetWidgetStyle<FScrollBoxStyle>("ScrollBox"));

		// Unlink UMG Editor colors from the editor settings colors.
		EditorScrollBoxStyle->UnlinkColors();
	}

	if (EditorScrollBoxBarStyle == nullptr)
	{
		EditorScrollBoxBarStyle = new FScrollBarStyle(FCoreStyle::Get().GetWidgetStyle<FScrollBarStyle>("ScrollBar"));

		// Unlink UMG Editor colors from the editor settings colors.
		EditorScrollBoxBarStyle->UnlinkColors();
	}

	if (IsEditorWidget())
	{
		WidgetStyle = *EditorScrollBoxStyle;
		WidgetBarStyle = *EditorScrollBoxBarStyle;

		// The CDO isn't an editor widget and thus won't use the editor style, call post edit change to mark difference from CDO
		PostEditChange();
	}
#endif // WITH_EDITOR

	bAllowRightClickDragScrolling = true;
}

void URTSDKScrollBox::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);

	MyScrollBox.Reset();
}

UClass* URTSDKScrollBox::GetSlotClass() const
{
	return URTSDKScrollBoxSlot::StaticClass();
}

void URTSDKScrollBox::OnSlotAdded(UPanelSlot* InSlot)
{
	// Add the child to the live canvas if it already exists
	if (MyScrollBox.IsValid())
	{
		CastChecked<URTSDKScrollBoxSlot>(InSlot)->BuildSlot(MyScrollBox.ToSharedRef());
	}
}

void URTSDKScrollBox::OnSlotRemoved(UPanelSlot* InSlot)
{
	// Remove the widget from the live slot if it exists.
	if (MyScrollBox.IsValid() && InSlot->Content)
	{
		const TSharedPtr<SWidget> Widget = InSlot->Content->GetCachedWidget();
		if (Widget.IsValid())
		{
			MyScrollBox->RemoveSlot(Widget.ToSharedRef());
		}
	}
}

TSharedRef<SWidget> URTSDKScrollBox::RebuildWidget()
{
	MyScrollBox = SNew(SRTSDKScrollBox)
		.Style(&WidgetStyle)
		.ScrollBarStyle(&WidgetBarStyle)
		.Orientation(Orientation)
		.ConsumeMouseWheel(ConsumeMouseWheel)
		.NavigationDestination(NavigationDestination)
		.NavigationScrollPadding(NavigationScrollPadding)
		.ScrollWhenFocusChanges(ScrollWhenFocusChanges)
		.BackPadScrolling(BackPadScrolling)
		.FrontPadScrolling(FrontPadScrolling)
		.AnimateWheelScrolling(bAnimateWheelScrolling)
		.WheelScrollMultiplier(WheelScrollMultiplier)
		.OnUserScrolled(BIND_UOBJECT_DELEGATE(FRTSDKOnUserScrolled, SlateHandleUserScrolled))
		.MovesNotched(MovesNotched)
		.NotchDistance(NotchDistance);

	for (UPanelSlot* PanelSlot : Slots)
	{
		if (URTSDKScrollBoxSlot* TypedSlot = Cast<URTSDKScrollBoxSlot>(PanelSlot))
		{
			TypedSlot->Parent = this;
			TypedSlot->BuildSlot(MyScrollBox.ToSharedRef());
		}
	}

	return MyScrollBox.ToSharedRef();
}

void URTSDKScrollBox::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	MyScrollBox->SetScrollOffset(DesiredScrollOffset);
	MyScrollBox->SetOrientation(Orientation);
	MyScrollBox->SetScrollBarVisibility(UWidget::ConvertSerializedVisibilityToRuntime(ScrollBarVisibility));
	MyScrollBox->SetScrollBarThickness(ScrollbarThickness);
	MyScrollBox->SetScrollBarPadding(ScrollbarPadding);
	MyScrollBox->SetScrollBarAlwaysVisible(AlwaysShowScrollbar);
	MyScrollBox->SetScrollBarTrackAlwaysVisible(AlwaysShowScrollbarTrack);
	MyScrollBox->SetAllowOverscroll(AllowOverscroll ? EAllowOverscroll::Yes : EAllowOverscroll::No);
	MyScrollBox->SetScrollBarRightClickDragAllowed(bAllowRightClickDragScrolling);
	MyScrollBox->SetConsumeMouseWheel(ConsumeMouseWheel);
	MyScrollBox->SetAnimateWheelScrolling(bAnimateWheelScrolling);
	MyScrollBox->SetWheelScrollMultiplier(WheelScrollMultiplier);
	MyScrollBox->SetMovesNotched(MovesNotched);
	MyScrollBox->SetNotchDistance(NotchDistance);
}

float URTSDKScrollBox::GetScrollOffset() const
{
	if (MyScrollBox.IsValid())
	{
		return MyScrollBox->GetScrollOffset();
	}

	return 0;
}

float URTSDKScrollBox::GetScrollOffsetOfEnd() const
{
	if (MyScrollBox.IsValid())
	{
		return MyScrollBox->GetScrollOffsetOfEnd();
	}

	return 0;
}

float URTSDKScrollBox::GetViewFraction() const
{
	if (MyScrollBox.IsValid())
	{
		return MyScrollBox->GetViewFraction();
	}

	return 0;
}

float URTSDKScrollBox::GetViewOffsetFraction() const
{
	if (MyScrollBox.IsValid())
	{
		return MyScrollBox->GetViewOffsetFraction();
	}

	return 0;
}

void URTSDKScrollBox::SetScrollOffset(float NewScrollOffset)
{
	DesiredScrollOffset = NewScrollOffset;

	if (MyScrollBox.IsValid())
	{
		MyScrollBox->SetScrollOffset(NewScrollOffset);
	}
}

void URTSDKScrollBox::ScrollToStart()
{
	if (MyScrollBox.IsValid())
	{
		MyScrollBox->ScrollToStart();
	}
}

void URTSDKScrollBox::ScrollToEnd()
{
	if (MyScrollBox.IsValid())
	{
		MyScrollBox->ScrollToEnd();
	}
}

void URTSDKScrollBox::ScrollWidgetIntoView(UWidget* WidgetToFind, bool AnimateScroll, ERTSDKDescendantScrollDestination InScrollDestination, float Padding)
{
	TSharedPtr<SWidget> SlateWidgetToFind;
	if (WidgetToFind)
	{
		SlateWidgetToFind = WidgetToFind->GetCachedWidget();
	}

	if (MyScrollBox.IsValid())
	{
		// NOTE: Pass even if null! This, in effect, cancels a request to scroll which is necessary to avoid warnings/ensures 
		//       when we request to scroll to a widget and later remove that widget!
		MyScrollBox->ScrollDescendantIntoView(SlateWidgetToFind, AnimateScroll, InScrollDestination, Padding);
	}
}

#if WITH_EDITORONLY_DATA

void URTSDKScrollBox::Serialize(FArchive& Ar)
{
	Ar.UsingCustomVersion(FEditorObjectVersion::GUID);

	const bool bDeprecateThickness = Ar.IsLoading() && Ar.CustomVer(FEditorObjectVersion::GUID) < FEditorObjectVersion::ScrollBarThicknessChange;
	if (bDeprecateThickness)
	{
		// Set ScrollbarThickness property to previous default value.
		ScrollbarThickness.Set(5.0f, 5.0f);
	}

	Super::Serialize(Ar);

	if (bDeprecateThickness)
	{
		// Implicit padding of 2 was removed, so ScrollbarThickness value must be incremented by 4.
		ScrollbarThickness += FVector2D(4.0f, 4.0f);
	}
}

#endif // if WITH_EDITORONLY_DATA

void URTSDKScrollBox::SetNavigationDestination(const ERTSDKDescendantScrollDestination NewNavigationDestination)
{
	NavigationDestination = NewNavigationDestination;

	if (MyScrollBox.IsValid())
	{
		MyScrollBox->SetNavigationDestination(NewNavigationDestination);
	}
}

void URTSDKScrollBox::SetConsumeMouseWheel(EConsumeMouseWheel NewConsumeMouseWheel)
{
	ConsumeMouseWheel = NewConsumeMouseWheel;

	if (MyScrollBox.IsValid())
	{
		MyScrollBox->SetConsumeMouseWheel(NewConsumeMouseWheel);
	}
}

void URTSDKScrollBox::SetOrientation(EOrientation NewOrientation)
{
	Orientation = NewOrientation;

	if (MyScrollBox.IsValid())
	{
		MyScrollBox->SetOrientation(Orientation);
	}
}

void URTSDKScrollBox::SetScrollBarVisibility(ESlateVisibility NewScrollBarVisibility)
{
	ScrollBarVisibility = NewScrollBarVisibility;

	if (MyScrollBox.IsValid())
	{
		switch (ScrollBarVisibility)
		{
		case ESlateVisibility::Collapsed:				MyScrollBox->SetScrollBarVisibility(EVisibility::Collapsed); break;
		case ESlateVisibility::Hidden:					MyScrollBox->SetScrollBarVisibility(EVisibility::Hidden); break;
		case ESlateVisibility::HitTestInvisible:		MyScrollBox->SetScrollBarVisibility(EVisibility::HitTestInvisible); break;
		case ESlateVisibility::SelfHitTestInvisible:	MyScrollBox->SetScrollBarVisibility(EVisibility::SelfHitTestInvisible); break;
		case ESlateVisibility::Visible:					MyScrollBox->SetScrollBarVisibility(EVisibility::Visible); break;
		}
	}
}

void URTSDKScrollBox::SetScrollbarThickness(const FVector2D& NewScrollbarThickness)
{
	ScrollbarThickness = NewScrollbarThickness;

	if (MyScrollBox.IsValid())
	{
		MyScrollBox->SetScrollBarThickness(ScrollbarThickness);
	}
}

void URTSDKScrollBox::SetScrollbarPadding(const FMargin& NewScrollbarPadding)
{
	ScrollbarPadding = NewScrollbarPadding;

	if (MyScrollBox.IsValid())
	{
		MyScrollBox->SetScrollBarPadding(ScrollbarPadding);
	}
}

void URTSDKScrollBox::SetAlwaysShowScrollbar(bool NewAlwaysShowScrollbar)
{
	AlwaysShowScrollbar = NewAlwaysShowScrollbar;

	if (MyScrollBox.IsValid())
	{
		MyScrollBox->SetScrollBarAlwaysVisible(AlwaysShowScrollbar);
	}
}

void URTSDKScrollBox::SetAllowOverscroll(bool NewAllowOverscroll)
{
	AllowOverscroll = NewAllowOverscroll;

	if (MyScrollBox.IsValid())
	{
		MyScrollBox->SetAllowOverscroll(AllowOverscroll ? EAllowOverscroll::Yes : EAllowOverscroll::No);
	}
}

void URTSDKScrollBox::SetAnimateWheelScrolling(bool bShouldAnimateWheelScrolling)
{
	bAnimateWheelScrolling = bShouldAnimateWheelScrolling;
	if (MyScrollBox)
	{
		MyScrollBox->SetAnimateWheelScrolling(bShouldAnimateWheelScrolling);
	}
}

void URTSDKScrollBox::SetWheelScrollMultiplier(float NewWheelScrollMultiplier)
{
	WheelScrollMultiplier = NewWheelScrollMultiplier;
	if (MyScrollBox)
	{
		MyScrollBox->SetWheelScrollMultiplier(NewWheelScrollMultiplier);
	}
}

void URTSDKScrollBox::SetScrollWhenFocusChanges(ERTSDKScrollWhenFocusChanges NewScrollWhenFocusChanges)
{
	ScrollWhenFocusChanges = NewScrollWhenFocusChanges;
	if (MyScrollBox)
	{
		MyScrollBox->SetScrollWhenFocusChanges(NewScrollWhenFocusChanges);
	}
}

void URTSDKScrollBox::EndInertialScrolling()
{
	if (MyScrollBox.IsValid())
	{
		MyScrollBox->EndInertialScrolling();
	}
}

void URTSDKScrollBox::SetNotchDistance(float InNotchDistance)
{
	NotchDistance = InNotchDistance;
	if (MyScrollBox.IsValid())
	{
		MyScrollBox->SetNotchDistance(InNotchDistance);
	}
}

float URTSDKScrollBox::GetNotchDistance() const
{
	if (MyScrollBox.IsValid())
	{
		return MyScrollBox->GetNotchDistance();
	}
	return NotchDistance;
}

void URTSDKScrollBox::SetMovesNotched(bool InMovesNotched)
{
	MovesNotched = InMovesNotched;
	if (MyScrollBox.IsValid())
	{
		MyScrollBox->SetMovesNotched(InMovesNotched);
	}
}

bool URTSDKScrollBox::GetMovesNotched() const
{
	if (MyScrollBox.IsValid())
	{
		return MyScrollBox->GetMovesNotched();
	}
	return MovesNotched;
}

void URTSDKScrollBox::SlateHandleUserScrolled(float CurrentOffset)
{
	OnUserScrolled.Broadcast(CurrentOffset);
}

#if WITH_EDITOR

const FText URTSDKScrollBox::GetPaletteCategory()
{
	return LOCTEXT("Panel", "Panel");
}

void URTSDKScrollBox::OnDescendantSelectedByDesigner(UWidget* DescendantWidget)
{
	UWidget* SelectedChild = UWidget::FindChildContainingDescendant(this, DescendantWidget);
	if (SelectedChild)
	{
		ScrollWidgetIntoView(SelectedChild, true);

		if (TickHandle.IsValid())
		{
			FTSTicker::GetCoreTicker().RemoveTicker(TickHandle);
			TickHandle.Reset();
		}
	}
}

void URTSDKScrollBox::OnDescendantDeselectedByDesigner(UWidget* DescendantWidget)
{
	if (TickHandle.IsValid())
	{
		FTSTicker::GetCoreTicker().RemoveTicker(TickHandle);
		TickHandle.Reset();
	}

	// because we get a deselect before we get a select, we need to delay this call until we're sure we didn't scroll to another widget.
	TickHandle = FTSTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateLambda(
		[=](float) -> bool
		{
			QUICK_SCOPE_CYCLE_COUNTER(STAT_UScrollBox_ScrollToStart_LambdaTick);
			this->ScrollToStart();
			return false;
		}
	));
}

#endif

/////////////////////////////////////////////////////

#undef LOCTEXT_NAMESPACE
