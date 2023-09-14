// Fill out your copyright notice in the Description page of Project Settings.

#include "RTSDKTypes.h"
#include "RTSDKConstants.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "RTSDKUnitComponent.h"
#include "RTSDKInputModifiers.h"
#include "RTSDKKeyBindMetadata.h"
#include "RTSDKVisRootComponent.h"
#include "RTSDKGameSimSubsystem.h"

AActor* FRTSTraitHelpers::GetOwnerAsActor(UObject* inOwner)
{
	if (inOwner != nullptr)
	{
		return Cast<AActor>(inOwner);
	}
	return nullptr;
}

void GetChildBounds(USceneComponent* inChild, FRTSVector64& outMin, FRTSVector64& outMax)
{
	TArray<USceneComponent*> children = TArray<USceneComponent*>();
	inChild->GetChildrenComponents(false, children);
	//FRTSVector64 origin = (FRTSVector64)inChild->Bounds.Origin;
	FRTSVector64 boxextent = (FRTSVector64)inChild->Bounds.BoxExtent;
	FRTSVector64 min = FRTSVector64::ZeroVector - boxextent;
	FRTSVector64 max = FRTSVector64::ZeroVector + boxextent;
	outMin = FRTSVector64::Min(outMin, min);
	outMax = FRTSVector64::Max(outMax, max);
	for (int32 i = 0; i < children.Num(); i++)
	{
		if (!children[i]->IsA<URTSDKVisRootComponent>())
		{
			GetChildBounds(children[i], outMin, outMax);
		}
	}
}

bool FRTSTraitHelpers::GetBounds(URTSDKGameSimSubsystem* inSim, AActor* inActor, FRTSVector64& outMin, FRTSVector64& outMax, FRTSVector64& outSize, FRTSNumber64& outHalfHeight, FRTSNumber64& outRadius, FRTSVector64& outFeetPosition)
{
	if (inActor == nullptr)
	{
		return false;
	}
	USceneComponent* simroot = inActor->GetRootComponent();
	if (simroot == nullptr)
	{
		return false;
	}
	FRTSVector64 globalgravdir = FRTSVector64::DownVector;
	if (inSim != nullptr)
	{
		globalgravdir = inSim->GetGravityDirection();
	}

	TArray<USceneComponent*> rootchildren = TArray<USceneComponent*>();
	simroot->GetChildrenComponents(false, rootchildren);
	//FRTSVector64 origin = (FRTSVector64)simroot->Bounds.Origin;
	FRTSVector64 boxextent = (FRTSVector64)simroot->Bounds.BoxExtent;
	outMin = FRTSVector64::ZeroVector - boxextent;
	outMax = FRTSVector64::ZeroVector + boxextent;
	for (int32 i = 0; i < rootchildren.Num(); i++)
	{
		if (!rootchildren[i]->IsA<URTSDKVisRootComponent>())
		{
			GetChildBounds(rootchildren[i], outMin, outMax);
		}
	}
	outSize = outMax - outMin;
	outHalfHeight = FRTSMath::Abs(outMin.Z);
	outFeetPosition = globalgravdir * outHalfHeight;
	outRadius = FRTSMath::Abs(FRTSMath::Max(outMax.X, outMax.Y));
	return true;
}

FRTSDKSavedBindContext::FRTSDKSavedBindContext(UInputMappingContext* inInputMappingContext)
{
	struct FKeyMappingPair
	{
		const FEnhancedActionKeyMapping* Primary;
		const FEnhancedActionKeyMapping* Secondary;
		bool bHasPrimary = false;
		bool bHasSecondary = false;
	};
	ContextName = inInputMappingContext->GetFName();
	DisplayName = inInputMappingContext->ContextDescription;
	TMap<FString, FKeyMappingPair> savedbindpairs;
	const TArray<FEnhancedActionKeyMapping> mappings = inInputMappingContext->GetMappings();
	const FString secondarysuffix = TEXT("_Secondary");
	for (int32 i = 0; i < mappings.Num(); i++)
	{
		if (mappings[i].bIsPlayerMappable)
		{
			FString name = mappings[i].PlayerMappableOptions.Name.ToString();
			
			if (name.EndsWith(secondarysuffix))
			{
				FString namewithoutsuffix = name.LeftChop(secondarysuffix.Len());
				FKeyMappingPair& pair = savedbindpairs.FindOrAdd(namewithoutsuffix);
				pair.Secondary = &mappings[i];
				pair.bHasSecondary = true;
			}
			else
			{
				FKeyMappingPair& pair = savedbindpairs.FindOrAdd(name);
				pair.Primary = &mappings[i];
				pair.bHasPrimary = true;
			}
		}
	}
	for (auto It = savedbindpairs.CreateConstIterator(); It; ++It)
	{
		if (It->Value.bHasPrimary && It->Value.bHasSecondary)
		{
			PlayerBinds.Add(FRTSDKSavedBindPair(*It->Value.Primary, *It->Value.Secondary));
		}
	}
}

