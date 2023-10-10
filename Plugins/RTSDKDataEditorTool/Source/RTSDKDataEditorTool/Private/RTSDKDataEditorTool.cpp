// Copyright Epic Games, Inc. All Rights Reserved.

#include "RTSDKDataEditorTool.h"
#include "RTSDKDataEditorToolStyle.h"
#include "RTSDKDataEditorToolCommands.h"
#include "LevelEditor.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Widgets/Views/STreeView.h"
#include "ToolMenus.h"
#include "RTSDKModManager.h"
#include "RTSDKModDefinitionBase.h"
#include "RTSDKGameModDefinition.h"

static const FName RTSDKDataEditorToolTabName("RTSDKDataEditorTool");

#define LOCTEXT_NAMESPACE "FRTSDKDataEditorToolModule"

void FRTSDKDataEditorToolModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FRTSDKDataEditorToolStyle::Initialize();
	FRTSDKDataEditorToolStyle::ReloadTextures();

	FRTSDKDataEditorToolCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FRTSDKDataEditorToolCommands::Get().OpenPluginWindow,
		FExecuteAction::CreateRaw(this, &FRTSDKDataEditorToolModule::PluginButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FRTSDKDataEditorToolModule::RegisterMenus));
	
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(RTSDKDataEditorToolTabName, FOnSpawnTab::CreateRaw(this, &FRTSDKDataEditorToolModule::OnSpawnPluginTab))
		.SetDisplayName(LOCTEXT("FRTSDKDataEditorToolTabTitle", "RTSDKDataEditorTool"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);
}

void FRTSDKDataEditorToolModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FRTSDKDataEditorToolStyle::Shutdown();

	FRTSDKDataEditorToolCommands::Unregister();

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(RTSDKDataEditorToolTabName);
}

TSharedRef<SDockTab> FRTSDKDataEditorToolModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
	FText WidgetText = FText::Format(
		LOCTEXT("WindowWidgetText", "Add code to {0} in {1} to override this window's contents"),
		FText::FromString(TEXT("FRTSDKDataEditorToolModule::OnSpawnPluginTab")),
		FText::FromString(TEXT("RTSDKDataEditorTool.cpp"))
	);
	RebuildRTSDKObjectTree();
	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			// Put your tab content here!
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			[
				SNew(SHorizontalBox)
				+SHorizontalBox::Slot()
				[
					SNew(SScrollBox)
					+ SScrollBox::Slot()
					[
						SNew(STreeView<TSharedRef<FRTSDKDataEditorTreeItem>>)
						.OnGenerateRow_Lambda(
							[this](
								TSharedPtr<FRTSDKDataEditorTreeItem> InItem,
								const TSharedRef<STableViewBase>& OwnerTable
								) -> TSharedRef<ITableRow> 
							{
								return SNew(SDataEditorTreeRow, OwnerTable)
								.Item(InItem);
							}
						)
						.OnGetChildren_Lambda(
							[this](
								TSharedRef<FRTSDKDataEditorTreeItem> InRowType, 
								TArray<TSharedRef<FRTSDKDataEditorTreeItem>>& OutChildren
								)
							{
								//OutChildren.Empty();
								InRowType.Get().GetChildrenForTree(OutChildren);
							}
						)
						.TreeItemsSource(&TreeItems)
							.PreventThrottling(true)
						//SNew(SBox)
					]
				]
				+ SHorizontalBox::Slot()
				[
					SNew(SVerticalBox)
					+ SVerticalBox::Slot()
					[
						SNew(SBox)
					]
					+ SVerticalBox::Slot()
					[
						SNew(SBox)
					]
					+ SVerticalBox::Slot()
					[
						SNew(SBox)
					]
				]
			]
		];
}

void FRTSDKDataEditorToolModule::RebuildRTSDKObjectTree()
{
	URTSDKModManager* modmanager = GEngine->GetEngineSubsystem<URTSDKModManager>();
	TArray<URTSDKGameModDefinition*> games = modmanager->GetAllValidGameMods(true);
	TreeItems.Empty(games.Num());
	for (int32 i = 0; i < games.Num(); i++)
	{
		TSharedRef<FRTSDKDataEditorTreeItem> item = FRTSDKDataEditorTreeItem::Make(games[i]);
		item.Get().BuildChildrenForTree();
		TreeItems.Add(item);
	}
}

void FRTSDKDataEditorToolModule::PluginButtonClicked()
{
	FGlobalTabmanager::Get()->TryInvokeTab(RTSDKDataEditorToolTabName);
}

void FRTSDKDataEditorToolModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
			Section.AddMenuEntryWithCommandList(FRTSDKDataEditorToolCommands::Get().OpenPluginWindow, PluginCommands);
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("Settings");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FRTSDKDataEditorToolCommands::Get().OpenPluginWindow));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FRTSDKDataEditorToolModule, RTSDKDataEditorTool)



void SDataEditorTreeRow::Construct(const FArguments& InArgs, const TSharedRef<STableViewBase>& OwnerTableView)
{
	Item = InArgs._Item;
	STableRow<TSharedRef<FRTSDKDataEditorTreeItem>>::Construct(
		STableRow<TSharedRef<FRTSDKDataEditorTreeItem>>::FArguments().Content()
		[
			SNew(SBox)
			[
				SNew(STextBlock).Text(Item.Get()->DisplayText)
			]
		]
	.ShowWires(true),
		OwnerTableView);
}

void FRTSDKDataEditorTreeItem::GetChildrenForTree(TArray<TSharedRef<FRTSDKDataEditorTreeItem>>& OutChildren)
{
	OutChildren = ChildNodes;
}

void FRTSDKDataEditorTreeItem::BuildChildrenForTree()
{
	TArray<IRTSDKDataEditorTreeNodeInterface*> childrenobjects = NodeObject->GetChildNodeObjectsForDataEditorTree();
	ChildNodes.Empty(childrenobjects.Num());
	for (int32 i = 0; i < childrenobjects.Num(); i++)
	{
		TSharedRef<FRTSDKDataEditorTreeItem> child = FRTSDKDataEditorTreeItem::Make(childrenobjects[i]);
		child.Get().BuildChildrenForTree();
		ChildNodes.Add(child);
	}
}
