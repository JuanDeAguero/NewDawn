// Copyright (c) Parallax 2023

#pragma once

#include "SaveGamePlayer.h"
#include "SaveGameServer.h"

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GameInstanceND.generated.h"

UCLASS()
class NEWDAWN_API UGameInstanceND : public UGameInstance
{
	GENERATED_BODY()

public:

    UPROPERTY( BlueprintReadWrite, EditAnywhere ) FString SaveGamePlayerSlot;
    UPROPERTY( BlueprintReadWrite, EditAnywhere ) FString SaveGameServerSlot;

public:

    UGameInstanceND();
	
};