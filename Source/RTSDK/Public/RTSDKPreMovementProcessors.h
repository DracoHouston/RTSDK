// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MassSignalProcessorBase.h"
#include "RTSDKGameSimProcessorInterface.h"
#include "RTSDKGameSimProcessorBase.h"
#include "RTSDKPreMovementProcessors.generated.h"


/**
*
*/
UCLASS()
class URTSDKGravityChangeProcessor : public UMassSignalProcessorBase, public IRTSDKGameSimProcessorInterface
{
	GENERATED_BODY()
public:
	URTSDKGravityChangeProcessor();
	
	virtual void Initialize(UObject& Owner) override;
protected:
	virtual void ConfigureQueries() override;
	virtual void SignalEntities(FMassEntityManager& EntityManager, FMassExecutionContext& Context, FMassSignalNameLookup& EntitySignals) override;
};

/**
*
*/
UCLASS()
class URTSDKCurrentToPrevious3DTransformProcessor : public URTSDKGameSimProcessorBase
{
	GENERATED_BODY()
public:
	URTSDKCurrentToPrevious3DTransformProcessor();

protected:
	virtual void ConfigureQueries() override;
	virtual void Execute(FMassEntityManager& EntityManager, FMassExecutionContext& Context) override;

	FMassEntityQuery MovementQuery;
};
