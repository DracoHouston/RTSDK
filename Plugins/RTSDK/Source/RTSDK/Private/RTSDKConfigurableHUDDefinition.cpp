// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSDKConfigurableHUDDefinition.h"
#include "RTSDKGameModDefinition.h"
#include "RTSDKHUDLayoutData.h"
#include "RTSDKModManager.h"
#include "Blueprint/UserWidget.h"

void URTSDKConfigurableHUDDefinition::Init(FName inDevName, const FText& inDisplayName, const FText& inDescriptionText, FName inAssociatedGameModDevName, FName inParentHUDDevName, TSoftClassPtr<UUserWidget> inWidgetClass, TArray<FRTSDKInitialHUDElementSettings> inElements, bool inbIsAbstractHUD)
{
	HUDDevName = inDevName;
	HUDDisplayName = inDisplayName;
	HUDDescriptionText = inDescriptionText;
	AssociatedGameModDevName = inAssociatedGameModDevName;
	ParentHUDDevName = inParentHUDDevName;
	WidgetClass = inWidgetClass;
	Elements = inElements;
	bIsAbstractHUD = inbIsAbstractHUD;
	bIsValid = false;
}

void URTSDKConfigurableHUDDefinition::BuildModDependencies(URTSDKModManager* inModManager)
{
	URTSDKGameModDefinition* gamemod = inModManager->GetGameModByName(AssociatedGameModDevName);
	if (gamemod != nullptr)
	{
		AssociatedGameMod = gamemod;
	}
	else
	{
		bIsValid = false;
		return;
	}
	if (!ParentHUDDevName.IsNone())
	{
		URTSDKConfigurableHUDDefinition* parent = inModManager->GetConfigurableHUDByName(ParentHUDDevName);
		if (parent != nullptr)
		{
			ParentHUD = parent;
			bIsValid = true;
			return;
		}
		else
		{
			bIsValid = false;
			return;
		}
	}
	else
	{
		bIsValid = true;
		return;
	}
}

void URTSDKConfigurableHUDDefinition::BuildMod(URTSDKModManager* inModManager)
{
	if (!bIsValid)
	{
		return;
	}

	TArray<URTSDKConfigurableHUDDefinition*> allparents;
	URTSDKConfigurableHUDDefinition* currentouter = ParentHUD;
	while (currentouter != nullptr)
	{
		if (!currentouter->bIsValid)
		{
			bIsValid = false;//bad parent
			return;
		}
		if (allparents.Contains(currentouter))
		{
			bIsValid = false;//circular dependency
			return;
		}
		allparents.Add(currentouter);
		currentouter = currentouter->ParentHUD;
	}
	CombinedElements.Empty(Elements.Num());
	TArray<FName> elementnames;
	elementnames.Empty(Elements.Num());
	for (int32 i = 0; i < Elements.Num(); i++)
	{
		if (!elementnames.Contains(Elements[i].ElementName))
		{
			elementnames.Add(Elements[i].ElementName);
			CombinedElements.Add(Elements[i]);
		}		
	}

	for (int32 i = 0; i < allparents.Num(); i++)
	{
		for (int32 j = 0; j < allparents[i]->Elements.Num(); j++)
		{
			if (!elementnames.Contains(allparents[i]->Elements[j].ElementName))
			{
				elementnames.Add(allparents[i]->Elements[j].ElementName);
				CombinedElements.Add(allparents[i]->Elements[j]);
			}
		}
	}
}
