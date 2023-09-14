// Copyright Epic Games, Inc. All Rights Reserved.
 
#include "Widgets/SRTSDKScrollBar.h"
#include "Widgets/Layout/SScrollBarTrack.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Layout/SSpacer.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Layout/SBox.h"
#include "Framework/Application/SlateApplication.h"

void SRTSDKScrollBar::Construct(const FArguments& InArgs)
{
	OnUserScrolled = InArgs._OnUserScrolled;
	Orientation = InArgs._Orientation;
	DragFocusCause = InArgs._DragFocusCause;
	UserVisibility = InArgs._Visibility;

	check(InArgs._Style);
	SetStyle(InArgs._Style);

	const TAttribute<FVector2D> Thickness = InArgs._Thickness.IsSet() ? InArgs._Thickness : FVector2D(InArgs._Style->Thickness, InArgs._Style->Thickness);

	EHorizontalAlignment HorizontalAlignment = Orientation == Orient_Vertical ? HAlign_Center : HAlign_Fill;
	EVerticalAlignment VerticalAlignment = Orientation == Orient_Vertical ? VAlign_Fill : VAlign_Center;

	MovesNotched = InArgs._MovesNotched;
	NotchOffset = InArgs._NotchOffset;

	bHideWhenNotInUse = InArgs._HideWhenNotInUse;
	bIsScrolling = false;
	LastInteractionTime = 0;

	SBorder::Construct( SBorder::FArguments()
		.BorderImage(FCoreStyle::Get().GetBrush("NoBorder"))
		.Padding(InArgs._Padding)
		[
			SNew(SVerticalBox)

			+ SVerticalBox::Slot()
			.FillHeight(1)
			[
				SNew(SBorder)
				.BorderImage(BackgroundBrush)
				.HAlign(HorizontalAlignment)
				.VAlign(VerticalAlignment)
				.Padding(0)
				[
					SAssignNew(Track, SScrollBarTrack)
					.Orientation(InArgs._Orientation)
					.TopSlot()
					[
						SNew(SBox)
						.HAlign(HorizontalAlignment)
						.VAlign(VerticalAlignment)
						[
							SNew(SImage)
							.ColorAndOpacity(this, &SRTSDKScrollBar::GetTrackOpacity)
							.Image(TopBrush)
						]
					]
					.ThumbSlot()
					[
						SAssignNew(DragThumb, SBorder)
						.BorderImage(this, &SRTSDKScrollBar::GetDragThumbImage)
						.ColorAndOpacity(this, &SRTSDKScrollBar::GetThumbOpacity)
						.HAlign(HAlign_Center)
						.VAlign(VAlign_Center)
						.Padding(0)
						[
							SAssignNew(ThicknessSpacer, SSpacer)
							.Size(InArgs._Thickness)
						]
					]
					.BottomSlot()
					[
						SNew(SBox)
						.HAlign(HorizontalAlignment)
						.VAlign(VerticalAlignment)
						[
							SNew(SImage)
							.ColorAndOpacity(this, &SRTSDKScrollBar::GetTrackOpacity)
							.Image(BottomBrush)
						]
					]
				]
			]
		]
	);

	this->SetEnabled(TAttribute<bool>( Track.ToSharedRef(), &SScrollBarTrack::IsNeeded ));
	SetScrollBarAlwaysVisible(InArgs._AlwaysShowScrollbar);
	bAlwaysShowScrollbarTrack = InArgs._AlwaysShowScrollbarTrack;
}

void SRTSDKScrollBar::SetOnUserScrolled( const FRTSDKOnUserScrolled& InHandler )
{
	OnUserScrolled = InHandler;
}

void SRTSDKScrollBar::SetState( float InOffsetFraction, float InThumbSizeFraction )
{
	if ( Track->DistanceFromTop() != InOffsetFraction || Track->GetThumbSizeFraction() != InThumbSizeFraction )
	{
		// Note that the maximum offset depends on how many items fit per screen
		// It is 1.0f-InThumbSizeFraction.
		Track->SetSizes(InOffsetFraction, InThumbSizeFraction);

		LastInteractionTime = FSlateApplication::Get().GetCurrentTime();
	}
}



void SRTSDKScrollBar::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	SBorder::Tick(AllottedGeometry, InCurrentTime, InDeltaTime);

	DragThumb->SetBorderImage(GetDragThumbImage());
	DragThumb->SetColorAndOpacity(GetThumbOpacity());

	TopImage->SetColorAndOpacity(GetTrackOpacity());
	BottomImage->SetColorAndOpacity(GetTrackOpacity());
}

