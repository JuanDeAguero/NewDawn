// Copyright (c) Parallax 2022-2023

#include "ActorND.h"

#include "Net/UnrealNetwork.h"
#include "PlayerControllerND.h"

AActorND::AActorND()
{
    PrimaryActorTick.bCanEverTick = false;
    bReplicates = true;
}

void AActorND::BeginPlay()
{
    Super::BeginPlay();
    
    OnRep_Location();
    OnRep_Rotation();
}

void AActorND::GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& props ) const
{
    Super::GetLifetimeReplicatedProps(props);

    TArray<FLifetimeProperty>& OutLifetimeProps = props;

    DOREPLIFETIME( AActorND, Id );
    DOREPLIFETIME_CONDITION( AActorND, Location,       COND_SkipOwner );
    DOREPLIFETIME_CONDITION( AActorND, LocationClient, COND_OwnerOnly );
    DOREPLIFETIME_CONDITION( AActorND, Rotation,       COND_SkipOwner );
    DOREPLIFETIME_CONDITION( AActorND, RotationClient, COND_OwnerOnly );
}

int64 AActorND::GetId()
{
    return Id;
}

FLocation64 AActorND::GetLocation()
{
    return Location;
}

FRotator AActorND::GetRotation()
{
    return Rotation;
}

void AActorND::Server_SetId_Implementation( int64 newId )
{
    Id = newId;
}

void AActorND::Server_SetLocation_Implementation( const FLocation64& newLocation, bool updateClient )
{
    Location = newLocation;
    OnRep_Location();

    if (updateClient)
    {
        LocationClient = newLocation;
        OnRep_LocationClient();
    }
}

void AActorND::Server_SetRotation_Implementation( const FRotator& newRotation, bool updateClient )
{
    Rotation = newRotation;
    OnRep_Rotation();

    if (updateClient)
    {
        RotationClient = newRotation;
        OnRep_RotationClient();
    }
}

void AActorND::OnRep_Location()
{
    UpdateLocation(Location);
}

void AActorND::OnRep_LocationClient()
{
    UpdateLocation(LocationClient);
}

void AActorND::UpdateLocation( FLocation64 location )
{
    APlayerController* playerController = GetWorld()->GetFirstPlayerController();
    if (!playerController) return;
    APlayerControllerND* playerControllerND = Cast<APlayerControllerND>(playerController);
    FVector physicalLocation = VirtualLocationToPhysical( location, playerControllerND->GetOffset() );
    SetActorLocation(physicalLocation);
}

void AActorND::OnRep_Rotation()
{
    SetActorRotation(Rotation);
}

void AActorND::OnRep_RotationClient()
{
    SetActorRotation(RotationClient);
}