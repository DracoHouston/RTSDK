// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSDKHUDPanel.h"
#include "RTSDKHUDElementPanel.h"
#include "Components/CanvasPanel.h"
#include "Components/PanelSlot.h"

void URTSDKHUDPanel::OnWidgetRebuilt()
{
	Super::OnWidgetRebuilt();
	if (HUDCanvas == nullptr)
	{
		return;
	}
	
	const TArray<UPanelSlot*> hudcanvasslots = HUDCanvas->GetSlots();
	HUDElements.Empty(hudcanvasslots.Num());
	for (int32 i = 0; i < hudcanvasslots.Num(); i++)
	{
		URTSDKHUDElementPanel* hudelement = Cast<URTSDKHUDElementPanel>(hudcanvasslots[i]->Content);
		if (hudelement != nullptr)
		{
			HUDElements.Add(hudelement->HUDElementName, hudelement);
		}
	}
}
