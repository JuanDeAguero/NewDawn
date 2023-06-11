// Copyright (c) Parallax 2022-2023

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "NDBlueprintLibrary.generated.h"

UCLASS()
class NEWDAWN_API UBlueprintLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:

    UFUNCTION(BlueprintCallable)
    static bool RandomBool( int64 x, int64 y, int64 z, int32 seed, float probability )
    {
        if ( probability == 0 ) return false;
        int64 h = seed + x * 374761393 + y * 668265263 + z * 555555653;
        h = ( h ^ ( h >> 13 )) * 1274126177;
        int64 r = h ^ ( h >> 16 );
        FRandomStream randomStream(r);
        int32 n = randomStream.RandRange( 1, 1 / probability );
        return n == 1;
    }

    UFUNCTION(BlueprintCallable)
    static float RandomFloat( float value, int64 seed, float min, float max )
    {
        int64 h = seed + value * 374761393;
        h = ( h ^ ( h >> 13 )) * 1274126177;
        int64 r = h ^ ( h >> 16 );
        FRandomStream randomStream(r);
        return randomStream.FRandRange( min, max );
    }

    UFUNCTION(BlueprintPure)
    static UPARAM(DisplayName="New Location") FVector RotateAroundPoint( const FVector& location, const FVector& point, float angle )
    {
        FVector axis = FVector( 0, 0, 1 );
        FVector translatedLocation = location - point;
        FVector a = FVector( translatedLocation.X, translatedLocation.Y, 0 );
        a.Normalize();
        FVector b = axis * FVector::DotProduct( axis, translatedLocation );
        float c = FMath::Sqrt( FMath::Pow( translatedLocation.Size(), 2 ) - FMath::Pow( b.Size(), 2 ));
        FVector d = a * c;
        FVector e = d.RotateAngleAxis( angle, axis );
        return e + b + point;
    }

    UFUNCTION(BlueprintPure)
    static UPARAM(DisplayName="New Rotation") FRotator RotateAngleAroundPoint( const FRotator& rotation, const FVector& point, float angle )
    {
        FVector v = rotation.Vector().RotateAngleAxis( angle, FVector( 0, 0, 1 ));
        return v.Rotation();
    }

    UFUNCTION(BlueprintPure)
    static UPARAM(DisplayName="Angle") float GetTimedAngle()
    {
        FTimespan now = FDateTime::Now().GetTimeOfDay();
        float milliseconds = now.GetTotalMilliseconds();
        float hours = now.GetTotalHours();
        float a = milliseconds - ( FMath::Floor(hours) * 3600000.0f );
        float b = 360.0f / 3600000.0f;
        return a * b;
    }

};