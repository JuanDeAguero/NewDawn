// Copyright (c) Parallax 2022

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlayerControllerND.generated.h"

UCLASS()
class NEWDAWN_API APlayerControllerND : public APlayerController
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly) TSubclassOf<AActor> UniverseBP;
	UPROPERTY(EditDefaultsOnly) TSubclassOf<AActor> SpaceshipBP;

private:

    bool Gate1a, Gate1b;
	FTimerHandle DayNightTimer;
	class APlayerStateND* PS;
	class ADirectionalLight* DirectionalLight;

public:

    APlayerControllerND();

	UFUNCTION(BlueprintCallable) FVector RotateAroundOrigin( const FVector& location, float angle );
	UFUNCTION(BlueprintCallable) float GetTimedAngle();
	
protected:

	virtual void BeginPlay() override;

private:

    void CheckDayNight();

};