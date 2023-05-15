// Copyright (c) Parallax 2022

#include "PlayerControllerND.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/DateTime.h"
#include "PlayerStateND.h"
#include "Universe.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/DirectionalLight.h"
#include "Star.h"
#include "Math/UnrealMathUtility.h"
#include "GameInstanceND.h"
#include "SaveGamePlayer.h"
#include "SaveGameServer.h"

APlayerControllerND::APlayerControllerND()
    : Gate1a(true), Gate1b(true) {}

void APlayerControllerND::BeginPlay()
{
    Super::BeginPlay();

    UGameInstanceND* gameInstance = Cast<UGameInstanceND>(GetWorld()->GetGameInstance());
    FString saveGamePlayerSlot = gameInstance->SaveGamePlayerSlot;
    FString saveGameServerSlot = gameInstance->SaveGameServerSlot;
    USaveGamePlayer* saveGamePlayer = Cast<USaveGamePlayer>(UGameplayStatics::LoadGameFromSlot( saveGamePlayerSlot, 0 ));

    PS = Cast<APlayerStateND>(GetWorld()->GetFirstPlayerController()->PlayerState);
    PS->SpaceTraveling = false;
    PS->InStarSystem = false;
    PS->PlanetSpinning = true;

    TArray<AActor*> directionalLight;
    UGameplayStatics::GetAllActorsOfClass( GetWorld(), ADirectionalLight::StaticClass(), directionalLight );
    DirectionalLight = Cast<ADirectionalLight>(directionalLight[0]);

    FVector spawnLocation = FVector(0);
    FRotator spawnRotation = FRotator(0);
    if (saveGamePlayer)
    {
        spawnLocation = saveGamePlayer->Location;
        spawnRotation = saveGamePlayer->Rotation;
    }
    AActor* spaceship = GetWorld()->SpawnActor<AActor>( SpaceshipBP, spawnLocation, spawnRotation );
    Possess(Cast<APawn>(spaceship));

    AActor* universe = GetWorld()->SpawnActor<AActor>( UniverseBP, FVector(0), FRotator(0) );
    Cast<AUniverse>(universe)->UpdateStars();

    // settings for higher resolution atmosphere
    ConsoleCommand("r.SkyAtmosphere.SampleCountMax 64");
    ConsoleCommand("r.SkyAtmosphere.FastSkyLUT 0");
    ConsoleCommand("r.SkyAtmosphere.AerialPerspectiveLUT.FastApplyOnOpaque 0");
    ConsoleCommand("r.SkyAtmosphere.AerialPerspectiveLUT.Depth 96");

    TArray<AActor*> rebaseActors;
    UGameplayStatics::GetAllActorsWithTag( GetWorld(), "rebase", rebaseActors );
    for ( AActor* actor : rebaseActors )
        actor->SetActorHiddenInGame(true);
    
	GetWorld()->GetTimerManager().SetTimer( DayNightTimer, this, &APlayerControllerND::CheckDayNight, 1.0f, true );
}

FVector APlayerControllerND::RotateAroundOrigin( const FVector& location, float angle )
{
    FVector axis = FVector( 0, 0, 1 );
    FVector a = FVector( location.X, location.Y, 0 );
    a.Normalize();
    FVector b = axis * FVector::DotProduct( axis, location );
    float c = FMath::Sqrt( FMath::Pow( location.Size(), 2 ) - FMath::Pow( b.Size(), 2 ));
    FVector d = a * c;
    FVector e = d.RotateAngleAxis( angle, axis );
    return e + b;
}

float APlayerControllerND::GetTimedAngle()
{
    FTimespan now = FDateTime::Now().GetTimeOfDay();
    float milliseconds = now.GetTotalMilliseconds();
    float hours = now.GetTotalHours();
    float a = milliseconds - ( FMath::Floor(hours) * 3600000.0f );
    float b = 360.0f / 3600000.0f;
    return a * b;
}

void APlayerControllerND::CheckDayNight()
{
    if (!PS->InPlanet)
    {
        TArray<AActor*> stars;
        UGameplayStatics::GetAllActorsOfClass( GetWorld(), AStar::StaticClass(), stars );
        for ( AActor* star : stars )
            star->SetActorHiddenInGame(false);
        DirectionalLight->SetCastShadows(true);
        Gate1b = false;
        Gate1a = true;
        return;
    }

    FHitResult hit;
    ECollisionChannel channel;
    FCollisionQueryParams queryParams;
    FCollisionResponseParams responseParams;

    channel = ECollisionChannel::ECC_Visibility;

    TArray<AActor*> voxelWorld;
    UGameplayStatics::GetAllActorsWithTag( GetWorld(), "normal", voxelWorld );
    queryParams.AddIgnoredActor(voxelWorld[0]);
    queryParams.AddIgnoredActor(GetPawn());

    GetWorld()->LineTraceSingleByChannel( hit, GetPawn()->GetActorLocation(), FVector(0), channel, queryParams, responseParams );

    FVector v = PS->ClosestStar->GetActorLocation() - GetPawn()->GetActorLocation();
    v.Normalize();
    float dot = FVector::DotProduct( v, hit.Normal );
    float angle = FMath::FastAsin(dot) * ( 180 / PI );

    TArray<AActor*> stars;
    UGameplayStatics::GetAllActorsOfClass( GetWorld(), AStar::StaticClass(), stars );

    bool condition = angle > 5 && angle < 90;
    if ( condition && Gate1a )
    {
        DirectionalLight->SetCastShadows(true);

        for ( AActor* star : stars )
        {
            if ( star != PS->ClosestStar )
                star->SetActorHiddenInGame(true);
        }

        Gate1a = false;
        Gate1b = true;
    }
    if ( !condition && Gate1b )
    {
        DirectionalLight->SetCastShadows(false);

        for ( AActor* star : stars )
            star->SetActorHiddenInGame(false);

        Gate1b = false;
        Gate1a = true;
    }
}