// Copyright (c) Parallax 2022-2023

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "UtilsND.h"

#include "PawnND.generated.h"

UCLASS()
class NEWDAWN_API APawnND : public APawn
{
    GENERATED_BODY()

private:

    UPROPERTY(Replicated)
    int64 Id;

    UPROPERTY(ReplicatedUsing=OnRep_Location)
    FLocation64 Location;

    UPROPERTY(ReplicatedUsing=OnRep_LocationClient)
    FLocation64 LocationClient;

    UPROPERTY(ReplicatedUsing=OnRep_Rotation)
    FRotator Rotation;

    UPROPERTY(ReplicatedUsing=OnRep_RotationClient)
    FRotator RotationClient;

public:

    APawnND();

    virtual void SetupPlayerInputComponent( UInputComponent* inputComponent );

    int64 GetId();

    UFUNCTION( Server, Reliable )
    void Server_SetId( int64 newId );

    FLocation64 GetLocation();

    UFUNCTION( Server, Reliable )
    void Server_SetLocation( const FLocation64& newLocation, bool updateClient );

    FRotator GetRotation();

    UFUNCTION( Server, Reliable )
    void Server_SetRotation( const FRotator& newRotation, bool updateClient );

protected:
    
    virtual void BeginPlay() override;

private:

    void Server_SetId_Implementation( int64 newId );

    void Server_SetLocation_Implementation( const FLocation64& newLocation, bool updateClient );

    void Server_SetRotation_Implementation( const FRotator& newRotation, bool updateClient );

    UFUNCTION()
    void OnRep_Location();

    UFUNCTION()
    void OnRep_LocationClient();

    void UpdateLocation( FLocation64 location );

    UFUNCTION()
    void OnRep_Rotation();

    UFUNCTION()
    void OnRep_RotationClient();

};