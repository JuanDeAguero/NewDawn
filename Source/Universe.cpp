// Copyright (c) Parallax 2022

#include "Universe.h"
#include "VoxelWorld.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Star.h"
#include "Planet.h"

AUniverse::AUniverse()
	: OffsetX(0), OffsetY(0), OffsetZ(0),
	  Seed(5555),
	  MinStarDistance(10000000000000), MinPlanetDistance(100000000000),
	  StarProbability(0.2f), PlanetProbability(0.1f)
{
	PrimaryActorTick.bCanEverTick = false;
}

void AUniverse::BeginPlay()
{
	Super::BeginPlay();

}

void AUniverse::UpdateStars()
{
	if ( !StarBP ) return;

	// rebase origin to pawn location
	APawn* pawn = UGameplayStatics::GetPlayerPawn( GetWorld(), 0 );
	if (!pawn) return;
	RebaseOrigin(pawn->GetActorLocation());

	// set spawn offset based on the rebase offset,
	// this will spawn stars around the player
	int64 spawnOffsetX = 0, spawnOffsetY = 0, spawnOffsetZ = 0;
	if ( FMath::Abs(OffsetX) > MinStarDistance ) spawnOffsetX = floor( OffsetX / MinStarDistance );
	if ( FMath::Abs(OffsetY) > MinStarDistance ) spawnOffsetY = floor( OffsetY / MinStarDistance );
	if ( FMath::Abs(OffsetZ) > MinStarDistance ) spawnOffsetZ = floor( OffsetZ / MinStarDistance );

	int32 n = 6;
	for ( int32 starZ = -n; starZ <= n; starZ++ )
	for ( int32 starY = -n; starY <= n; starY++ )
	for ( int32 starX = -n; starX <= n; starX++ )
	{
		// spawn star if RandomBool true using 'Seed'
		if (RandomBool( starX + spawnOffsetX, starY + spawnOffsetY, starZ + spawnOffsetZ, Seed, StarProbability ))
		{
			// compute spawn location
			int64 spawnStarX = (( starX + spawnOffsetX ) * MinStarDistance ) - OffsetX;
			int64 spawnStarY = (( starY + spawnOffsetY ) * MinStarDistance ) - OffsetY;
			int64 spawnStarZ = (( starZ + spawnOffsetZ ) * MinStarDistance ) - OffsetZ;
			FVector starLocation = FVector( spawnStarX, spawnStarY, spawnStarZ );

			// check if star already exists
			bool shouldSpawn = true;
			for ( FVector existingStarLocation : StarLocations )
			{
				if ( starLocation == existingStarLocation )
				{
					shouldSpawn = false;
					break;
				}
			}
			
			// spawn star if it doesn't exist
			if (shouldSpawn)
			{
				StarLocations.Add(starLocation);
				GetWorld()->SpawnActor<AActor>( StarBP, starLocation, FRotator() );
			}
		}
	}

	// TODO: destroy stars that are too far

	Scale();
}

void AUniverse::SpawnPlanets( FVector starLocation )
{
	TArray<AActor*> planets;
	UGameplayStatics::GetAllActorsOfClass( GetWorld(), PlanetBP, planets );
	for ( AActor* planet : planets ) planet->Destroy();

	int64 x = 0, y = 0, z = 0;
	x = ( starLocation.X + OffsetX ) / MinStarDistance;
	y = ( starLocation.Y + OffsetY ) / MinStarDistance;
	z = ( starLocation.Z + OffsetZ ) / MinStarDistance;

	for ( int32 i = 0; i < 8; i++ )
	{
		float vx = RandomFloat( x + i, Seed, -1.0f, 1.0f );
		float vy = RandomFloat( y + i, Seed, -1.0f, 1.0f );
		float vz = RandomFloat( z + i, Seed, -0.1f, 0.1f );

		FVector v( vx, vy, 0 );
		v.Normalize();

		bool b = RandomBool( x + i, y + i, z + i, Seed, 0.5f );
		if (b) v *= 600000000000;
		else v *= 600000000000;

		int64 spawnPlanetX = starLocation.X + v.X;
		int64 spawnPlanetY = starLocation.Y + v.Y;
		int64 spawnPlanetZ = starLocation.Z + v.Z;

		spawnPlanetX = ((int64)( spawnPlanetX / 1000 )) * 1000;
		spawnPlanetY = ((int64)( spawnPlanetY / 1000 )) * 1000;
		spawnPlanetZ = ((int64)( spawnPlanetZ / 1000 )) * 1000;

		FVector spawnLocation = FVector( spawnPlanetX, spawnPlanetY, spawnPlanetZ );

		bool shouldSpawn = true;
		TArray<AActor*> newPlanets;
		UGameplayStatics::GetAllActorsOfClass( GetWorld(), PlanetBP, newPlanets );
		for ( AActor* planet : newPlanets )
		{
			float distance = FVector::Dist( spawnLocation, planet->GetActorLocation() );
			if ( distance < MinPlanetDistance )
			{
				shouldSpawn = false;
				break;
			}
		}

		if (shouldSpawn)
			GetWorld()->SpawnActor<AActor>( PlanetBP, spawnLocation, FRotator() );
	}
}

