// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RTSDKDataEditorTreeNodeInterface.h"
#include "RTSDKUnitTypes.generated.h"

class UMassEntityTraitBase;
class URTSDKUnitDefinition;

UCLASS(DefaultToInstanced, EditInlineNew)
class RTSDK_API URTSDKModTraitsCollection : public UObject, public IRTSDKDataEditorTreeNodeInterface
{
	GENERATED_BODY()

public:

	//IRTSDKDataEditorTreeNodeInterface
	virtual FText GetDisplayNameForDataEditorTree() override;
	virtual UMaterialInterface* GetSmallIconForDataEditorTree() override;
	virtual TArray<IRTSDKDataEditorTreeNodeInterface*> GetChildNodeObjectsForDataEditorTree() override;
	//~IRTSDKDataEditorTreeNodeInterface

	

	/*UPROPERTY(transient)
		TArray<TObjectPtr<IRTSDKDataEditorTreeNodeInterface>> DataEditorTreeChildren;*/

	UPROPERTY(Category = "General", EditAnywhere)
		FName DevName;

	UPROPERTY(Category = "General", EditAnywhere, meta = (DisplayAfter = "DevName"))
		FText DisplayName;

	UPROPERTY(Category = "General", EditAnywhere, meta = (DisplayAfter = "DisplayName"))
		TObjectPtr<UMaterialInterface> SmallIcon;

	UPROPERTY(Category = "Traits", EditAnywhere, Instanced, meta = (DisplayAfter = "SmallIcon"))
		TArray<TObjectPtr<UMassEntityTraitBase>> MandatoryTraits;

	UPROPERTY(Category = "Traits", EditAnywhere, Instanced, meta = (DisplayAfter = "MandatoryTraits"))
		TArray<TObjectPtr<UMassEntityTraitBase>> OptionalTraits;

	UPROPERTY(Category = "Traits", EditAnywhere, Instanced, meta = (DisplayAfter = "OptionalTraits"))
		TArray<TObjectPtr<UMassEntityTraitBase>> MandatoryMutuallyExclusiveTraits;

	UPROPERTY(Category = "Traits", EditAnywhere, Instanced, meta = (DisplayAfter = "MandatoryMutuallyExclusiveTraits"))
		TArray<TObjectPtr<UMassEntityTraitBase>> OptionalMutuallyExclusiveTraits;

	UPROPERTY(EditAnywhere, Instanced, meta = (DisplayAfter = "OptionalMutuallyExclusiveTraits"))
		TArray<TObjectPtr<URTSDKModTraitsCollection>> ChildCollections;
};

UCLASS(DefaultToInstanced, EditInlineNew)
class RTSDK_API URTSDKUnitTraitsCollection : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, Instanced)
		TArray<TObjectPtr<URTSDKUnitTraitsCollection>> ChildCollections;

	UPROPERTY(VisibleAnywhere)
		FName DevName;

	UPROPERTY(VisibleAnywhere)
		FText DisplayName;

	UPROPERTY(VisibleAnywhere)
		TObjectPtr<UMaterialInterface> SmallIcon;

};

UCLASS(DefaultToInstanced, EditInlineNew)
class RTSDK_API URTSDKModUnitDefinitionCollection : public UObject, public IRTSDKDataEditorTreeNodeInterface
{
	GENERATED_BODY()

public:

	//IRTSDKDataEditorTreeNodeInterface
	virtual FText GetDisplayNameForDataEditorTree() override;
	virtual UMaterialInterface* GetSmallIconForDataEditorTree() override;
	virtual TArray<IRTSDKDataEditorTreeNodeInterface*> GetChildNodeObjectsForDataEditorTree() override;
	//~IRTSDKDataEditorTreeNodeInterface

	UPROPERTY(Category = "General", VisibleAnywhere)
		FName DevName;

	UPROPERTY(Category = "General", VisibleAnywhere, meta = (DisplayAfter = "DevName"))
		FText DisplayName;

	UPROPERTY(Category = "General", VisibleAnywhere, meta = (DisplayAfter = "DisplayName"))
		TObjectPtr<UMaterialInterface> SmallIcon;

	UPROPERTY(Category = "Units", EditAnywhere, Instanced, meta = (DisplayAfter = "SmallIcon"))
		TArray<TObjectPtr<URTSDKUnitDefinition>> UnitDefinitions;

	UPROPERTY(EditAnywhere, Instanced, meta = (DisplayAfter = "MandatoryTraits"))
		TArray<TObjectPtr<URTSDKModUnitDefinitionCollection>> ChildCollections;

};