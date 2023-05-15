// Copyright (c) Parallax 2023

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SavedGames.generated.h"

UCLASS()
class NEWDAWN_API USavedGames : public USaveGame
{
	GENERATED_BODY()

public:

    UPROPERTY( BlueprintReadWrite, EditAnywhere )
    TArray<FString> Games;

public:

    USavedGames();
	
};