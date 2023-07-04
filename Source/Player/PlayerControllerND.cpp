// Copyright (c) Parallax 2022-2023

#include "PlayerControllerND.h"

#include "EnhancedInputSubsystems.h"
#include "GameModeND.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

APlayerControllerND::APlayerControllerND()
{
    PlanetPreview = true;
}

void APlayerControllerND::BeginPlay()
{
    Super::BeginPlay();

    if (IsLocalPlayerController())
    {
        SetupInputMapping();
        SetWorldReferences();
        SetupTimers();
        SkyAtmosphereCommands();
        SpawnFirstCharacter();
    }
}

void APlayerControllerND::Tick( float deltaTime )
{
    Super::Tick(deltaTime);

    CheckPlanetDistance();
}

void APlayerControllerND::GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& props ) const
{
    Super::GetLifetimeReplicatedProps(props);
    TArray<FLifetimeProperty>& OutLifetimeProps = props;
    DOREPLIFETIME( APlayerControllerND, FirstCharacterId );
}

void APlayerControllerND::RebaseOrigin( FVector location )
{

}

FVector64 APlayerControllerND::GetOffset()
{
    return Offset;
}

void APlayerControllerND::Server_SetFirstCharacterId_Implementation( int64 id )
{
    if ( id != 0 && FirstCharacterId == 0 )
        FirstCharacterId = id;
}

void APlayerControllerND::SetupInputMapping()
{
    if (!InputMappingContext) return;
    if (!GetLocalPlayer()) return;
    UEnhancedInputLocalPlayerSubsystem* subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
    if (!subsystem) return;
    subsystem->AddMappingContext( InputMappingContext, 0 );
}

void APlayerControllerND::SetWorldReferences()
{

}

void APlayerControllerND::SetupTimers()
{

}

void APlayerControllerND::SkyAtmosphereCommands()
{
    ConsoleCommand("r.SkyAtmosphere.SampleCountMax 64");
    ConsoleCommand("r.SkyAtmosphere.FastSkyLUT 0");
    ConsoleCommand("r.SkyAtmosphere.AerialPerspectiveLUT.FastApplyOnOpaque 0");
    ConsoleCommand("r.SkyAtmosphere.AerialPerspectiveLUT.Depth 96");
}

void APlayerControllerND::SpawnFirstCharacter()
{
    Server_SpawnCharacter();

    FTimerDelegate timerDelegate;
    timerDelegate.BindLambda( [this]
    {
        if ( FirstCharacterId == 0 ) return;
        TArray<AActor*> foundActors;
        ACharacterND* foundCharacter = nullptr;
        UGameplayStatics::GetAllActorsOfClass( GetWorld(), ACharacterND::StaticClass(), foundActors );
        for ( AActor* actor : foundActors )
        {
            ACharacterND* character = Cast<ACharacterND>(actor);
            if ( character && character->IsActorInitialized() && character->GetId() == FirstCharacterId )
            {
                foundCharacter = character;
                break;
            }
        }
        if (foundCharacter)
        {
            Server_Possess(foundCharacter);
            GetWorld()->GetTimerManager().ClearTimer(FirstCharacterTimer);
        }
    });
    GetWorld()->GetTimerManager().SetTimer( FirstCharacterTimer, timerDelegate, 0.1f, true );
}

void APlayerControllerND::Server_SpawnCharacter_Implementation()
{
    AGameModeND* gameMode = Cast<AGameModeND>(UGameplayStatics::GetGameMode(GetWorld()));
    FLocation64 location = { FVector( 0.0f, 0.0f, 5.0f ), { 0, 0, 0 } };
    FRotator rotation = FRotator( 0.0f, 0.0f, 0.0f );
    gameMode->Server_SpawnPawn( CharacterClass, location, rotation, this );
}

void APlayerControllerND::Server_Possess_Implementation( class APawnND* pawnND )
{
    Possess(pawnND);
}

void APlayerControllerND::CheckPlanetDistance()
{
    
}