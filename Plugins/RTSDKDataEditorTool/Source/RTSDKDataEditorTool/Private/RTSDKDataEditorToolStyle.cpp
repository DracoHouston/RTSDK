// Copyright Epic Games, Inc. All Rights Reserved.

#include "RTSDKDataEditorToolStyle.h"
#include "Styling/SlateStyleRegistry.h"
#include "Framework/Application/SlateApplication.h"
#include "Slate/SlateGameResources.h"
#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyleMacros.h"

#define RootToContentDir Style->RootToContentDir

TSharedPtr<FSlateStyleSet> FRTSDKDataEditorToolStyle::StyleInstance = nullptr;

void FRTSDKDataEditorToolStyle::Initialize()
{
	if (!StyleInstance.IsValid())
	{
		StyleInstance = Create();
		FSlateStyleRegistry::RegisterSlateStyle(*StyleInstance);
	}
}

void FRTSDKDataEditorToolStyle::Shutdown()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(*StyleInstance);
	ensure(StyleInstance.IsUnique());
	StyleInstance.Reset();
}

FName FRTSDKDataEditorToolStyle::GetStyleSetName()
{
	static FName StyleSetName(TEXT("RTSDKDataEditorToolStyle"));
	return StyleSetName;
}

const FVector2D Icon16x16(16.0f, 16.0f);
const FVector2D Icon20x20(20.0f, 20.0f);

TSharedRef< FSlateStyleSet > FRTSDKDataEditorToolStyle::Create()
{
	TSharedRef< FSlateStyleSet > Style = MakeShareable(new FSlateStyleSet("RTSDKDataEditorToolStyle"));
	Style->SetContentRoot(IPluginManager::Get().FindPlugin("RTSDKDataEditorTool")->GetBaseDir() / TEXT("Resources"));

	Style->Set("RTSDKDataEditorTool.OpenPluginWindow", new IMAGE_BRUSH_SVG(TEXT("PlaceholderButtonIcon"), Icon20x20));

	return Style;
}

void FRTSDKDataEditorToolStyle::ReloadTextures()
{
	if (FSlateApplication::IsInitialized())
	{
		FSlateApplication::Get().GetRenderer()->ReloadTextureResources();
	}
}

const ISlateStyle& FRTSDKDataEditorToolStyle::Get()
{
	return *StyleInstance;
}
