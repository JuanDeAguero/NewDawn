// Copyright (c) Parallax 2022-2023

#pragma once

#include "CoreMinimal.h"
#include "Utility.generated.h"

USTRUCT(BlueprintType)
struct FVector64
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite)
    int64 X;

    UPROPERTY(BlueprintReadWrite)
    int64 Y;

    UPROPERTY(BlueprintReadWrite)
    int64 Z;
};

USTRUCT(BlueprintType)
struct FLocation64
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite)
    FVector Location;

    UPROPERTY(BlueprintReadWrite)
    FVector64 Offset;
};

UENUM(BlueprintType)
enum class EDestinationType : uint8
{
    Star         UMETA(DisplayName="Star"),
    Planet       UMETA(DisplayName="Planet"),
    Moon         UMETA(DisplayName="Moon"),
    SpaceStation UMETA(DisplayName="SpaceStation")
};

UENUM(BlueprintType)
enum class EGravityProfile : uint8
{
    Planet       UMETA(DisplayName="Planet"),
    Moon         UMETA(DisplayName="Moon"),
    Spaceship    UMETA(DisplayName="Spaceship"),
    SpaceStation UMETA(DisplayName="SpaceStation"),
    SpaceRing    UMETA(DisplayName="SpaceRing")
};

UCLASS()
class NEWDAWN_API UDoOnce : public UObject
{
    GENERATED_BODY()

public:

    bool HasExecuted;

    UDoOnce()
        : HasExecuted(false) {}

    bool Execute( TFunction<void()> func )
    {
        if (!HasExecuted)
        {
            if (func) func();
            HasExecuted = true;
            return true;
        }
        return false;
    }

    void Reset()
    {
        HasExecuted = false;
    }

};

UCLASS()
class NEWDAWN_API UDelayAction : public UObject
{
    GENERATED_BODY()

public:

    FTimerHandle TimerHandle;
    TFunction<void()> FunctionToExecute;

    UDelayAction() {}

    void Delay( UWorld* world, float delayTime, TFunction<void()> func )
    {
        if (world)
        {
            FunctionToExecute = func;
            world->GetTimerManager().SetTimer( TimerHandle, this, &UDelayAction::ExecuteFunction, delayTime, false );
        }
    }

    void ExecuteFunction()
    {
        if (FunctionToExecute)
            FunctionToExecute();
    }

};

UCLASS()
class NEWDAWN_API UActorValidityCheck : public UObject
{
    GENERATED_BODY()

public:

    UDelayAction* DelayInstance;

    UActorValidityCheck()
    {
        DelayInstance = NewObject<UDelayAction>();
    }

    void CheckActorValidity( AActor* actorToCheck, TFunction<void()> onActorValid )
    {
        ActorCheckRoutine( actorToCheck, onActorValid );
    }

    void ActorCheckRoutine( AActor* actorToCheck, TFunction<void()> onActorValid )
    {
        if ( actorToCheck && IsValid(actorToCheck) )
            onActorValid();
        else
        {
            DelayInstance->Delay( actorToCheck->GetWorld(), 0.1f, [ this, actorToCheck, onActorValid ]()
            {
                this->ActorCheckRoutine( actorToCheck, onActorValid );
            });
        }
    }

};

UCLASS()
class NEWDAWN_API UBoolCheck : public UObject
{
    GENERATED_BODY()

public:

    UDelayAction* DelayInstance;

    UBoolCheck()
    {
        DelayInstance = NewObject<UDelayAction>();
    }

    void CheckBool( TFunction<bool()> boolProvider, TFunction<void()> onBoolTrue )
    {
        BoolCheckRoutine( boolProvider, onBoolTrue );
    }

    void BoolCheckRoutine( TFunction<bool()> boolProvider, TFunction<void()> onBoolTrue )
    {
        if (boolProvider())
            onBoolTrue();
        else
        {
            DelayInstance->Delay( GetWorld(), 0.1f, [ this, boolProvider, onBoolTrue ]()
            {
                this->BoolCheckRoutine( boolProvider, onBoolTrue );
            });
        }
    }

};