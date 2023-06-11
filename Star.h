// Copyright (c) Parallax 2022-2023

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Star.generated.h"

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