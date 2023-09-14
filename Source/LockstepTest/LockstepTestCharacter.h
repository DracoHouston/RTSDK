// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FixedPointNumbers.h"
#include "LockstepTestCharacter.generated.h"

UCLASS(Blueprintable)
class ALockstepTestCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ALockstepTestCharacter();

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FixedPointTest")
		bool testeditcondition;

	UFUNCTION()
		void OnRep_fixedpointtest();
	

	UFUNCTION(BlueprintImplementableEvent)
		void OnFixedPointTestChanged();

	UFUNCTION(BlueprintCallable)
		void DumpConstantsAsFixed();

	UFUNCTION(BlueprintCallable)
		void DumpAtanAsFixed();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FixedPointTest, Replicated, ReplicatedUsing = OnRep_fixedpointtest, meta = (EditCondition = "testeditcondition"))
		FFixed64 testfixednumber;

	/** Returns TopDownCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

private:
	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;
};

