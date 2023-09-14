// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSDKUnitComponent.h"
#include "MassEntityView.h"
#include "MassEntitySubsystem.h"
#include "RTSDKGameSimSubsystem.h"
#include "RTSDKUnitDefinition.h"
#include "RTSDKBatchedScriptCommand.h"
#include "RTSDKScriptExecutionContext.h"
#include "RTSDKFragments.h"
#include "RTSDKSimState.h"
#include "RTSDKForceState.h"



//void URTSDKUnitComponent::OnRegister()
//{
//	if (IsRunningCommandlet() || IsRunningCookCommandlet() || GIsCookerLoadingPackage)
//	{
//		// ignore, we're not doing any registration while cooking or running a commandlet
//		Super::OnRegister();
//		return;
//	}
//
//	if (GetOuter() == nullptr || GetOuter()->HasAnyFlags(RF_ClassDefaultObject | RF_ArchetypeObject) || HasAnyFlags(RF_ArchetypeObject))
//	{
//		// we won't try registering a CDO's component with Mass
//		ensure(false && "temp, wanna know this happened");
//		Super::OnRegister();
//		return;
//	}
//
//	UWorld* World = GetWorld();
//	if (World == nullptr
//#if WITH_EDITOR
//		|| World->IsPreviewWorld() || (bAutoRegisterInEditorMode == false && World->IsGameWorld() == false)
//#endif // WITH_EDITOR
//		)
//	{
//		// we don't care about preview worlds. Those are transient, temporary worlds like the one created when opening a BP editor.
//		Super::OnRegister();
//		return;
//	}
//
//	TConstArrayView<UMassEntityTraitBase*> traits = EntityConfig.GetTraits();
//	bool traitexists = false;
//	for (int32 i = 0; i < traits.Num(); i++)
//	{
//		if ((traits[i] != nullptr) && (traits[i]->GetClass() == URTSUnitRegistrationTrait::StaticClass()))
//		{
//			traitexists = true;
//			break;
//		}
//	}
//
//	if (!traitexists)
//	{
//		URTSUnitRegistrationTrait* unitregotrait = NewObject<URTSUnitRegistrationTrait>(this);
//		EntityConfig.AddTrait(*unitregotrait);
//	}
//	
//	Super::OnRegister();
//
//	URTSDKGameSimSubsystem* sim = World->GetSubsystem<URTSDKGameSimSubsystem>();
//	if (sim == nullptr)
//	{
//		return;
//	}
//	OwningSim = sim;
//}

void URTSDKUnitComponent::Setup(URTSDKGameSimSubsystem* inSimSubsystem, uint32 inUnitID, int32 inForceID, UClass* inUnitDefintion, FMassEntityHandle inEntityHandle)
{
	OwningSim = inSimSubsystem;
	UnitID = inUnitID;
	ForceID = inForceID;
	ForceState = OwningSim->GetSimState()->GetForce(inForceID);
	UnitDefintion = inUnitDefintion;
	UnitDefintionCDO = inUnitDefintion->GetDefaultObject<URTSDKUnitDefinition>();
	UnitEntityHandle = inEntityHandle;
	UnitIcon = UnitDefintionCDO->Icon;
	UnitDisplayName = UnitDefintionCDO->DisplayName;
	bUnitIsAlive = true;
}

int64 URTSDKUnitComponent::GetUnitID() const
{
	return UnitID;
}

FVector URTSDKUnitComponent::GetUnitInput()
{
	/*FMassEntityView ent(GetWorld()->GetSubsystem<UMassEntitySubsystem>()->GetEntityManager(), UnitEntityHandle);
	FRTSMovementInputFragment& input = ent.GetFragmentData<FRTSMovementInputFragment>();
	return input.Input;*/
	return FVector();
}

FFixed64 URTSDKUnitComponent::GetUnitHealth()
{
	return FixedPoint::Constants::Fixed64::One;
}

FFixed64 URTSDKUnitComponent::GetUnitMaxHealth()
{
	return FixedPoint::Constants::Fixed64::One;
}

FText URTSDKUnitComponent::GetUnitDisplayName()
{
	return UnitDisplayName;
}

UMaterialInterface* URTSDKUnitComponent::GetUnitIcon()
{
	return UnitIcon;
}

bool URTSDKUnitComponent::GetUnitIsAlive()
{
	return bUnitIsAlive;
}

void URTSDKUnitComponent::SetUnitInput(FRTSDKScriptExecutionContext inContext, FVector inDir)
{
	/*if (OwningSim->IsScriptCallingMode())
	{
		inContext.ScriptCommandBuffer->PushCommand<FRTSDKSetUnitInputScriptCommand>(UnitEntityHandle, (FRTSVector64)inDir);
	}*/
}
