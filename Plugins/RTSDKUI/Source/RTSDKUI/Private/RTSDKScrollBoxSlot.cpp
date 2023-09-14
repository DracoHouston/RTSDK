// Copyright Epic Games, Inc. All Rights Reserved.

#include "RTSDKScrollBoxSlot.h"
#include "Components/Widget.h"

/////////////////////////////////////////////////////
// URTSDKScrollBoxSlot

URTSDKScrollBoxSlot::URTSDKScrollBoxSlot(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, Slot(nullptr)
{
PRAGMA_DISABLE_DEPRECATION_WARNINGS
	HorizontalAlignment = HAlign_Fill;
	VerticalAlignment = VAlign_Fill;
PRAGMA_ENABLE_DEPRECATION_WARNINGS
}

void URTSDKScrollBoxSlot::BuildSlot(TSharedRef<SRTSDKScrollBox> ScrollBox)
{
	PRAGMA_DISABLE_DEPRECATION_WARNINGS
		ScrollBox->AddSlot()
		.Padding(Padding)
		.HAlign(HorizontalAlignment)
		.VAlign(VerticalAlignment)
		.Expose(Slot)
		[
			Content == nullptr ? SNullWidget::NullWidget : Content->TakeWidget()
		];
	PRAGMA_ENABLE_DEPRECATION_WARNINGS
}

PRAGMA_DISABLE_DEPRECATION_WARNINGS
FMargin URTSDKScrollBoxSlot::GetPadding() const
{
	return Slot ? Slot->GetPadding() : Padding;
}

void URTSDKScrollBoxSlot::SetPadding(FMargin InPadding)
{
	Padding = InPadding;
	if (Slot)
	{
		Slot->SetPadding(InPadding);
	}
}

EHorizontalAlignment URTSDKScrollBoxSlot::GetHorizontalAlignment() const
{
	return Slot ? Slot->GetHorizontalAlignment() : HorizontalAlignment.GetValue();
}

void URTSDKScrollBoxSlot::SetHorizontalAlignment(EHorizontalAlignment InHorizontalAlignment)
{
	HorizontalAlignment = InHorizontalAlignment;
	if (Slot)
	{
		Slot->SetHorizontalAlignment(InHorizontalAlignment);
	}
}

EVerticalAlignment URTSDKScrollBoxSlot::GetVerticalAlignment() const
{
	return Slot ? Slot->GetVerticalAlignment() : VerticalAlignment.GetValue();
}

void URTSDKScrollBoxSlot::SetVerticalAlignment(EVerticalAlignment InVerticalAlignment)
{
	VerticalAlignment = InVerticalAlignment;
	if (Slot)
	{
		Slot->SetVerticalAlignment(InVerticalAlignment);
	}
}
PRAGMA_ENABLE_DEPRECATION_WARNINGS

void URTSDKScrollBoxSlot::SynchronizeProperties()
{
	PRAGMA_DISABLE_DEPRECATION_WARNINGS
		SetPadding(Padding);
	SetHorizontalAlignment(HorizontalAlignment);
	SetVerticalAlignment(VerticalAlignment);
	PRAGMA_ENABLE_DEPRECATION_WARNINGS
}

void URTSDKScrollBoxSlot::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);
	Slot = nullptr;
}
