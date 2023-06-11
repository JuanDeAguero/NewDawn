// Copyright (c) Parallax 2022-2023

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Planet.generated.h"

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