FRTSDKSavedBind::FRTSDKSavedBind(const FEnhancedActionKeyMapping& inInputMapping)
{
	Action = inInputMapping.PlayerMappableOptions.Name;
	BoundKeys.Add(inInputMapping.Key);
	bool foundignoretaporholdoptions = false;
	for (int32 i = 0; i < inInputMapping.Modifiers.Num(); i++)
	{
		if (inInputMapping.Modifiers[i]->IsA<URTSDKInputModifierAdditionalKeys>())
		{
			URTSDKInputModifierAdditionalKeys* additionalkeys = Cast<URTSDKInputModifierAdditionalKeys>(inInputMapping.Modifiers[i]);
			BoundKeys += additionalkeys->Keys;
		}
		else if (inInputMapping.Modifiers[i]->IsA<URTSDKInputModifierDefaultModifierKeyOptions>())
		{
			URTSDKInputModifierDefaultModifierKeyOptions* activemodifieroptions = Cast<URTSDKInputModifierDefaultModifierKeyOptions>(inInputMapping.Modifiers[i]);
			ModifierOptions = activemodifieroptions->OptionsPerModifierKeyCombo;
			AvailableModifierOptions = activemodifieroptions->AvailableOptions;
			ActiveModifiers = activemodifieroptions->DefaultActiveModifierKeys;
		}
		else if (inInputMapping.Modifiers[i]->IsA<URTSDKInputModifierIgnoreTapOrHoldOptions>())
		{
			TapOrHoldOptions = ERTSDKInputTapOrHold::Ignore;
			foundignoretaporholdoptions = true;
		}
	}
	if (!foundignoretaporholdoptions)
	{
		if (inInputMapping.Triggers.Num() == 0)//no triggers, it will default to key down trigger
		{
			TapOrHoldOptions = ERTSDKInputTapOrHold::Ignore;
		}
		else
		{
			for (int32 i = 0; i < inInputMapping.Triggers.Num(); i++)
			{
				if (inInputMapping.Triggers[i]->IsA<UInputTriggerHold>())
				{
					TapOrHoldOptions = ERTSDKInputTapOrHold::Hold;
				}
				else if (inInputMapping.Triggers[i]->IsA<UInputTriggerTap>())
				{
					TapOrHoldOptions = ERTSDKInputTapOrHold::Tap;
				}
				else if (inInputMapping.Triggers[i]->IsA<UInputTriggerReleased>())
				{
					TapOrHoldOptions = ERTSDKInputTapOrHold::Ignore;
				}
				else if (inInputMapping.Triggers[i]->IsA<UInputTriggerDown>())
				{
					TapOrHoldOptions = ERTSDKInputTapOrHold::Ignore;
				}
			}
		}
	}
}

FRTSDKSavedBindPair::FRTSDKSavedBindPair(const FEnhancedActionKeyMapping& inPrimaryMapping, const FEnhancedActionKeyMapping& inSecondaryMapping)
{
	Primary = FRTSDKSavedBind(inPrimaryMapping);
	Secondary = FRTSDKSavedBind(inSecondaryMapping);
	DisplayName = inPrimaryMapping.PlayerMappableOptions.DisplayName;
	BindName = inPrimaryMapping.PlayerMappableOptions.Name;
	for (int32 i = 0; i < inPrimaryMapping.Modifiers.Num(); i++)
	{
		if (inPrimaryMapping.Modifiers[i]->IsA<URTSDKInputModifierSettingsTooltip>())
		{
			URTSDKInputModifierSettingsTooltip* tooltip = Cast<URTSDKInputModifierSettingsTooltip>(inPrimaryMapping.Modifiers[i]);
			TooltipText = tooltip->TooltipText;
			break;
		}
	}
}

void FRTSDKSavedBindPair::SetInitialValues()
{
}

FRTSDKSavedBind FRTSDKSavedBindPair::GetBind(bool inbIsPrimary)
{
	return inbIsPrimary ? Primary : Secondary;
}

FRTSDKSavedBind& FRTSDKSavedBindPair::GetMutableBind(bool inbIsPrimary)
{
	return inbIsPrimary ? Primary : Secondary;
}

FRTSDKSavedBindContext FRTSDKInputSettingsProfile::GetContextByName(FName inContextName)
{
	for (int32 i = 0; i < PlayerBindContexts.Num(); i++)
	{
		if (PlayerBindContexts[i].ContextName == inContextName)
		{
			return PlayerBindContexts[i];
		}
	}
	return FRTSDKSavedBindContext();
}

FRTSDKSavedBindContext& FRTSDKInputSettingsProfile::GetMutableContextByName(FName inContextName)
{
	for (int32 i = 0; i < PlayerBindContexts.Num(); i++)
	{
		if (PlayerBindContexts[i].ContextName == inContextName)
		{
			return PlayerBindContexts[i];
		}
	}
	
	return PlayerBindContexts[0];
}

FRTSDKSavedBindPair FRTSDKSavedBindContext::GetBindsByName(FName inBindName)
{
	for (int32 i = 0; i < PlayerBinds.Num(); i++)
	{
		if (PlayerBinds[i].BindName == inBindName)
		{
			return PlayerBinds[i];
		}
	}
	return FRTSDKSavedBindPair();
}

FRTSDKSavedBindPair& FRTSDKSavedBindContext::GetMutableBindsByName(FName inBindName)
{
	for (int32 i = 0; i < PlayerBinds.Num(); i++)
	{
		if (PlayerBinds[i].BindName == inBindName)
		{
			return PlayerBinds[i];
		}
	}
	
	return PlayerBinds[0];
}

FRTSDKSavedHUDElementSettings FRTSDKHUDSettingsProfile::GetElementByName(FName inElementName)
{
	for (int32 i = 0; i < Elements.Num(); i++)
	{
		if (Elements[i].ElementName == inElementName)
		{
			return Elements[i];
		}
	}
	return FRTSDKSavedHUDElementSettings();
}