FReply SRTSDKScrollBar::OnMouseButtonDown( const FGeometry& MyGeometry, const FPointerEvent& MouseEvent )
{
	if (MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton && IsNeeded())
	{
		FGeometry ThumbGeometry = FindChildGeometry(MyGeometry, DragThumb.ToSharedRef());

		if (DragThumb->IsDirectlyHovered())
		{
			// Clicking on the scrollbar drag thumb
			if( Orientation == Orient_Horizontal )
			{
				DragGrabOffset = ThumbGeometry.AbsoluteToLocal( MouseEvent.GetScreenSpacePosition() ).X;
			}
			else
			{
				DragGrabOffset = ThumbGeometry.AbsoluteToLocal( MouseEvent.GetScreenSpacePosition() ).Y;
			}

			bDraggingThumb = true;
		}
		else if (OnUserScrolled.IsBound())
		{
			// Clicking in the non drag thumb area of the scrollbar
			DragGrabOffset = Orientation == Orient_Horizontal ? (ThumbGeometry.GetLocalSize().X * 0.5f) : (ThumbGeometry.GetLocalSize().Y * 0.5f);
			bDraggingThumb = true;

			ExecuteOnUserScrolled( MyGeometry, MouseEvent );
		}
	}

	if( bDraggingThumb )
	{
		return FReply::Handled().CaptureMouse(AsShared()).SetUserFocus(AsShared(), DragFocusCause);
	}
	else
	{
		return FReply::Unhandled();
	}
}

FReply SRTSDKScrollBar::OnMouseButtonUp( const FGeometry& MyGeometry, const FPointerEvent& MouseEvent )
{
	if ( MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton )
	{
		bDraggingThumb = false;
		return FReply::Handled().ReleaseMouseCapture();
	}
	else
	{
		return FReply::Unhandled();
	}
}

FReply SRTSDKScrollBar::OnMouseMove( const FGeometry& MyGeometry, const FPointerEvent& MouseEvent )
{
	if ( this->HasMouseCapture() )
	{
		if (!MouseEvent.GetCursorDelta().IsZero())
		{
			if (OnUserScrolled.IsBound())
			{
				ExecuteOnUserScrolled(MyGeometry, MouseEvent);
			}
			return FReply::Handled();
		}
	}
	
	return FReply::Unhandled();
}

void SRTSDKScrollBar::OnMouseEnter(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	SBorder::OnMouseEnter(MyGeometry, MouseEvent);
	LastInteractionTime = FSlateApplication::Get().GetCurrentTime();
}

void SRTSDKScrollBar::OnMouseLeave(const FPointerEvent& MouseEvent)
{
	SBorder::OnMouseLeave(MouseEvent);
	LastInteractionTime = FSlateApplication::Get().GetCurrentTime();
}

void SRTSDKScrollBar::ExecuteOnUserScrolled( const FGeometry& MyGeometry, const FPointerEvent& MouseEvent )
{
	const int32 AxisId = (Orientation == Orient_Horizontal) ? 0 : 1;
	const FGeometry TrackGeometry = FindChildGeometry( MyGeometry, Track.ToSharedRef() );
	const float UnclampedOffsetInTrack = TrackGeometry.AbsoluteToLocal( MouseEvent.GetScreenSpacePosition() )[ AxisId ] - DragGrabOffset;
	const float TrackSizeBiasedByMinThumbSize = TrackGeometry.GetLocalSize()[ AxisId ] - Track->GetMinThumbSize();
	const float ThumbOffsetInTrack = FMath::Clamp( UnclampedOffsetInTrack, 0.0f, TrackSizeBiasedByMinThumbSize );
	const float ThumbOffset = ThumbOffsetInTrack / TrackSizeBiasedByMinThumbSize;
	if (MovesNotched)
	{
		const float NotchedOffset = GetClosestNotchOffset(ThumbOffset);
		OnUserScrolled.ExecuteIfBound(NotchedOffset);
	}
	else
	{
		OnUserScrolled.ExecuteIfBound(ThumbOffset);
	}
}

bool SRTSDKScrollBar::IsNeeded() const
{
	return Track->IsNeeded();
}

float SRTSDKScrollBar::DistanceFromTop() const
{
	return Track->DistanceFromTop();
}

float SRTSDKScrollBar::DistanceFromBottom() const
{
	return Track->DistanceFromBottom();
}

SRTSDKScrollBar::SRTSDKScrollBar()
	: bDraggingThumb(false)
	, DragGrabOffset( 0.0f )
{
}

FSlateColor SRTSDKScrollBar::GetTrackOpacity() const
{
	if (bDraggingThumb || IsHovered())
	{
		return FLinearColor(1,1,1,1);
	}
	else if ((bAlwaysShowScrollbarTrack  && Track->GetThumbSizeFraction() > KINDA_SMALL_NUMBER) || AlwaysShowScrollbar())
	{
		return FLinearColor(1, 1, 1, 0.5f);
	}
	else
	{
		return FLinearColor(1,1,1,0);
	}
}

