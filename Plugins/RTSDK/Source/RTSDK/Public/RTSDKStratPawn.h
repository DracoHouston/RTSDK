// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "RTSDKStratPawn.generated.h"

class UUserWidget;
class UInputMappingContext;

/**
* Base class for player pawns in RTSDK
* Within RTSDK's sim are mass entities and their representative actors
* which are kind of hard to simply 'possess' in the unreal sense. 
* To keep compatibility between the lockstep-deterministic code and server authoritative code
* players directly own a pawn that is a subclass of this. Possessing a gameplay affecting unit
* is done through another system. This pawn is meant to represent the typical disembodied camera
* of a strategy game. 
* 
* Child classes of this are responsible for setting up the camera and the components for player input.
* Movement component defaults to RTSDK Strat Movement Component, client side authoritative movement.
* All players periodically report position and rotation to other players and their copy of this will
* interpolate between.
*/
UCLASS(abstract)
class RTSDK_API ARTSDKStratPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ARTSDKStratPawn();

public:

	UPROPERTY(Transient)
		TObjectPtr<UInputMappingContext> DefaultInputMappingContext;

	UPROPERTY(Transient)
		TObjectPtr<UUserWidget> PlayerHUD;

	

	virtual void PossessedBy(AController* inController) override;
	virtual void OnRep_Controller() override;
};
