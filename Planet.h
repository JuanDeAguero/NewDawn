// Copyright (c) Parallax 2022

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Planet.generated.h"

UCLASS()
class NEWDAWN_API APlanet : public AActor
{
	GENERATED_BODY()

public:

	UPROPERTY( BlueprintReadWrite, EditAnywhere ) class UStaticMeshComponent* ScanningSphere;
	
protected:
	
	UPROPERTY( BlueprintReadWrite, EditAnywhere ) class USceneComponent*      Root;
	UPROPERTY( BlueprintReadWrite, EditAnywhere ) class UStaticMeshComponent* PixelSphere;
	UPROPERTY( BlueprintReadWrite, EditAnywhere ) class UStaticMeshComponent* CursorSphere;

private:

	FTimerHandle DistanceTimer, RotateTimer;
	bool Gate1a, Gate1b, Gate2a, Gate2b, Gate3a, Gate3b, Gate4a, Gate4b;

	class APlayerStateND* PS;
    class APlayerControllerND* PC;

	class AUniverse*         Universe;
	class AVoxelWorld*       VoxelWorld;
	class AVoxelWorld*       VoxelWorldFlat;
	class ADirectionalLight* DirectionalLight;
	class ASkyAtmosphere*    Atmosphere;
	class AStaticMeshActor*  WaterSphere;
	class AStaticMeshActor*  CloudsSphere;

    float StartingAngle;

public:
	
	APlanet();
	
	// Scale PixelSphere to have the size of one pixel on screen.
	void ScalePixelSphere();

protected:
	
	virtual void BeginPlay() override;

private:

	void CheckDistance();
    void Rotate();
    void RotatePawn( bool entering );

};