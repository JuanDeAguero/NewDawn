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

// ====================================================================================
public:

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

// ====================================================================================
protected:

    /* Reference to the input mapping context. To be set in editor. */
    UPROPERTY(EditDefaultsOnly)
    class UInputMappingContext* InputMappingContext;

    /* Blueprint class of the player character. To be set in editor. */
    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<ACharacterND> CharacterClass;

    // --------------------------------------------------------------------------------
    
    /**
     * Initial player setup and spawns the first character to possess.
     */
    virtual void BeginPlay() override;

    /**
     * Called every frame.
     */
    virtual void Tick( float deltaTime ) override;

// ====================================================================================
private:

    // -- Coordinate System -----------------------------------------------------------

    /* Player local offset. When "RebaseOrigin" is called this will change. */
    FVector64 Offset;

    /* Player is rebasing. See the method "RebaseOrigin" for more info. */
    bool Rebasing;

    // -- First Character Id ----------------------------------------------------------

    /* Id of the first character to possess when starting the game.
       Set by the server after calling "Server_SpawnCharacter". */
    UPROPERTY(Replicated)
    int64 FirstCharacterId;

    /* Timer used to find the first character to possess when starting the game. */
    FTimerHandle FirstCharacterTimer;

    // -- Stars -----------------------------------------------------------------------

    /* Actor holding the intanced static meshes for the stars. */
    class AStars* Stars;

    /* Main star actor. It is moved around the universe and placed at the closest star position. */
    class AStar* Star;

    float StarAngle;

    /* Timer used for the rotation of the star around the planet, when the player is in the planet. */
    FTimerHandle StarRotationTimer;
    
    // -- Planets ---------------------------------------------------------------------

    /* Planet actors. Used for scanning and placing the voxel planets. */
    TArray< class APlanet* > Planets;

    /* Reference to the voxel world for the planet placed in the level. */
    class AVoxelWorld* VoxelPlanet;

    /* Reference to the voxel world for the planet preview placed in the level. */
    class AVoxelWorld* VoxelPlanetPreview;

    /* Reference to the voxel world for the ocean placed in the level. */
    class AVoxelWorld* VoxelOcean;

    /* Player is in the current planet. */
    bool InPlanet;

    /* Planet preview is visible.
       When the preview is visible, the actual voxel planet is not visible (and vice versa).
       Note that the player can be in a planet and the preview still visible. */
    bool PlanetPreview;

    /* Player teleported to a planet.
       "CheckPlanetDistance" will see that the player is in the current planet and will set this value to false. */
    bool TeleportedToPlanet;

    FRotator StartRotation;

    FRotator TargetRotation;

    float StartingAngle;

    /* Timer used for the rotation of the planet, when the player is not in the planet. */
    FTimerHandle PlanetRotationTimer;

    class UDoOnce* DoOnceEnterPlanet;

    class UDoOnce* DoOnceExitPlanet;

    // -- Sky -------------------------------------------------------------------------

    class ASkyAtmosphere* SkyAtmosphere;
    
    class AActor* CloudsSphere;

    class AActor* Skybox;

    // -- Lights ----------------------------------------------------------------------

    class ADirectionalLight* DirectionalLight;

    class ADirectionalLight* DirectionalLightNight;

    class ASkyLight* SkyLight;

    // -- Scan ------------------------------------------------------------------------

    /* Player is scanning to find a destination for travel. */
    bool Scanning;

    /* Timer used when the player is scanning. */
    FTimerHandle ScanTimer;

    /* Cursor has hit a target destination when scanning. */
    bool CursorHit;

    // -- Destination -----------------------------------------------------------------

    /* A destination has been set by the player when scanning.
       See "EDestinationType" for the types of destinations. (star, planet, space station...) */
    bool DestinationSet;

    FLocation64 Destination;

    enum class EDestinationType
    {
        Star, Planet, Moon, SpaceStation
    };

    EDestinationType DestinationType;

    // -- Travel ----------------------------------------------------------------------

    /* Player is currently traveling to a set destination. */
    bool Traveling;

    /* Timer used when the player is traveling. */
    FTimerHandle TravelTimer;

    // -- Widgets ---------------------------------------------------------------------

    class UGameWidget* GameWidget;

    // --------------------------------------------------------------------------------

    void SetupInputMapping();

    void SetWorldReferences();

    void SetupTimers();

    void SkyAtmosphereCommands();

    void SpawnFirstCharacter();

    UFUNCTION( Server, Reliable )
    void Server_SpawnCharacter();
    void Server_SpawnCharacter_Implementation();

    UFUNCTION( Server, Reliable )
    void Server_Possess( class APawnND* pawnND );
    void Server_Possess_Implementation( class APawnND* pawnND );

    UFUNCTION( Server, Reliable )
    void Server_UnPossess();
    void Server_UnPossess_Implementation();

    void CheckPlanetDistance();

    void Server_SetFirstCharacterId_Implementation( int64 id );

};