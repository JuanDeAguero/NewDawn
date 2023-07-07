// Copyright (c) Parallax 2022-2023

#pragma once

#include "ActorND.h"
#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "PawnND.h"

#include "GameModeND.generated.h"

UCLASS()
class NEWDAWN_API AGameModeND : public AGameMode
{
    GENERATED_BODY()

protected:

    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<AActorND> SpaceStationAClass;

public:

    UFUNCTION( Server, Reliable )
    void Server_SpawnActor( TSubclassOf<AActorND> actorClass, const FLocation64& location, const FRotator& rotation );

    UFUNCTION( Server, Reliable )
    void Server_SpawnPawn( TSubclassOf<APawnND> pawnClass, const FLocation64& location, const FRotator& rotation,
                           class APlayerControllerND* controller );

protected:

    virtual void BeginPlay() override;

private:

    void Server_SpawnActor_Implementation( TSubclassOf<AActorND> actorClass, const FLocation64& location, const FRotator& rotation );

    void Server_SpawnPawn_Implementation( TSubclassOf<APawnND> pawnClass, const FLocation64& location, const FRotator& rotation,
                                          class APlayerControllerND* controller );

    int64 CreateActorId();

};