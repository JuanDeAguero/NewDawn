// Copyright (c) Parallax 2022-2023

#pragma once

#include "CoreMinimal.h"
#include "Utility.h"

#include "Stars.h"
#include "Star.h"
#include "Planet.h"
#include "GameWidget.h"

#include "VoxelWorld.h"
#include "Components/SkyAtmosphereComponent.h"
#include "Engine/DirectionalLight.h"
#include "Engine/SkyLight.h"
#include "Engine/EngineTypes.h"

#include "GameFramework/PlayerController.h"
#include "PlayerControllerND.generated.h"

UCLASS()
class NEWDAWN_API APlayerControllerND : public APlayerController
{
    GENERATED_BODY()

public:

    UPROPERTY(BlueprintReadWrite)
    bool InPlanet = false;

    UPROPERTY(BlueprintReadWrite)
    bool PlanetPreview = true;

    UPROPERTY(BlueprintReadWrite)
    bool TeleportedToPlanet = false;

    UPROPERTY(BlueprintReadWrite)
    bool Scanning = false;

    UPROPERTY(BlueprintReadWrite)
    bool CursorHit = false;

    UPROPERTY(BlueprintReadWrite)
    bool DestinationSet = false;

    UPROPERTY(BlueprintReadWrite)
    bool Traveling = false;

    UPROPERTY(BlueprintReadWrite)
    bool Rebasing = false;

    UPROPERTY(BlueprintReadWrite)
    bool FirstPawnPossessed = false;

    UPROPERTY(BlueprintReadWrite)
    FTimerHandle ScanTimerHandle;

    UPROPERTY(BlueprintReadWrite)
    FTimerHandle TravelTimerHandle;

    UPROPERTY(BlueprintReadWrite)
    AStars* Stars;

    UPROPERTY(BlueprintReadWrite)
    AStar* Star;

    UPROPERTY(BlueprintReadWrite)
    AVoxelWorld* VoxelPlanet;

    UPROPERTY(BlueprintReadWrite)
    AVoxelWorld* VoxelPlanetPreview;

    UPROPERTY(BlueprintReadWrite)
    AVoxelWorld* VoxelOcean;

    UPROPERTY(BlueprintReadWrite)
    AActor* CloudsSphere;

    UPROPERTY(BlueprintReadWrite)
    AActor* Skybox;

    UPROPERTY(BlueprintReadWrite)
    ASkyLight* SkyLight;

    UPROPERTY(BlueprintReadWrite)
    ASkyAtmosphere* SkyAtmosphere;

    UPROPERTY(BlueprintReadWrite)
    ADirectionalLight* DirectionalLight;

    UPROPERTY(BlueprintReadWrite)
    ADirectionalLight* DirectionalLightNight;

    UPROPERTY(BlueprintReadWrite)
    FLocation64 Destination;

    UPROPERTY(BlueprintReadWrite)
    EDestinationType DestinationType;

    UPROPERTY(BlueprintReadWrite)
    EGravityProfile GravityProfile;

    UPROPERTY(BlueprintReadWrite)
    int64 FirstPawnId = 0;

    UPROPERTY(BlueprintReadWrite)
    float StartingAngle = 0.0f;

    UPROPERTY(BlueprintReadWrite)
    FRotator StartRotation;

    UPROPERTY(BlueprintReadWrite)
    FRotator TargetRotation;

    UPROPERTY(BlueprintReadWrite)
    UGameWidget* GameWidget;

    APlayerControllerND() {}
    
    UFUNCTION(BlueprintCallable)
    virtual void BeginPlay() override
    {
        Super::BeginPlay();
        if (!GetWorld()->IsServer())
        {
            SkyAtmosphereCommands();
            SetupTimers();
            SetupInputMapping();
            SpawnStars();
            SetWorldRefs();
            SetActorsVisibility();
            SpawnFirstCharacter();

            GameWidget = Cast<UGameWidget>(CreateWidget<UGameWidget>( this, UGameWidget::StaticClass() ));
            GameWidget->AddToViewport();
        }
    }

    UFUNCTION(BlueprintCallable)
    void SkyAtmosphereCommands()
    {
        ConsoleCommand("r.SkyAtmosphere.SampleCountMax 64");
        ConsoleCommand("r.SkyAtmosphere.FastSkyLUT 0");
        ConsoleCommand("r.SkyAtmosphere.AerialPerspectiveLUT.FastApplyOnOpaque 0");
        ConsoleCommand("r.SkyAtmosphere.AerialPerspectiveLUT.Depth 96");
    }

    UFUNCTION(BlueprintCallable)
    void SetupTimers()
    {
        FTimerManager& timerManager = GetWorld()->GetTimerManager();
        timerManager.SetTimer( ScanTimerHandle, this, &APlayerControllerND::ScanTimer, 0.1f, true );
        timerManager.SetTimer( TravelTimerHandle, this, &APlayerControllerND::TravelTimer, 0.01f, true);
        timerManager.PauseTimer(ScanTimerHandle);
        timerManager.PauseTimer(TravelTimerHandle);
    }

    UFUNCTION(BlueprintCallable)
    void ScanTimer()
    {
        FHitResult hitResult;
        bool hit = GetHitResultUnderCursorByChannel( UEngineTypes::ConvertToTraceType(ECC_Visibility), true, hitResult );
        if (!hit)
        {
            CursorHit = false;
            return;
        }

        AActor* hitActor = hitResult.GetActor();
        if (!hitActor)
        {
            CursorHit = false;
            return;
        }
        int32 hitItem = hitResult.Item;

        bool isStar = hitActor->IsA(AStars::StaticClass());
        bool isPlanet = hitActor->IsA(APlanet::StaticClass());

        if ( isStar || isPlanet ) CursorHit = true;
        else
        {
            CursorHit = false;
            return;
        }

        if (isStar)
        {
            FLocation64 dest = { FVector(), { 0, 0, 0 } };
            Destination = dest;

        }
        else if (isPlanet)
        {

        }



    }

    UFUNCTION(BlueprintCallable)
    void TravelTimer()
    {

    }

    UFUNCTION(BlueprintCallable)
    void SetupInputMapping()
    {

    }

    UFUNCTION(BlueprintCallable)
    void SpawnStars()
    {

    }
    
    UFUNCTION(BlueprintCallable)
    void SetWorldRefs()
    {

    }

    UFUNCTION(BlueprintCallable)
    void SetActorsVisibility()
    {

    }

    UFUNCTION(BlueprintCallable)
    void SpawnFirstCharacter()
    {

    }

};