FLinearColor SRTSDKScrollBar::GetThumbOpacity() const
{
	if (Track->GetThumbSizeFraction() <= 0.0f)
	{
		return FLinearColor(1, 1, 1, 0);
	}
	else if ( bDraggingThumb || IsHovered() )
	{
		return FLinearColor(1,1,1,1);
	}
	else
	{
		if ( bHideWhenNotInUse && !bAlwaysShowScrollbar )
		{
			const double LastInteractionDelta = bIsScrolling ? 0 : ( FSlateApplication::Get().GetCurrentTime() - LastInteractionTime );

			float ThumbOpacity = FMath::Lerp(1.0f, 0.0f, FMath::Clamp((float)( ( LastInteractionDelta - 0.2 ) / 0.2 ), 0.0f, 1.0f));
			return FLinearColor(1, 1, 1, ThumbOpacity);
		}
		else 
		{
			return FLinearColor(1, 1, 1, 0.75f);
		}
	}
}

void SRTSDKScrollBar::BeginScrolling()
{
	bIsScrolling = true;
}

void SRTSDKScrollBar::EndScrolling()
{
	bIsScrolling = false;
	LastInteractionTime = FSlateApplication::Get().GetCurrentTime();
}

void SRTSDKScrollBar::SetMovesNotched(bool InMovesNotched)
{
	MovesNotched = InMovesNotched;
}

bool SRTSDKScrollBar::GetMovesNotched()
{
	return MovesNotched;
}

void SRTSDKScrollBar::SetNotchOffset(float InNotchOffset)
{
	NotchOffset = InNotchOffset;
}

float SRTSDKScrollBar::GetNotchOffset()
{
	return NotchOffset;
}

float SRTSDKScrollBar::GetClosestNotchOffset(float InRawOffset)
{
	float notches = FMath::RoundToFloat(InRawOffset / NotchOffset);
	return notches * NotchOffset;
}

const FSlateBrush* SRTSDKScrollBar::GetDragThumbImage() const
{
	if ( bDraggingThumb )
	{
		return DraggedThumbImage;
	}
	else if (DragThumb->IsDirectlyHovered())
	{
		return HoveredThumbImage;
	}
	else
	{
		return NormalThumbImage;
	}
}

EVisibility SRTSDKScrollBar::ShouldBeVisible() const
{
	if ( this->HasMouseCapture() )
	{
		return EVisibility::Visible;
	}
	else if( Track->IsNeeded() )
	{
		return UserVisibility.Get();
	}
	else
	{
		return EVisibility::Collapsed;
	}
}

bool SRTSDKScrollBar::IsScrolling() const
{
	return bDraggingThumb;
}

EOrientation SRTSDKScrollBar::GetOrientation() const
{
	return Orientation;
}

void SRTSDKScrollBar::SetStyle(const FScrollBarStyle* InStyle)
{
	const FScrollBarStyle* Style = InStyle;

	if (Style == nullptr)
	{
		FArguments Defaults;
		Style = Defaults._Style;
	}

	check(Style);

	NormalThumbImage = &Style->NormalThumbImage;
	HoveredThumbImage = &Style->HoveredThumbImage;
	DraggedThumbImage = &Style->DraggedThumbImage;

	if (Orientation == Orient_Vertical)
	{
		BackgroundBrush = &Style->VerticalBackgroundImage;
		TopBrush = &Style->VerticalTopSlotImage;
		BottomBrush = &Style->VerticalBottomSlotImage;
	}
	else
	{
		BackgroundBrush = &Style->HorizontalBackgroundImage;
		TopBrush = &Style->HorizontalTopSlotImage;
		BottomBrush = &Style->HorizontalBottomSlotImage;
	}

	Invalidate(EInvalidateWidget::Layout);
}

void SRTSDKScrollBar::SetDragFocusCause(EFocusCause InDragFocusCause)
{
	DragFocusCause = InDragFocusCause;
}

void SRTSDKScrollBar::SetThickness(TAttribute<FVector2D> InThickness)
{
	ThicknessSpacer->SetSize(InThickness);
}

void SRTSDKScrollBar::SetScrollBarAlwaysVisible(bool InAlwaysVisible)
{
	bAlwaysShowScrollbar = InAlwaysVisible;

	if ( InAlwaysVisible )
	{
		SetVisibility(EVisibility::Visible);
	}
	else
	{
		SetVisibility(TAttribute<EVisibility>(SharedThis(this), &SRTSDKScrollBar::ShouldBeVisible));
	}

	Track->SetIsAlwaysVisible(InAlwaysVisible);
}

void SRTSDKScrollBar::SetScrollBarTrackAlwaysVisible(bool InAlwaysVisible)
{
	bAlwaysShowScrollbarTrack = InAlwaysVisible;
	Invalidate(EInvalidateWidget::Layout);
}

bool SRTSDKScrollBar::AlwaysShowScrollbar() const
{
	return bAlwaysShowScrollbar;
}
