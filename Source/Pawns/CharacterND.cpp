// Copyright (c) Parallax 2022-2023

#include "CharacterND.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/SpringArmComponent.h"

ACharacterND::ACharacterND()
{
    Root      = CreateDefaultSubobject<USceneComponent>(       TEXT("Root"));
    Capsule   = CreateDefaultSubobject<UCapsuleComponent>(     TEXT("Capsule"));
    Body      = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Body"));
    SpringArm = CreateDefaultSubobject<USpringArmComponent>(   TEXT("SpringArm"));
    Camera    = CreateDefaultSubobject<UCameraComponent>(      TEXT("Camera"));

    RootComponent = Root;
    
    Capsule->  SetupAttachment(Root);
    Body->     SetupAttachment(Root);
    SpringArm->SetupAttachment(Root);
    Camera->   SetupAttachment(SpringArm);
}

void ACharacterND::SetupPlayerInputComponent( UInputComponent* inputComponent )
{
    Super::SetupPlayerInputComponent(inputComponent);

    UEnhancedInputComponent* enhancedInputComponent = Cast<UEnhancedInputComponent>(inputComponent);

    enhancedInputComponent->BindAction( IA_Look,     ETriggerEvent::Triggered, this, &ACharacterND::Look );
    enhancedInputComponent->BindAction( IA_Move,     ETriggerEvent::Triggered, this, &ACharacterND::Move );
    enhancedInputComponent->BindAction( IA_Interact, ETriggerEvent::Started,   this, &ACharacterND::Interact );
}

void ACharacterND::Look( const FInputActionValue& actionValue )
{
    FVector2D axisValue = actionValue.Get<FVector2D>();
    float x = axisValue.X;
    float y = axisValue.Y;

    FRotator newActorRotation = FRotator( 0.0f, x, 0.0f ) + GetActorRotation();
    SetActorRotation(newActorRotation);
    Server_SetRotation( newActorRotation, false );

    FRotator newSpringArmRotation = FRotator( y, 0.0f, 0.0f ) + SpringArm->GetRelativeRotation();
    SpringArm->SetRelativeRotation(newSpringArmRotation);

}

void ACharacterND::Move( const FInputActionValue& actionValue )
{

}

void ACharacterND::Interact()
{

}