// Copyright (c) Parallax 2022

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Universe.generated.h"

UCLASS()
class NEWDAWN_API AUniverse : public AActor
{
	GENERATED_BODY()

private:

	// Offset from shifting the orgirin location when calling 'RebaseOrigin'.
	int64 OffsetX, OffsetY, OffsetZ;

	int32 Seed;
	int64 MinStarDistance, MinPlanetDistance;
	float StarProbability, PlanetProbability;
	TArray<FVector> StarLocations;

protected:

	UPROPERTY(EditDefaultsOnly) TSubclassOf<AActor> StarBP;
	UPROPERTY(EditDefaultsOnly) TSubclassOf<AActor> PlanetBP;

public:

	AUniverse();

	// Spawn stars around the player. If the star already exists, don't spawn it again.
	UFUNCTION(BlueprintCallable) void UpdateStars();

	// Spawn planets around the star at location 'starLocation'.
	UFUNCTION(BlueprintCallable) void SpawnPlanets( FVector starLocation );

	// Scale stars and planets that are too far and are smaller than a pixel.
	UFUNCTION(BlueprintCallable) void Scale();

	// Make 'location' the origin in space (0, 0, 0), and shift the other actors accordingly.
	UFUNCTION(BlueprintCallable) void RebaseOrigin( FVector location );

	UFUNCTION(BlueprintCallable) int64 GetOffsetX();
	UFUNCTION(BlueprintCallable) int64 GetOffsetY();
	UFUNCTION(BlueprintCallable) int64 GetOffsetZ();

protected:

	virtual void BeginPlay() override;

private:

	bool  RandomBool ( float x, float y, float z, int64 seed, float probability );
	float RandomFloat( float value, int64 seed, float min, float max );

	// Base case for delayed recursion on 'Scale'.
	// Scale stars and planets when viewport scale is set (on begin play it is set to 0).
	void Scale_BC();

};