void AUniverse::RebaseOrigin( FVector location )
{
	APawn* pawn = UGameplayStatics::GetPlayerPawn( GetWorld(), 0 );
	if (!pawn) return;

	// increment offset
	OffsetX += location.X;
	OffsetY += location.Y;
	OffsetZ += location.Z;

	// get all the actors to rebase
	TArray<AActor*> actorsToRebase;
	TArray<AActor*> stars, planets, taggedActors;
	UGameplayStatics::GetAllActorsOfClass( GetWorld(), StarBP, stars );
	UGameplayStatics::GetAllActorsOfClass( GetWorld(), PlanetBP, planets );
	UGameplayStatics::GetAllActorsWithTag( GetWorld(), "rebase", taggedActors );
	actorsToRebase.Add(pawn);
	actorsToRebase.Append(stars);
	actorsToRebase.Append(planets);
	actorsToRebase.Append(taggedActors);

	// rebase actors
	for ( AActor* actor : actorsToRebase )
		actor->AddActorWorldOffset( -1.0f * FVector( location.X, location.Y, location.Z ));
}


void AUniverse::Scale()
{
	// get viewport size
	int32 viewportX, viewportY;
	float viewportSize = 0;
	GetWorld()->GetFirstPlayerController()->GetViewportSize( viewportX, viewportY );
	viewportSize = viewportX * viewportY;

	// if viewport has size 0, wait 0.1s and recurse
	if ( viewportSize == 0 )
	{
		FTimerHandle handle;
		GetWorld()->GetTimerManager().SetTimer( handle, this, &AUniverse::Scale, 0.1f, false );
	}
	else Scale_BC(); // scale when viewport size is set
}

void AUniverse::Scale_BC()
{
	TArray<AActor*> stars, planets;
	UGameplayStatics::GetAllActorsOfClass( GetWorld(), StarBP, stars );
	UGameplayStatics::GetAllActorsOfClass( GetWorld(), PlanetBP, planets );

	// scale stars
	for ( AActor* star : stars )
		Cast<AStar>(star)->ScalePixelSphere();

	// scale planets
	for ( AActor* planet : planets )
		Cast<APlanet>(planet)->ScalePixelSphere();
}

bool AUniverse::RandomBool( float x, float y, float z, int64 seed, float probability )
{
	if ( probability == 0 ) return false;
	int64 h = seed + x * 374761393 + y * 668265263 + z * 555555653;
	h = ( h ^ ( h >> 13 )) * 1274126177;
	int64 r = h ^ ( h >> 16 );
	FRandomStream randomStream(r);
	int32 n = randomStream.RandRange( 1, 1 / probability );
	return n == 1;
}

float AUniverse::RandomFloat( float value, int64 seed, float min, float max )
{
	int64 h = seed + value * 374761393;
	h = ( h ^ ( h >> 13 )) * 1274126177;
	int64 r = h ^ ( h >> 16 );
	FRandomStream randomStream(r);
	return randomStream.FRandRange( min, max );
}

int64 AUniverse::GetOffsetX()
{
	return OffsetX;
}

int64 AUniverse::GetOffsetY()
{
	return OffsetY;
}

int64 AUniverse::GetOffsetZ()
{
	return OffsetZ;
}