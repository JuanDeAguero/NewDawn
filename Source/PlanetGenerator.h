// Copyright (c) Parallax 2022

#pragma once

#include "CoreMinimal.h"
#include "FastNoise/VoxelFastNoise.h"
#include "VoxelGenerators/VoxelGeneratorHelpers.h"
#include "PlanetGenerator.generated.h"

UCLASS(Blueprintable)
class UPlanetGenerator : public UVoxelGenerator
{
	GENERATED_BODY()

public:

    UPROPERTY(EditDefaultsOnly, Category = Curve)
    UCurveFloat* Curve;

	UPROPERTY( EditAnywhere, BlueprintReadWrite ) float NoiseHeight = 10.0f;
	UPROPERTY( EditAnywhere, BlueprintReadWrite ) int32 Seed = 1337;
	virtual TVoxelSharedRef<FVoxelGeneratorInstance> GetInstance() override;

};

class FPlanetGeneratorInstance : public TVoxelGeneratorInstanceHelper< FPlanetGeneratorInstance, UPlanetGenerator >
{

public:

	using Super = TVoxelGeneratorInstanceHelper< FPlanetGeneratorInstance, UPlanetGenerator >;
	explicit FPlanetGeneratorInstance( const UPlanetGenerator& generator );
	virtual void Init( const FVoxelGeneratorInit& initStruct ) override;
	v_flt GetValueImpl( v_flt x, v_flt y, v_flt z, int32 LOD, const FVoxelItemStack& items ) const;
	FVoxelMaterial GetMaterialImpl( v_flt x, v_flt y, v_flt z, int32 LOD, const FVoxelItemStack& items ) const;
	TVoxelRange<v_flt> GetValueRangeImpl( const FVoxelIntBox& bounds, int32 LOD, const FVoxelItemStack& items ) const;
	virtual FVector GetUpVector( v_flt x, v_flt y, v_flt z ) const override final;

    v_flt GetHeight( v_flt x, v_flt y, v_flt z, int32 LOD, const FVoxelItemStack& items ) const;
    TVoxelRange<v_flt> GetHeightRange( const FVoxelIntBox& bounds, int32 LOD, const FVoxelItemStack& items ) const;

    v_flt GetTreeDensity( v_flt x, v_flt y, v_flt z, int32 LOD, const FVoxelItemStack& items ) const;

private:

	const float NoiseHeight;
	const int32 Seed;
	FVoxelFastNoise Noise;

};