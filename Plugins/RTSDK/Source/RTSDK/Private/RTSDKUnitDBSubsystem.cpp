// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSDKUnitDBSubsystem.h"
#include "RTSDKUnitDefinition.h"
#include "MassEntityTraitBase.h"

void URTSDKUnitDBSubsystem::AddMandatoryUnitTraits(const TArray<FRTSDKUnitDBTraitCategoryInfo>& inTraits)
{
	for (int32 a = 0; a < inTraits.Num(); a++)
	{
		FRTSDKUnitDBTraitArray& traits = MandatoryUnitTraitsByCategory.FindOrAdd(inTraits[a].CategoryName);
		for (int32 i = 0; i < inTraits[a].Traits.Num(); i++)
		{
			for (int32 t = 0; t < traits.Traits.Num(); t++)
			{
				if (traits.Traits[t]->GetClass() == inTraits[a].Traits[i]->GetClass())
				{
					traits.Traits.RemoveAt(t);
					t--;
				}
			}
			traits.Traits.Add(inTraits[a].Traits[i]);
		}
	}
}

void URTSDKUnitDBSubsystem::AddOptionalUnitTraits(const TArray<FRTSDKUnitDBTraitCategoryInfo>& inTraits)
{
	for (int32 a = 0; a < inTraits.Num(); a++)
	{
		FRTSDKUnitDBTraitArray& traits = OptionalUnitTraitsByCategory.FindOrAdd(inTraits[a].CategoryName);
		for (int32 i = 0; i < inTraits[a].Traits.Num(); i++)
		{
			for (int32 t = 0; t < traits.Traits.Num(); t++)
			{
				if (traits.Traits[t]->GetClass() == inTraits[a].Traits[i]->GetClass())
				{
					traits.Traits.RemoveAt(t);
					t--;
				}
			}
			traits.Traits.Add(inTraits[a].Traits[i]);
		}
	}
}

void URTSDKUnitDBSubsystem::AddMutuallyExclusiveMandatoryUnitTraits(const TArray<FRTSDKUnitDBTraitCategoryInfo>& inTraits)
{
	for (int32 a = 0; a < inTraits.Num(); a++)
	{
		FRTSDKUnitDBTraitArray& traits = MandatoryMutuallyExclusiveUnitTraitsByCategory.FindOrAdd(inTraits[a].CategoryName);
		for (int32 i = 0; i < inTraits[a].Traits.Num(); i++)
		{
			for (int32 t = 0; t < traits.Traits.Num(); t++)
			{
				if (traits.Traits[t]->GetClass() == inTraits[a].Traits[i]->GetClass())
				{
					traits.Traits.RemoveAt(t);
					t--;
				}
			}
			traits.Traits.Add(inTraits[a].Traits[i]);
		}
	}
}

void URTSDKUnitDBSubsystem::AddMutuallyExclusiveOptionalUnitTraits(const TArray<FRTSDKUnitDBTraitCategoryInfo>& inTraits)
{
	for (int32 a = 0; a < inTraits.Num(); a++)
	{
		FRTSDKUnitDBTraitArray& traits = OptionalMutuallyExclusiveUnitTraitsByCategory.FindOrAdd(inTraits[a].CategoryName);
		for (int32 i = 0; i < inTraits[a].Traits.Num(); i++)
		{
			for (int32 t = 0; t < traits.Traits.Num(); t++)
			{
				if (traits.Traits[t]->GetClass() == inTraits[a].Traits[i]->GetClass())
				{
					traits.Traits.RemoveAt(t);
					t--;
				}
			}
			traits.Traits.Add(inTraits[a].Traits[i]);
		}
	}
}

void URTSDKUnitDBSubsystem::AddUnit(TSubclassOf<URTSDKUnitDefinition> inDefinition)
{
	if (inDefinition == nullptr)
	{
		return;
	}
	if (WorldUnitDB.Contains(inDefinition))
	{
		return;
	}
	WorldUnitDB.FindOrAdd(inDefinition) = NewObject<URTSDKUnitDefinition>(this, inDefinition);
}

void URTSDKUnitDBSubsystem::AddUnitModifier(TSubclassOf<URTSDKUnitDefinition> inDefinition, URTSDKUnitDefinitionModifier* inModifier)
{
}

void URTSDKUnitDBSubsystem::RemoveUnitModifier(TSubclassOf<URTSDKUnitDefinition> inDefinition, URTSDKUnitDefinitionModifier* inModifier)
{
}

void URTSDKUnitDBSubsystem::RemoveUnit(TSubclassOf<URTSDKUnitDefinition> inDefinition)
{
	WorldUnitDB.Remove(inDefinition);
}

URTSDKUnitDefinition* URTSDKUnitDBSubsystem::GetFinalUnitDefinition(TSubclassOf<URTSDKUnitDefinition> inDefinition)
{
	if (WorldUnitDB.Contains(inDefinition))
	{
		return WorldUnitDB[inDefinition];
	}
	return nullptr;
}
