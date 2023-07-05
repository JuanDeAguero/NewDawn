// Copyright (c) Parallax 2022-2023

#include "GameModeND.h"

#include "PlayerControllerND.h"

void AGameModeND::BeginPlay()
{
    Super::BeginPlay();

    Server_SpawnActor( SpaceStationAClass, { FVector(), { 0, 0, 0 } }, FRotator() );
}

void AGameModeND::Server_SpawnActor_Implementation( TSubclassOf<AActorND> actorClass, const FLocation64& location,
                                                    const FRotator& rotation )
{
    FActorSpawnParameters spawnParams;
    spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    AActorND* actor = GetWorld()->SpawnActor<AActorND>( actorClass, FVector(), FRotator(), spawnParams );

    actor->Server_SetId(CreateActorId());
    actor->Server_SetLocation( location, false );
    actor->Server_SetRotation( rotation, false );
}

void AGameModeND::Server_SpawnPawn_Implementation( TSubclassOf<APawnND> pawnClass, const FLocation64& location, const FRotator& rotation,
                                                   APlayerControllerND* controller )
{
    FActorSpawnParameters spawnParams;
    spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    APawnND* pawn = GetWorld()->SpawnActor<APawnND>( pawnClass, FVector(), FRotator(), spawnParams );

    int64 id = CreateActorId();
    pawn->Server_SetId(id);

    pawn->Server_SetLocation( location, false );
    pawn->Server_SetRotation( rotation, false );

    if (controller)
        controller->Server_SetFirstCharacterId(id);
}

int64 AGameModeND::CreateActorId()
{
    int64 min = 0;
    int64 max = 10000;
    return FMath::RandRange( min, max );
}
