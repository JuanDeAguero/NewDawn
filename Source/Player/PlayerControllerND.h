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

    /* ... */
    bool Scanning;

    /* ... */
    bool CursorHit;

    /* ... */
    bool DestinationSet;

    /* ... */
    bool Traveling;

protected:

    /* Editor reference to the input mapping context. To be set in editor. */
    UPROPERTY(EditDefaultsOnly)
    class UInputMappingContext* InputMappingContext;

    /* Blueprint class of the player character. To be set in editor. */
    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<ACharacterND> CharacterClass;

private:
    
    /* Player local offset. When "RebaseOrigin" is called this will change. */
    FVector64 Offset;

    /* ... */
    bool Rebasing;

    /* Id of the first character to possess when starting the game.
       Set by the server after calling "Server_SpawnCharacter". */
    UPROPERTY(Replicated)
    int64 FirstCharacterId;

    /* Timer used to find the first character to possess when starting the game. */
    FTimerHandle FirstCharacterTimer;

    /* ... */
    FTimerHandle ScanTimer;

    /* ... */
    FTimerHandle TravelTimer;

    /* ... */
    FTimerHandle StarRotationTimer;

    /* ... */
    FTimerHandle PlanetRotationTimer;

    /* ... */
    bool InPlanet;

    /* ... */
    bool PlanetPreview;

    /* ... */
    bool TeleportedToPlanet;

    /* ... */
    class AStars* Stars;

    /* ... */
    class AStar* Star;

    /* ... */
    TArray< class APlanet* > Planets;

    /* ... */
    class ASkyAtmosphere* SkyAtmosphere;
    
    /* ... */
    class AActor* CloudsSphere;

    /* ... */
    class AActor* Skybox;

    /* ... */
    class ADirectionalLight* DirectionalLight;

    /* ... */
    class ADirectionalLight* DirectionalLightNight;

    /* ... */
    class ASkyLight* SkyLight;

    /* Reference to the voxel world for the planet placed in the level. */
    class AVoxelWorld* VoxelPlanet;

    /* Reference to the voxel world for the planet preview placed in the level. */
    class AVoxelWorld* VoxelPlanetPreview;

    /* Reference to the voxel world for the ocean placed in the level. */
    class AVoxelWorld* VoxelOcean;

    /* ... */
    FLocation64 Destination;

    /* ... */
    enum class EDestinationType
    {
        Star, Planet, Moon, SpaceStation
    };

    /* ... */
    EDestinationType DestinationType;

    /* ... */
    FRotator StartRotation;

    /* ... */
    FRotator TargetRotation;

    /* ... */
    float StartingAngle;

    /* ... */
    float StarAngle;

    /* ... */
    class UGameWidget* GameWidget;

    /* ... */
    class UDoOnce* DoOnceEnterPlanet;

    /* ... */
    class UDoOnce* DoOnceExitPlanet;

public:

    /**
     * Constructor. Sets default values.
     */
    APlayerControllerND();

    /**
     * Shifts all actors by the same amount so that "location" becomes the new local origin.
     * The value of "Offset" gets updated accordingly.
     * @note Only affects the local player.
     * @param location Location for the new origin.
     */
    void RebaseOrigin( FVector location );

    /**
     * Gets a copy of private member "Offset".
     * @return Copy of "Offset".
     */
    FVector64 GetOffset();

    /**
     * Sets private member "FirstCharacterId".
     * @note "FirstCharacterId" can only be modified once.
     * @param id Value to set "FirstCharacterId".
     */
    UFUNCTION( Server, Reliable )
    void Server_SetFirstCharacterId( int64 id );

protected:
    
    /**
     * Initial player setup and spawns the first character to possess.
     */
    virtual void BeginPlay() override;

    /**
     * Called every frame.
     */
    virtual void Tick( float deltaTime ) override;

private:

    /**
     * 
     */
    void Server_SetFirstCharacterId_Implementation( int64 id );

    /**
     * 
     */
    void SetupInputMapping();

    /**
     * 
     */
    void SetWorldReferences();

    /**
     * 
     */
    void SetupTimers();

    /**
     * 
     */
    void SkyAtmosphereCommands();

    /**
     * 
     */
    void SpawnFirstCharacter();

    /**
     * 
     */
    UFUNCTION( Server, Reliable )
    void Server_SpawnCharacter();
    void Server_SpawnCharacter_Implementation();

    /**
     * 
     * @param pawnND
     */
    UFUNCTION( Server, Reliable )
    void Server_Possess( class APawnND* pawnND );
    void Server_Possess_Implementation( class APawnND* pawnND );

    /**
     * 
     */
    void CheckPlanetDistance();

};