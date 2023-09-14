// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MassEntityTraitBase.h"
#include "RTSDKTypes.h"
#include "RTSDKConstants.h"
#include "FixedPointNumbers.h"
#include "RTSDKMovementCoreTraits.generated.h"

/**
*
*/
UCLASS()
class URTSVisRootInterpolationTrait : public UMassEntityTraitBase
{
public:
	GENERATED_BODY()

protected:
	//Where the magic happens
	virtual void BuildTemplate(
		FMassEntityTemplateBuildContext& BuildContext,
		const UWorld& World) const override;
};
