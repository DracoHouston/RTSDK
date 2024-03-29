// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RTSDKGameSimProcessorBase.h"
#include "RTSDKComplexWalkingPreMovementProcessors.generated.h"


/**
*
*/
UCLASS()
class URTSDKComplexWalkingGravityChangeProcessor : public URTSDKModGameSimSignalProcessorBase
{
	GENERATED_BODY()
public:
	URTSDKComplexWalkingGravityChangeProcessor();
	
	virtual void Initialize(UObject& Owner) override;
protected:
	virtual void ConfigureQueries() override;
	virtual void SignalEntities(FMassEntityManager& EntityManager, FMassExecutionContext& Context, FMassSignalNameLookup& EntitySignals) override;
};
