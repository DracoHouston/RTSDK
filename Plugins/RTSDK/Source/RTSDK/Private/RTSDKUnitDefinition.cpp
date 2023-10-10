// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSDKUnitDefinition.h"
#include "MassEntityTraitBase.h"

void URTSDKUnitDefinition::InitRuntimeTraits()
{
	BeforeModifiersFinalUnitTraits = FRTSDKUnitDesignerTraitCollection();
	for (int32 c = 0; c < UnitTraits.TraitCategories.Num(); c++)
	{
		FRTSDKUnitDBTraitCategoryInfo category;
		category.CategoryName = UnitTraits.TraitCategories[c].CategoryName;
		for (int32 t = 0; t < UnitTraits.TraitCategories[c].Traits.Num(); t++)
		{
			UMassEntityTraitBase* trait = NewObject<UMassEntityTraitBase>(
				this, 
				UnitTraits.TraitCategories[c].Traits[t]->GetClass(), 
				NAME_None, 
				RF_NoFlags, 
				UnitTraits.TraitCategories[c].Traits[t]);
			category.Traits.Add(trait);
		}
		BeforeModifiersFinalUnitTraits.TraitCategories.Add(category);
	}
	BuildFinalRuntimeTraits();
}

void URTSDKUnitDefinition::BuildFinalRuntimeTraits()
{
	FinalUnitTraits = FMassEntityConfig();
	TArray<UMassEntityTraitBase*> finaltraits;
	for (int32 c = 0; c < BeforeModifiersFinalUnitTraits.TraitCategories.Num(); c++)
	{
		for (int32 t = 0; t < BeforeModifiersFinalUnitTraits.TraitCategories[c].Traits.Num(); t++)
		{
			UMassEntityTraitBase* trait = NewObject<UMassEntityTraitBase>(this, BeforeModifiersFinalUnitTraits.TraitCategories[c].Traits[t]->GetClass(), NAME_None, RF_NoFlags, BeforeModifiersFinalUnitTraits.TraitCategories[c].Traits[t]);
			finaltraits.Add(trait);
		}
	}

	//todo: call all runtime modifiers passing the config by reference to each for modification

	for (int32 i = 0; i < finaltraits.Num(); i++)
	{
		FinalUnitTraits.AddTrait(*finaltraits[i]);
	}
}

void URTSDKUnitDefinition::ApplyMandatoryUnitTrait(FName inCategoryName, UMassEntityTraitBase* inTrait)
{
	//if unit def came with this trait defined we do nothing.
	for (int32 c = 0; c < UnitTraits.TraitCategories.Num(); c++)
	{
		for (int32 t = 0; t < UnitTraits.TraitCategories[c].Traits.Num(); t++)
		{
			if (UnitTraits.TraitCategories[c].Traits[t]->GetClass() == inTrait->GetClass())
			{
				return;
			}
		}
	}
	//if unit def didnt come with this trait defined we look for an existing
	//mod added trait and replace it
	bool replacedtrait = false;
	for (int32 c = 0; c < BeforeModifiersFinalUnitTraits.TraitCategories.Num(); c++)
	{
		for (int32 t = 0; t < BeforeModifiersFinalUnitTraits.TraitCategories[c].Traits.Num(); t++)
		{
			if (BeforeModifiersFinalUnitTraits.TraitCategories[c].Traits[t]->GetClass() == inTrait->GetClass())
			{
				BeforeModifiersFinalUnitTraits.TraitCategories[c].Traits[t] = inTrait;
				replacedtrait = true;
				break;
			}
		}
	}
	//if we didn't replace it, add it
	if (!replacedtrait)
	{
		bool foundcategory = false;
		for (int32 c = 0; c < BeforeModifiersFinalUnitTraits.TraitCategories.Num(); c++)
		{
			if (BeforeModifiersFinalUnitTraits.TraitCategories[c].CategoryName == inCategoryName)
			{
				BeforeModifiersFinalUnitTraits.TraitCategories[c].Traits.Add(inTrait);
				foundcategory = true;
			}
		}
		if (!foundcategory)
		{
			FRTSDKUnitDBTraitCategoryInfo category;
			category.CategoryName = inCategoryName;
			category.Traits.Add(inTrait);
			BeforeModifiersFinalUnitTraits.TraitCategories.Add(category);
		}
	}
	BuildFinalRuntimeTraits();
}
