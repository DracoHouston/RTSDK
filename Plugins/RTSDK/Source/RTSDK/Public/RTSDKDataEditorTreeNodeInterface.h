// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "RTSDKDataEditorTreeNodeInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class URTSDKDataEditorTreeNodeInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * Interface for things that can be listed on the data editor tool tree
 */
class RTSDK_API IRTSDKDataEditorTreeNodeInterface
{
	GENERATED_BODY()

public:

	virtual FText GetDisplayNameForDataEditorTree() { return FText(); }
	virtual UMaterialInterface* GetSmallIconForDataEditorTree() { return nullptr; }

	virtual TArray<IRTSDKDataEditorTreeNodeInterface*> GetChildNodeObjectsForDataEditorTree() { return TArray<IRTSDKDataEditorTreeNodeInterface*>(); }


};
