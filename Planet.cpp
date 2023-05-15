// Copyright (c) Parallax 2022

#include "Planet.h"
#include "VoxelWorld.h"
#include "PlayerStateND.h"
#include "Engine/DirectionalLight.h"
#include "Components/SkyAtmosphereComponent.h"
#include "Engine/StaticMeshActor.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Universe.h"
#include "Spaceship.h"
#include "Star.h"
#include "PlayerControllerND.h"
#include "Spaceship.h"
#include "GameFramework/SpringArmComponent.h"

APlanet::APlanet()
    : Gate1a(true), Gate1b(true),
      Gate2a(true), Gate2b(true),
      Gate3a(true), Gate3b(true),
      Gate4a(true), Gate4b(false),
      StartingAngle(0)
{
    PrimaryActorTick.bCanEverTick = false;

    Root           = CreateDefaultSubobject<USceneComponent>(     TEXT("Root"));
    PixelSphere    = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PixelSphere"));
    CursorSphere   = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CursorSphere"));
    ScanningSphere = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ScanningSphere"));

    RootComponent = Root;

    PixelSphere->   SetupAttachment(Root);
    CursorSphere->  SetupAttachment(Root);
    ScanningSphere->SetupAttachment(Root);

    ScanningSphere->SetHiddenInGame(true);
    
    PixelSphere->   CastShadow = false;
    CursorSphere->  CastShadow = false;
    ScanningSphere->CastShadow = false;
}

void APlanet::BeginPlay()
{
    Super::BeginPlay();

    PS = Cast<APlayerStateND>( GetWorld()->GetFirstPlayerController()->PlayerState );
    PC = Cast<APlayerControllerND>( GetWorld()->GetFirstPlayerController() );

    TArray<AActor*> universe, voxelWorld, voxelWorldFlat, directionalLight, atmosphere, waterSphere, cloudsSphere;
    UGameplayStatics::GetAllActorsOfClass( GetWorld(), AUniverse::StaticClass(), universe );
    UGameplayStatics::GetAllActorsWithTag( GetWorld(), "normal", voxelWorld );
    UGameplayStatics::GetAllActorsWithTag( GetWorld(), "flat", voxelWorldFlat );
    UGameplayStatics::GetAllActorsOfClass( GetWorld(), ADirectionalLight::StaticClass(), directionalLight );
    UGameplayStatics::GetAllActorsOfClass( GetWorld(), ASkyAtmosphere::StaticClass(), atmosphere );
    UGameplayStatics::GetAllActorsWithTag( GetWorld(), "water", waterSphere );
    UGameplayStatics::GetAllActorsWithTag( GetWorld(), "clouds", cloudsSphere );
    
    Universe         = Cast<AUniverse>(universe[0]);
    VoxelWorld       = Cast<AVoxelWorld>(voxelWorld[0]);
    VoxelWorldFlat   = Cast<AVoxelWorld>(voxelWorldFlat[0]);
    DirectionalLight = Cast<ADirectionalLight>(directionalLight[0]);
    Atmosphere       = Cast<ASkyAtmosphere>(atmosphere[0]);
    WaterSphere      = Cast<AStaticMeshActor>(waterSphere[0]);
    CloudsSphere     = Cast<AStaticMeshActor>(cloudsSphere[0]);

    GetWorld()->GetTimerManager().SetTimer( DistanceTimer, this, &APlanet::CheckDistance, 0.1f, true );
    GetWorld()->GetTimerManager().SetTimer( RotateTimer, this, &APlanet::Rotate, 0.1f, true );
}

void APlanet::ScalePixelSphere()
{
    // get viewport size
    int32 viewportX, viewportY;
    float viewportSize = 0;
    GetWorld()->GetFirstPlayerController()->GetViewportSize( viewportX, viewportY );
    viewportSize = viewportX * viewportY;
    if ( viewportSize == 0 ) return;

    // find scale based on viewport size and planet distance to pawn
    APawn* pawn = GetWorld()->GetFirstPlayerController()->GetPawn();
    float distance = FVector::Dist( pawn->GetActorLocation(), GetActorLocation() );
    float scale = ( distance * 40 ) / viewportSize;
    scale *= 0.5f;

    // scale the spheres
    PixelSphere->   SetWorldScale3D(FVector(scale));
    CursorSphere->  SetWorldScale3D(FVector( scale * 20 ));
    ScanningSphere->SetWorldScale3D(FVector( scale * 10 ));
}

