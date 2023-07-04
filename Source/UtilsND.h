// Copyright (c) Parallax 2022-2023

#pragma once

#include "CoreMinimal.h"

#include "UtilsND.generated.h"

USTRUCT()
struct FVector64
{
    GENERATED_BODY()

    UPROPERTY()
    int64 X;

    UPROPERTY()
    int64 Y;

    UPROPERTY()
    int64 Z;
};

USTRUCT()
struct FLocation64
{
    GENERATED_BODY()

    UPROPERTY()
    FVector Location;

    UPROPERTY()
    FVector64 Offset;
};

static FVector VirtualLocationToPhysical( const FLocation64& location, const FVector64& offset )
{
    return location.Location + FVector(
        location.Offset.X - offset.X,
        location.Offset.Y - offset.Y,
        location.Offset.Z - offset.Z
    );
}

UCLASS()
class NEWDAWN_API UDoOnce : public UObject
{
    GENERATED_BODY()

private:

    bool HasExecuted;

public:

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