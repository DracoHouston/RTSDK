// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MassTranslator.h"
#include "RTSDKVisRootInterpolationTranslator.generated.h"

/**
 * 
 */
UCLASS()
class URTSDKVisRootInterpolationTranslator : public UMassTranslator
{
	GENERATED_BODY()
public:
	//sets some defaults in the class default object
	URTSDKVisRootInterpolationTranslator();
	
	//Function where we subscribe to a signal
	virtual void Initialize(UObject& Owner) override;
protected:
	//Function that configures the queries, after Initialize is run.
	virtual void ConfigureQueries() override;
	virtual void Execute(FMassEntityManager& EntityManager, FMassExecutionContext& Context) override;

	//Queries for specific taunters by current taunt
	FMassEntityQuery InterpolationQuery;
};
