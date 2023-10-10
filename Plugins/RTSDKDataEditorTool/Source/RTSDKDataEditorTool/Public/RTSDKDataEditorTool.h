// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "RTSDKDataEditorTreeNodeInterface.h"

class FToolBarBuilder;
class FMenuBuilder;

struct FRTSDKDataEditorTreeItem
{
public:

	FText DisplayText;
	UMaterialInterface* SmallIcon;

	static TSharedRef<FRTSDKDataEditorTreeItem> Make(IRTSDKDataEditorTreeNodeInterface* inObject)
	{
		return MakeShareable(new FRTSDKDataEditorTreeItem(inObject));
	}

	//FRTSDKDataEditorTreeItem() {}

	void GetChildrenForTree(TArray<TSharedRef<FRTSDKDataEditorTreeItem>>& OutChildren);
	void BuildChildrenForTree();

protected:

	FRTSDKDataEditorTreeItem(IRTSDKDataEditorTreeNodeInterface* inObject)
	{
		NodeObject = inObject;
		DisplayText = NodeObject->GetDisplayNameForDataEditorTree();
		SmallIcon = NodeObject->GetSmallIconForDataEditorTree();
	}
	FRTSDKDataEditorTreeItem() {}

	TArray<TSharedRef<FRTSDKDataEditorTreeItem>> ChildNodes;

	IRTSDKDataEditorTreeNodeInterface* NodeObject;
};

class SDataEditorTreeRow : public STableRow<TSharedRef<FRTSDKDataEditorTreeItem>>
{
public:
	SLATE_BEGIN_ARGS(SDataEditorTreeRow) {}

	SLATE_ARGUMENT(TSharedPtr<FRTSDKDataEditorTreeItem>, Item)
	
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, const TSharedRef<STableViewBase>& OwnerTableView);

protected:

	TSharedPtr<FRTSDKDataEditorTreeItem> Item;
};

class FRTSDKDataEditorToolModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	
	/** This function will be bound to Command (by default it will bring up plugin window) */
	void PluginButtonClicked();
	
private:

	void RegisterMenus();

	TSharedRef<class SDockTab> OnSpawnPluginTab(const class FSpawnTabArgs& SpawnTabArgs);

private:
	TSharedPtr<class FUICommandList> PluginCommands;

protected:

	TArray<TSharedRef<FRTSDKDataEditorTreeItem>> TreeItems;

	void RebuildRTSDKObjectTree();


	//void GetChildrenForTree(TSharedPtr<FRTSDKDataEditorTreeItem> InItem, TArray<TSharedPtr<FRTSDKDataEditorTreeItem>>& OutChildren);

	//TSharedRef<ITableRow> GenerateRowForTree(TSharedPtr<FRTSDKDataEditorTreeItem> Item, const TSharedRef<STableViewBase>& OwnerTable);
};
