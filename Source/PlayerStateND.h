// Copyright (c) Parallax 2023

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "PlayerStateND.generated.h"

UCLASS()
class NEWDAWN_API APlayerStateND : public APlayerState
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite) int32   Health;
	UPROPERTY(BlueprintReadWrite) AActor* ClosestStar;
	UPROPERTY(BlueprintReadWrite) AActor* ClosestPlanet;
	UPROPERTY(BlueprintReadWrite) AActor* ScannedStar;
	UPROPERTY(BlueprintReadWrite) AActor* ScannedPlanet;
	UPROPERTY(BlueprintReadWrite) bool    InSpaceship;
	UPROPERTY(BlueprintReadWrite) bool    InStarSystem;
	UPROPERTY(BlueprintReadWrite) bool    InPlanet;
	UPROPERTY(BlueprintReadWrite) bool    InPlanetOrbit;
	UPROPERTY(BlueprintReadWrite) bool    Scanning;
	UPROPERTY(BlueprintReadWrite) bool    ScanningStar;
	UPROPERTY(BlueprintReadWrite) bool    ScanningPlanet;
	UPROPERTY(BlueprintReadWrite) bool    SpaceTraveling;
	UPROPERTY(BlueprintReadWrite) bool    PlanetSpinning;

public:

	APlayerStateND();

};