// Copyright (c) Parallax 2022

#include "PlanetGenerator.h"
#include "FastNoise/VoxelFastNoise.inl"
#include "VoxelMaterialBuilder.h"

TVoxelSharedRef<FVoxelGeneratorInstance> UPlanetGenerator::GetInstance()
{
	return MakeVoxelShared<FPlanetGeneratorInstance>(*this);
}

FPlanetGeneratorInstance::FPlanetGeneratorInstance( const UPlanetGenerator& generator )
	: Super( &generator, FCustomFunctionPtrs{
        TMap<FName, TOutputFunctionPtr<v_flt>> {
            { FName(TEXT("Height")), static_cast<TOutputFunctionPtr<v_flt>>(&FPlanetGeneratorInstance::GetHeight) }
        }, 
        TMap<FName, TOutputFunctionPtr<int32>> {},
        TMap<FName, TOutputFunctionPtr<FColor>> {},
        TMap<FName, TRangeOutputFunctionPtr<v_flt>> {
            { FName(TEXT("Height")), static_cast<TRangeOutputFunctionPtr<v_flt>>(&FPlanetGeneratorInstance::GetHeightRange) }
        }
    }),
    NoiseHeight(generator.NoiseHeight), Seed(generator.Seed) {}

void FPlanetGeneratorInstance::Init( const FVoxelGeneratorInit& initStruct )
{
	Noise.SetSeed(Seed);
}

v_flt FPlanetGeneratorInstance::GetHeight( v_flt x, v_flt y, v_flt z, int32 LOD, const FVoxelItemStack& items ) const
{
	const float radius = 6360000.0f;
	const FVector position = FVector( x, y, z ).GetSafeNormal();
	float height = radius + Noise.GetPerlin_3D( position.X, position.Y, position.Z, 0.01 * radius ) * NoiseHeight;
	//height /= 5;
    return height;
}

TVoxelRange<v_flt> FPlanetGeneratorInstance::GetHeightRange( const FVoxelIntBox &bounds, int32 LOD, const FVoxelItemStack &items ) const
{
    //const TVoxelRange<v_flt> height = TVoxelRange<v_flt>(-1, 1) * NoiseHeight;
    //return height;
    return TVoxelRange<v_flt>::Infinite();
}

v_flt FPlanetGeneratorInstance::GetTreeDensity( v_flt x, v_flt y, v_flt z, int32 LOD, const FVoxelItemStack& items ) const
{
    return 0.5f;
}

v_flt FPlanetGeneratorInstance::GetValueImpl( v_flt x, v_flt y, v_flt z, int32 LOD, const FVoxelItemStack& items ) const
{
    const v_flt radius = 6360000.0f;
	const FVector position = FVector( x, y, z ).GetSafeNormal();
	const v_flt height = radius + Noise.GetPerlin_3D( position.X, position.Y, position.Z, 0.001f * radius ) * 100.0f;
	v_flt value = FVector( x, y, z ).Size() - height;
	return value;
}

FVoxelMaterial FPlanetGeneratorInstance::GetMaterialImpl( v_flt x, v_flt y, v_flt z, int32 LOD, const FVoxelItemStack& items ) const
{
	FVoxelMaterialBuilder builder;
	builder.SetMaterialConfig(EVoxelMaterialConfig::RGB);
	builder.SetColor(FColor::Red);
	return builder.Build();
}

TVoxelRange<v_flt> FPlanetGeneratorInstance::GetValueRangeImpl( const FVoxelIntBox& bounds, int32 LOD, const FVoxelItemStack& items ) const
{
	return TVoxelRange<v_flt>::Infinite();
}

FVector FPlanetGeneratorInstance::GetUpVector( v_flt X, v_flt Y, v_flt Z ) const
{
	return FVector::UpVector;
}