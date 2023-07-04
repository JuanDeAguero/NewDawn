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

protected:

    class USceneComponent* Root;

    UPROPERTY(EditDefaultsOnly)
    class UCapsuleComponent* Capsule;

    UPROPERTY(EditDefaultsOnly)
    class USkeletalMeshComponent* Body;

    UPROPERTY(EditDefaultsOnly)
    class USpringArmComponent* SpringArm;

    UPROPERTY(EditDefaultsOnly)
    class UCameraComponent* Camera;

    UPROPERTY(EditDefaultsOnly)
    class UInputAction* IA_Look;

    UPROPERTY(EditDefaultsOnly)
    class UInputAction* IA_Move;

    UPROPERTY(EditDefaultsOnly)
    class UInputAction* IA_Interact;

private:

    enum class EGravityProfile
    {
        Planet, Moon, Spaceship, SpaceStation, SpaceRing
    };
    
    EGravityProfile GravityProfile;

public:

    ACharacterND();

    virtual void SetupPlayerInputComponent( UInputComponent* inputComponent );

private:

    void Look( const FInputActionValue& actionValue );

    void Move( const FInputActionValue& actionValue );

    void Interact();

};