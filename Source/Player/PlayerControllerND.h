// Copyright (c) Parallax 2022-2023

#pragma once

#include "CharacterND.h"
#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "UtilsND.h"

#include "PlayerControllerND.generated.h"

UCLASS()
class NEWDAWN_API APlayerControllerND : public APlayerController
{
    GENERATED_BODY()

public:

    APlayerControllerND();

    void RebaseOrigin( FVector location );

    FVector64 GetOffset();

    UFUNCTION( Server, Reliable )
    void Server_SetFirstCharacterId( int64 id );
    
protected:

    UPROPERTY(EditDefaultsOnly)
    class UInputMappingContext* InputMappingContext;

    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<ACharacterND> CharacterClass;
    
    virtual void BeginPlay() override;

    virtual void Tick( float deltaTime ) override;
    
private:

    FVector64 Offset;

    bool Rebasing;
    
    UPROPERTY(Replicated)
    int64 FirstCharacterId;

    FTimerHandle FirstCharacterTimer;
    
    class AStars* Stars;

    class AStar* Star;

    float StarAngle;

    FTimerHandle StarRotationTimer;
    
    TArray< class APlanet* > Planets;

    class AVoxelWorld* VoxelPlanet, *VoxelPlanetPreview, *VoxelOcean;

    bool InPlanet;

    bool PlanetPreview;

    bool TeleportedToPlanet;

    float StartingAngle;

    FRotator StartRotation, TargetRotation;
    
    FTimerHandle PlanetRotationTimer;

    class UDoOnce* DoOnceEnterPlanet, *DoOnceExitPlanet;

    class ASkyAtmosphere* SkyAtmosphere;
    
    class AActor* CloudsSphere;

    class AActor* Skybox;

    class ADirectionalLight* DirectionalLight;

    class ADirectionalLight* DirectionalLightNight;

    class ASkyLight* SkyLight;

    bool Scanning;

    FTimerHandle ScanTimer;

    bool CursorHit;

    bool DestinationSet;

    FLocation64 Destination;

    enum class EDestinationType
    {
        Star, Planet, Moon, SpaceStation
    };

    EDestinationType DestinationType;

    bool Traveling;

    FTimerHandle TravelTimer;

    class UGameWidget* GameWidget;

    void SetupInputMapping();

    void SetWorldReferences();

    void SetupTimers();

    void SkyAtmosphereCommands();

    void SpawnFirstCharacter();

    UFUNCTION( Server, Reliable )
    void Server_SpawnCharacter();

    UFUNCTION( Server, Reliable )
    void Server_Possess( class APawnND* pawnND );

    UFUNCTION( Server, Reliable )
    void Server_UnPossess();
    
    void CheckPlanetDistance();

    void Server_SetFirstCharacterId_Implementation( int64 id );

    void Server_SpawnCharacter_Implementation();

    void Server_Possess_Implementation( class APawnND* pawnND );

    void Server_UnPossess_Implementation();
    
};