// Fill out your copyright notice in the Description page of Project Settings.

#include "RTSDKUnitTypes.h"
#include "MassEntityTraitBase.h"
#include "RTSDKUnitDefinition.h"

FText URTSDKModTraitsCollection::GetDisplayNameForDataEditorTree()
{
	return DisplayName;
}

UMaterialInterface* URTSDKModTraitsCollection::GetSmallIconForDataEditorTree()
{
	return SmallIcon;
}

TArray<IRTSDKDataEditorTreeNodeInterface*> URTSDKModTraitsCollection::GetChildNodeObjectsForDataEditorTree()
{
	TArray<IRTSDKDataEditorTreeNodeInterface*> childnodes;
	for (int32 i = 0; i < ChildCollections.Num(); i++)
	{
		IRTSDKDataEditorTreeNodeInterface* child = Cast<IRTSDKDataEditorTreeNodeInterface>(ChildCollections[i]);
		if (child != nullptr)
		{
			childnodes.Add(child);
		}
	}
	return childnodes;
}

FText URTSDKModUnitDefinitionCollection::GetDisplayNameForDataEditorTree()
{
	return DisplayName;
}

UMaterialInterface* URTSDKModUnitDefinitionCollection::GetSmallIconForDataEditorTree()
{
	return SmallIcon;
}

TArray<IRTSDKDataEditorTreeNodeInterface*> URTSDKModUnitDefinitionCollection::GetChildNodeObjectsForDataEditorTree()
{
	TArray<IRTSDKDataEditorTreeNodeInterface*> childnodes;
	for (int32 i = 0; i < ChildCollections.Num(); i++)
	{
		IRTSDKDataEditorTreeNodeInterface* child = Cast<IRTSDKDataEditorTreeNodeInterface>(ChildCollections[i]);
		if (child != nullptr)
		{
			childnodes.Add(child);
		}
	}
	return childnodes;
}
