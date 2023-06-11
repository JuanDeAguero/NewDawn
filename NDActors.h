// Copyright (c) Parallax 2022-2023

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NDActors.generated.h"

UCLASS()
class NEWDAWN_API AStars : public AActor
{
    GENERATED_BODY()

public:

    AStars()
    {
        PrimaryActorTick.bCanEverTick = false;
    }

    virtual void BeginPlay() override
    {
        Super::BeginPlay();

    }

};

UCLASS()
class NEWDAWN_API AStar : public AActor
{
    GENERATED_BODY()

public:

    AStar()
    {
        PrimaryActorTick.bCanEverTick = false;
    }

    virtual void BeginPlay() override
    {
        Super::BeginPlay();

    }

};

UCLASS()
class NEWDAWN_API APlanet : public AActor
{
    GENERATED_BODY()

public:

    APlanet()
    {
        PrimaryActorTick.bCanEverTick = false;
    }

    virtual void BeginPlay() override
    {
        Super::BeginPlay();

    }

};