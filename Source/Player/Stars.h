// Copyright (c) Parallax 2022-2023

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Stars.generated.h"

UCLASS()
class NEWDAWN_API AStars : public AActor
{
    GENERATED_BODY()

public:

    AStars();

protected:

    virtual void BeginPlay() override;

};