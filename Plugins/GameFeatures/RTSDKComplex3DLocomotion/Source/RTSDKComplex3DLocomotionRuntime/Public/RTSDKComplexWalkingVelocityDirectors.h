// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RTSDKGameSimProcessorBase.h"
#include "RTSDKComplexWalkingVelocityDirectors.generated.h"

/**
*
*/
UCLASS()
class URTSDKComplexWalkingBasedVelocityDirector : public URTSDKModGameSimProcessorBase
{
	GENERATED_BODY()
public:
	//sets some defaults in the class default object
	URTSDKComplexWalkingBasedVelocityDirector();
	
protected:
	//Function that configures the queries, after Initialize is run.
	virtual void ConfigureQueries() override;
	virtual void Execute(FMassEntityManager& EntityManager, FMassExecutionContext& Context) override;

	//Queries for specific taunters by current taunt
	FMassEntityQuery MovementQuery;	
};

/**
*
*/
UCLASS()
class URTSDKComplexWalkingAirVelocityDirector : public URTSDKModGameSimProcessorBase
{
	GENERATED_BODY()
public:
	//sets some defaults in the class default object
	URTSDKComplexWalkingAirVelocityDirector();

protected:
	//Function that configures the queries, after Initialize is run.
	virtual void ConfigureQueries() override;
	virtual void Execute(FMassEntityManager& EntityManager, FMassExecutionContext& Context) override;

	//Queries for specific taunters by current taunt
	FMassEntityQuery MovementQuery;
};

/**
*
*/
UCLASS()
class URTSDKComplexWalkingAngularVelocityDirector : public URTSDKModGameSimProcessorBase
{
	GENERATED_BODY()
public:
	//sets some defaults in the class default object
	URTSDKComplexWalkingAngularVelocityDirector();

protected:
	//Function that configures the queries, after Initialize is run.
	virtual void ConfigureQueries() override;
	virtual void Execute(FMassEntityManager& EntityManager, FMassExecutionContext& Context) override;

	//Queries for specific taunters by current taunt
	FMassEntityQuery MovementQuery;
};
