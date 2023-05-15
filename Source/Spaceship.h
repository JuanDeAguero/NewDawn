// Copyright (c) Parallax 2022

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Spaceship.generated.h"

UCLASS()
class NEWDAWN_API ASpaceship : public APawn
{
	GENERATED_BODY()

public:

	float Speed;

	UPROPERTY( BlueprintReadWrite, EditAnywhere ) class USpringArmComponent* SpringArmCameraBack;

protected:

	UPROPERTY( BlueprintReadWrite, EditAnywhere ) class USceneComponent* Root;

private:

	float MaxSpeed;
	FTimerHandle MovementTimer, ScaleTimer, DayNightTimer, SaveTimer;
	class AUniverse* Universe;
	class APlayerStateND* PS;

public:

	ASpaceship();
	virtual void SetupPlayerInputComponent( class UInputComponent* input ) override;

	UFUNCTION(BlueprintCallable)
	float GetSpeed();

protected:

	virtual void BeginPlay() override;

private:

    // Timed methods
	void MoveForward();
	void ScaleUniverse();
    void Save();

	// Action bindings
	void SpeedUp();
	void SpeedDown();
	void ToggleScan();
	void SpaceTravel();

	// Axis bindings
	void Pitch( float axisValue );
	void Roll ( float axisValue );
	void Yaw  ( float axisValue );

};