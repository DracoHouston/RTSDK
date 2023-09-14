// Copyright Epic Games, Inc. All Rights Reserved.

#include "RTSDKButtonSlot.h"
#include "Widgets/SNullWidget.h"
#include "Widgets/SRTSDKButton.h"
#include "Components/Widget.h"

/////////////////////////////////////////////////////
// UButtonSlot

URTSDKButtonSlot::URTSDKButtonSlot(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
PRAGMA_DISABLE_DEPRECATION_WARNINGS
	Padding = FMargin(4.f, 2.f);

	HorizontalAlignment = HAlign_Center;
	VerticalAlignment = VAlign_Center;
PRAGMA_ENABLE_DEPRECATION_WARNINGS
}

void URTSDKButtonSlot::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);

	Button.Reset();
}

void URTSDKButtonSlot::BuildSlot(TSharedRef<SRTSDKButton> InButton)
{
	Button = InButton;

PRAGMA_DISABLE_DEPRECATION_WARNINGS
	InButton->SetContentPadding(Padding);
	InButton->SetHAlign(HorizontalAlignment);
	InButton->SetVAlign(VerticalAlignment);
PRAGMA_ENABLE_DEPRECATION_WARNINGS

	InButton->SetContent(Content ? Content->TakeWidget() : SNullWidget::NullWidget);
}

PRAGMA_DISABLE_DEPRECATION_WARNINGS
FMargin URTSDKButtonSlot::GetPadding() const
{
	return Padding;
}

void URTSDKButtonSlot::SetPadding(FMargin InPadding)
{
	Padding = InPadding;
	if ( Button.IsValid() )
	{
		Button.Pin()->SetContentPadding(InPadding);
	}
}

EHorizontalAlignment URTSDKButtonSlot::GetHorizontalAlignment() const
{
	return HorizontalAlignment;
}

void URTSDKButtonSlot::SetHorizontalAlignment(EHorizontalAlignment InHorizontalAlignment)
{
	HorizontalAlignment = InHorizontalAlignment;
	if ( Button.IsValid() )
	{
		Button.Pin()->SetHAlign(InHorizontalAlignment);
	}
}

EVerticalAlignment URTSDKButtonSlot::GetVerticalAlignment() const
{
	return VerticalAlignment;
}

void URTSDKButtonSlot::SetVerticalAlignment(EVerticalAlignment InVerticalAlignment)
{
	VerticalAlignment = InVerticalAlignment;
	if ( Button.IsValid() )
	{
		Button.Pin()->SetVAlign(InVerticalAlignment);
	}
}
PRAGMA_ENABLE_DEPRECATION_WARNINGS

void URTSDKButtonSlot::SynchronizeProperties()
{
PRAGMA_DISABLE_DEPRECATION_WARNINGS
	SetPadding(Padding);
	SetHorizontalAlignment(HorizontalAlignment);
	SetVerticalAlignment(VerticalAlignment);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
}