void APlanet::CheckDistance()
{
    if (!PS) return;
    
    APawn* pawn = UGameplayStatics::GetPlayerPawn( GetWorld(), 0 );
    float distance = FVector::Distance( GetActorLocation(), pawn->GetActorLocation() );

    bool condition1 = distance < 5 * FMath::Pow( 10, 10 );
    if ( condition1 && Gate1a )
    {
        // rotate directional light towards planet
        FRotator rotation = UKismetMathLibrary::FindLookAtRotation( PS->ClosestStar->GetActorLocation(), GetActorLocation() );
        DirectionalLight->SetActorRotation(rotation);

        // place voxel world at planet's location
        TArray<AActor*> rebaseActors;
        UGameplayStatics::GetAllActorsWithTag( GetWorld(), "rebase", rebaseActors );
        for ( AActor* actor : rebaseActors )
        {
            actor->SetActorHiddenInGame(false);
            actor->SetActorLocation(GetActorLocation());
        }

        ScanningSphere->SetHiddenInGame(true);
        CursorSphere->SetHiddenInGame(true);
        PS->ClosestPlanet = this;

        Gate1a = false;
        Gate1b = true;
    }
    if ( !condition1 && Gate1b && PS->ClosestPlanet == this )
    {
        CursorSphere->SetHiddenInGame(false);
        PS->InPlanetOrbit = false;

        Gate1b = false;
        Gate1a = true;
    }

    bool condition2 = distance < 2147483647;
    if ( condition2 && Gate2a )
    {
        Universe->RebaseOrigin(VoxelWorld->GetActorLocation());
        TArray<AActor*> rebaseActors;
        UGameplayStatics::GetAllActorsWithTag( GetWorld(), "rebase", rebaseActors );
        for ( AActor* actor : rebaseActors )
            actor->SetActorLocation(FVector(0));

        Cast<ASpaceship>(pawn)->Speed = 1;
        PS->SpaceTraveling = false;
        PS->InPlanetOrbit = true;

        Gate2a = false;
        Gate2b = true;
    }
    if ( !condition2 && Gate2b )
    {
        Gate2b = false;
        Gate2a = true;
    }

    bool condition3 = distance < 100500000;
    if ( condition3 && Gate3a )
    {
        PS->InPlanet = true;

        Gate3a = false;
        Gate3b = true;
    }
    if ( !condition3 && Gate3b && PS->ClosestPlanet == this )
    {
        PS->InPlanet = false;

        Gate3b = false;
        Gate3a = true;
    }
    
    bool condition4 = distance < 130000000;
    if ( condition4 && Gate4a )
    {
        StartingAngle = PC->GetTimedAngle();

        VoxelWorld->SetActorRotation(FRotator(0));
        CloudsSphere->SetActorRotation(FRotator(0));

        if (PS->PlanetSpinning)
        {
            PS->PlanetSpinning = false;

            FVector pawnLocation = PC->RotateAroundOrigin( PC->GetPawn()->GetActorLocation(), StartingAngle );
            PC->GetPawn()->SetActorLocation(pawnLocation);
            FRotator pawnRotation = PC->GetPawn()->GetActorRotation();
            FRotator rotated = pawnRotation.Vector().RotateAngleAxis( StartingAngle, FVector( 0, 0 , 1 )).Rotation();
            PC->GetPawn()->SetActorRotation(FRotator( pawnRotation.Pitch, rotated.Yaw, pawnRotation.Roll ));

            Cast<AStar>(PS->ClosestStar)->RotateAroundPlanet();

            Cast<ASpaceship>(PC->GetPawn())->SpringArmCameraBack->bEnableCameraRotationLag = false;
            FTimerHandle delay;
            GetWorld()->GetTimerManager().SetTimer( delay, [&]()
            {
                Cast<ASpaceship>(PC->GetPawn())->SpringArmCameraBack->bEnableCameraRotationLag = true;
            }, 0.1f, false );
        }

        Gate4a = false;
        Gate4b = true;
    }
    if ( !condition4 && Gate4b && PS->ClosestPlanet == this && !PS->PlanetSpinning )
    {
        PS->PlanetSpinning = true;

        float angle = ( -1.0f * StartingAngle ) - ( PC->GetTimedAngle() - StartingAngle );
        FVector pawnLocation = PC->RotateAroundOrigin( PC->GetPawn()->GetActorLocation(), angle );
        PC->GetPawn()->SetActorLocation(pawnLocation);
        FRotator pawnRotation = PC->GetPawn()->GetActorRotation();
        FRotator rotated = pawnRotation.Vector().RotateAngleAxis( angle, FVector( 0, 0 , 1 )).Rotation();
        PC->GetPawn()->SetActorRotation(FRotator( pawnRotation.Pitch, rotated.Yaw, pawnRotation.Roll ));

        AStar* star = Cast<AStar>(PS->ClosestStar);
        star->SetActorLocation(PC->RotateAroundOrigin( star->GetActorLocation(), angle ));
        DirectionalLight->SetActorRotation(
            UKismetMathLibrary::FindLookAtRotation( star->GetActorLocation(), PS->ClosestPlanet->GetActorLocation() ));
        star->Angle = 0;

        Gate4b = false;
        Gate4a = true;
    }
}

void APlanet::Rotate()
{
    if ( PS->PlanetSpinning && PS->ClosestPlanet == this )
    {
        float angle = PC->GetTimedAngle() * -1.0f;
        VoxelWorld->SetActorRotation(FRotator( 0, angle, 0 ));
        CloudsSphere->SetActorRotation(FRotator( 0, angle, 0 ));
    }
}