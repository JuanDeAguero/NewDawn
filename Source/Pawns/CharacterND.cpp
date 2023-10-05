// Copyright (c) Parallax 2022-2023

#include "CharacterND.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Net/UnrealNetwork.h"

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

    GravityProfile = EGravityProfile::SpaceStation;
}

void ACharacterND::GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& props ) const
{
    Super::GetLifetimeReplicatedProps(props);
    TArray<FLifetimeProperty>& OutLifetimeProps = props;
    DOREPLIFETIME( ACharacterND, Walking );
    DOREPLIFETIME( ACharacterND, Sitting );
    DOREPLIFETIME_CONDITION( ACharacterND, BodyRotation,            COND_SkipOwner );
    DOREPLIFETIME_CONDITION( ACharacterND, BodyRotationClient,      COND_OwnerOnly );
    DOREPLIFETIME_CONDITION( ACharacterND, SpringArmRotation,       COND_SkipOwner );
    DOREPLIFETIME_CONDITION( ACharacterND, SpringArmRotationClient, COND_OwnerOnly );
}

void ACharacterND::SetupPlayerInputComponent( UInputComponent* inputComponent )
{
    Super::SetupPlayerInputComponent(inputComponent);
    UEnhancedInputComponent* enhancedInputComponent = Cast<UEnhancedInputComponent>(inputComponent);
    enhancedInputComponent->BindAction( IA_Look,     ETriggerEvent::Triggered, this, &ACharacterND::Look );
    enhancedInputComponent->BindAction( IA_Move,     ETriggerEvent::Triggered, this, &ACharacterND::Move );
    enhancedInputComponent->BindAction( IA_Move,     ETriggerEvent::Completed, this, &ACharacterND::MoveCompleted );
    enhancedInputComponent->BindAction( IA_Interact, ETriggerEvent::Started,   this, &ACharacterND::Interact );
}

bool ACharacterND::GetWalking()
{
    return Walking;
}

bool ACharacterND::GetSitting()
{
    return Sitting;
}

void ACharacterND::Look( const FInputActionValue& actionValue )
{
    FVector2D axisValue = actionValue.Get<FVector2D>();
    float x = axisValue.X;
    float y = axisValue.Y;
    if ( FMath::Abs(x) < 0.1f ) x = 0.0f;
    if ( FMath::Abs(y) < 0.1f ) y = 0.0f;
    if ( x == 0.0f && y == 0.0f ) return;
    
    FRotator newActorRotation = FRotator( 0.0f, x, 0.0f ) + GetActorRotation();
    SetActorRotation(newActorRotation);
    Server_SetRotation( newActorRotation, false );
    FRotator newSpringArmRotation = FRotator( y, 0.0f, 0.0f ) + SpringArm->GetRelativeRotation();
    SpringArm->SetRelativeRotation(newSpringArmRotation);
    Server_SetSpringArmRotation( newSpringArmRotation, false );
    if (!Walking)
    {
        FRotator newBodyRotation = FRotator( 0.0f, -1.0f * x, 0.0f ) + Body->GetRelativeRotation();
        Body->SetRelativeRotation(newBodyRotation);
        Server_SetBodyRotation( newBodyRotation, false );
    }
}

void ACharacterND::Move( const FInputActionValue& actionValue )
{
    FVector2D axisValue = actionValue.Get<FVector2D>();
    float x = axisValue.X;
    float y = axisValue.Y;
    if ( FMath::Abs(x) < 0.1f ) x = 0.0f;
    if ( FMath::Abs(y) < 0.1f ) y = 0.0f;
    if ( x == 0.0f && y == 0.0f )
    {
        Server_SetWalking(false);
        return;
    }
    Server_SetWalking(true);

    FVector start;
    FVector end;
    GetMoveLineTraceParams( x, y, 20.0f, 500.0f, 10.0f, 10.0f, start, end );
    FCollisionQueryParams traceParams;
    FHitResult hitResult;
    GetWorld()->LineTraceSingleByChannel( hitResult, start, end, ECC_Visibility, traceParams );
    FVector hitLocation = hitResult.Location;
    FRotator rotation;
    switch (GravityProfile)
    {
        case EGravityProfile::Planet:
        {

        }
        case EGravityProfile::SpaceStation:
        {

        }
    }
    SetActorLocation(hitLocation);
    //SetActorRotation(rotation);
    Body->SetRelativeRotation(FRotator( 0.0f, -90.0f, 0.0f ));
}

void ACharacterND::GetMoveLineTraceParams( float x, float y, float up, float down, float forward, float right, FVector& start, FVector& end )
{
    FVector a = GetActorForwardVector() * y * forward;
    FVector b = GetActorUpVector() * up;
    FVector c = GetActorRightVector() * x * right;
    FVector d = GetActorUpVector() * down;
    start = GetActorLocation() + a + b + c;
    end = start - d;
}

FRotator ACharacterND::GetRotationInPlanet( FVector location )
{
    return FRotator();
}

FRotator ACharacterND::GetRotationInSpaceStation( FVector gravityDirection )
{
    return FRotator();
}

void ACharacterND::MoveCompleted( const FInputActionValue& actionValue )
{
    Server_SetWalking(false);
}

void ACharacterND::Interact()
{

}

void ACharacterND::Server_SetWalking_Implementation( bool newValue )
{
    Walking = newValue;
}

void ACharacterND::Server_SetSitting_Implementation( bool newValue )
{
    Sitting = newValue;
}

void ACharacterND::Server_SetBodyRotation_Implementation( const FRotator& newRotation, bool updateClient )
{
    BodyRotation = newRotation;
    OnRep_BodyRotation();
    if (updateClient)
    {
        BodyRotationClient = newRotation;
        OnRep_BodyRotationClient();
    }
}

void ACharacterND::Server_SetSpringArmRotation_Implementation( const FRotator& newRotation, bool updateClient )
{
    SpringArmRotation = newRotation;
    OnRep_SpringArmRotation();
    if (updateClient)
    {
        SpringArmRotationClient = newRotation;
        OnRep_SpringArmRotationClient();
    }
}

void ACharacterND::OnRep_BodyRotation()
{
    Body->SetRelativeRotation(BodyRotation);
}

void ACharacterND::OnRep_BodyRotationClient()
{
    Body->SetRelativeRotation(BodyRotationClient);
}

void ACharacterND::OnRep_SpringArmRotation()
{
    SpringArm->SetRelativeRotation(SpringArmRotation);
}

void ACharacterND::OnRep_SpringArmRotationClient()
{
    SpringArm->SetRelativeRotation(SpringArmRotationClient);
}