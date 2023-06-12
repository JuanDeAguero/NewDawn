// Copyright (c) Parallax 2023

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SaveGamePlayer.generated.h"

UCLASS()
class NEWDAWN_API USaveGamePlayer : public USaveGame
{
	GENERATED_BODY()

public:

    UPROPERTY( BlueprintReadWrite, EditAnywhere ) FVector Location;
    UPROPERTY( BlueprintReadWrite, EditAnywhere ) FRotator Rotation;

public:

    USaveGamePlayer();

};