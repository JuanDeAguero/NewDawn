// Copyright (c) Parallax 2022-2023

#include "PawnND.h"

#include "Net/UnrealNetwork.h"
#include "PlayerControllerND.h"

APawnND::APawnND()
{
    PrimaryActorTick.bCanEverTick = false;
    bReplicates = true;
}

void APawnND::BeginPlay()
{
    Super::BeginPlay();
    OnRep_Location();
    OnRep_Rotation();
}

void APawnND::SetupPlayerInputComponent( UInputComponent* inputComponent )
{
    Super::SetupPlayerInputComponent(inputComponent);
}

void APawnND::GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& props ) const
{
    Super::GetLifetimeReplicatedProps(props);
    TArray<FLifetimeProperty>& OutLifetimeProps = props;
    DOREPLIFETIME( APawnND, Id );
    DOREPLIFETIME_CONDITION( APawnND, Location,       COND_SkipOwner );
    DOREPLIFETIME_CONDITION( APawnND, LocationClient, COND_OwnerOnly );
    DOREPLIFETIME_CONDITION( APawnND, Rotation,       COND_SkipOwner );
    DOREPLIFETIME_CONDITION( APawnND, RotationClient, COND_OwnerOnly );
}

int64 APawnND::GetId()
{
    return Id;
}

FLocation64 APawnND::GetLocation()
{
    return Location;
}

FRotator APawnND::GetRotation()
{
    return Rotation;
}

void APawnND::Server_SetId_Implementation( int64 newId )
{
    Id = newId;
}

void APawnND::Server_SetLocation_Implementation( const FLocation64& newLocation, bool updateClient )
{
    Location = newLocation;
    OnRep_Location();
    if (updateClient)
    {
        LocationClient = newLocation;
        OnRep_LocationClient();
    }
}

void APawnND::Server_SetRotation_Implementation( const FRotator& newRotation, bool updateClient )
{
    Rotation = newRotation;
    OnRep_Rotation();
    if (updateClient)
    {
        RotationClient = newRotation;
        OnRep_RotationClient();
    }
}

void APawnND::OnRep_Location()
{
    UpdateLocation(Location);
}

void APawnND::OnRep_LocationClient()
{
    UpdateLocation(LocationClient);
}

void APawnND::UpdateLocation( FLocation64 location )
{
    APlayerController* playerController = GetWorld()->GetFirstPlayerController();
    if (!playerController) return;
    APlayerControllerND* playerControllerND = Cast<APlayerControllerND>(playerController);
    FVector physicalLocation = VirtualLocationToPhysical( location, playerControllerND->GetOffset() );
    SetActorLocation(physicalLocation);
}

void APawnND::OnRep_Rotation()
{
    SetActorRotation(Rotation);
}

void APawnND::OnRep_RotationClient()
{
    SetActorRotation(RotationClient);
}