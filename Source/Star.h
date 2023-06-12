// Copyright (c) Parallax 2022

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Star.generated.h"

UCLASS()
class NEWDAWN_API AStar : public AActor
{
	GENERATED_BODY()

public:

    float Angle;
    
protected:

	UPROPERTY( BlueprintReadWrite, EditAnywhere ) class USceneComponent*      Root;
	UPROPERTY( BlueprintReadWrite, EditAnywhere ) class UStaticMeshComponent* Sphere;
	UPROPERTY( BlueprintReadWrite, EditAnywhere ) class UStaticMeshComponent* PixelSphere;
	UPROPERTY( BlueprintReadWrite, EditAnywhere ) class UStaticMeshComponent* CursorSphere;
	
private:

	bool Gate1a, Gate1b, Gate2a, Gate2b;
	FTimerHandle DistanceTimer, RotationTimer;
	class APlayerStateND* PS;
    class APlayerControllerND* PC;
	class AUniverse* Universe;
	class ADirectionalLight* DirectionalLight;

public:
	
	AStar();

	// Scale PixelSphere to have the size of one pixel on screen.
	// If the star is too far from the camera it will be smaller than a pixel and won't be rendered.
	void ScalePixelSphere();

    void RotateAroundPlanet();

protected:
	
	virtual void BeginPlay() override;

private:

	void CheckDistance();

};