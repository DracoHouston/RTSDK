// Copyright Epic Games, Inc. All Rights Reserved.

#include "RTSDKUIHelpers.h"
#include "RTSDKUITypes.h"
#include "RTSDKCommanderState.h"
#include "RTSDKUnitComponent.h"

FVector2D URTSDKUIHelpers::GetDPIScaledScreenspacePosition(FVector2D inAbsolutePosition, double inDPIScale)
{
    const double oneoverdpiscale = 1.0 / inDPIScale;
    return FVector2D(inAbsolutePosition.X * oneoverdpiscale, inAbsolutePosition.Y * oneoverdpiscale);
}

int32 URTSDKUIHelpers::GetAliveCountFromUnitCollection(const FRTSDKUIUnitsCollection& inUnits)
{
    int32 count = 0;
    for (int32 i = 0; i < inUnits.Items.Num(); i++)
    {
        if ((inUnits.Items[i].Unit != nullptr) && (inUnits.Items[i].Unit->GetUnitIsAlive()))
        {
            count++;
        }
    }
    return count;
}
