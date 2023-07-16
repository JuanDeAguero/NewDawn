// Copyright (c) Parallax 2022-2023

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "PawnND.h"

#include "CharacterND.generated.h"

UCLASS()
class NEWDAWN_API ACharacterND : public APawnND
{
    GENERATED_BODY()
    
public:

    ACharacterND();

    virtual void SetupPlayerInputComponent( UInputComponent* inputComponent );

    UFUNCTION(BlueprintPure)
    UPARAM(DisplayName="Walking") bool GetWalking();

    UFUNCTION( Server, Reliable )
    void Server_SetWalking( bool newValue );

    UFUNCTION(BlueprintPure)
    UPARAM(DisplayName="Sitting") bool GetSitting();

    UFUNCTION( Server, Reliable )
    void Server_SetSitting( bool newValue );

    UFUNCTION( Server, Reliable )
    void Server_SetBodyRotation( const FRotator& newRotation, bool updateClient );

    UFUNCTION( Server, Reliable )
    void Server_SetSpringArmRotation( const FRotator& newRotation, bool updateClient );
    
protected:

    // -- Scene Components --

    class USceneComponent* Root;

    UPROPERTY(EditDefaultsOnly)
    class UCapsuleComponent* Capsule;

    UPROPERTY(EditDefaultsOnly)
    class USkeletalMeshComponent* Body;

    UPROPERTY(EditDefaultsOnly)
    class USpringArmComponent* SpringArm;

    UPROPERTY(EditDefaultsOnly)
    class UCameraComponent* Camera;
    
    // -- Input --

    UPROPERTY(EditDefaultsOnly)
    class UInputAction* IA_Look;

    UPROPERTY(EditDefaultsOnly)
    class UInputAction* IA_Move;

    UPROPERTY(EditDefaultsOnly)
    class UInputAction* IA_Interact;
    
private:

    UPROPERTY(Replicated)
    bool Walking;

    UPROPERTY(Replicated)
    bool Sitting;

    UPROPERTY(ReplicatedUsing=OnRep_BodyRotation)
    FRotator BodyRotation;

    UPROPERTY(ReplicatedUsing=OnRep_BodyRotationClient)
    FRotator BodyRotationClient;

    UPROPERTY(ReplicatedUsing=OnRep_SpringArmRotation)
    FRotator SpringArmRotation;

    UPROPERTY(ReplicatedUsing=OnRep_SpringArmRotationClient)
    FRotator SpringArmRotationClient;

    enum class EGravityProfile
    {
        Planet, Moon, Spaceship, SpaceStation, SpaceRing
    };
    
    EGravityProfile GravityProfile;

    void Look( const FInputActionValue& actionValue );

    void Move( const FInputActionValue& actionValue );

    void GetMoveLineTraceParams( float x, float y, float up, float down, float forward, float right, FVector& start, FVector& end );

    FRotator GetRotationInPlanet( FVector location );

    FRotator GetRotationInSpaceStation( FVector gravityDirection );

    void MoveCompleted( const FInputActionValue& actionValue );

    void Interact();

    void Server_SetWalking_Implementation( bool newValue );

    void Server_SetSitting_Implementation( bool newValue );

    // -- Rep Implementations --

    void Server_SetBodyRotation_Implementation( const FRotator& newRotation, bool updateClient );

    void Server_SetSpringArmRotation_Implementation( const FRotator& newRotation, bool updateClient );
                                                     
    // -- On Rep --

    UFUNCTION()
    void OnRep_BodyRotation();

    UFUNCTION()
    void OnRep_BodyRotationClient();

    UFUNCTION()
    void OnRep_SpringArmRotation();

    UFUNCTION()
    void OnRep_SpringArmRotationClient();
    
};