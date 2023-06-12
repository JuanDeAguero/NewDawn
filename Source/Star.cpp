// Copyright (c) Parallax 2022

#include "Star.h"
#include "Kismet/GameplayStatics.h"
#include "Universe.h"
#include "PlayerStateND.h"
#include "Spaceship.h"
#include "PlayerControllerND.h"
#include "Engine/DirectionalLight.h"
#include "Kismet/KismetMathLibrary.h"

AStar::AStar()
	: Angle(0),
      Gate1a(true), Gate1b(true), Gate2a(true), Gate2b(true)
{
	PrimaryActorTick.bCanEverTick = false;

	Root         = CreateDefaultSubobject<USceneComponent>(     TEXT("Root"));
	Sphere       = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sphere"));
	PixelSphere  = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PixelSphere"));
	CursorSphere = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CursorSphere"));

	RootComponent = Root;

	Sphere->      SetupAttachment(Root);
	PixelSphere-> SetupAttachment(Root);
	CursorSphere->SetupAttachment(Root);
	
	Sphere->SetWorldScale3D(FVector(200000000.0f));

    Sphere->      CastShadow = false;
    PixelSphere-> CastShadow = false;
    CursorSphere->CastShadow = false;
}

void AStar::BeginPlay()
{
	Super::BeginPlay();

	PS = Cast<APlayerStateND>( GetWorld()->GetFirstPlayerController()->PlayerState );
	PC = Cast<APlayerControllerND>( GetWorld()->GetFirstPlayerController() );
	
	TArray<AActor*> universe, directionalLight;
    UGameplayStatics::GetAllActorsOfClass( GetWorld(), ADirectionalLight::StaticClass(), directionalLight );
	UGameplayStatics::GetAllActorsOfClass( GetWorld(), AUniverse::StaticClass(), universe );

	Universe = Cast<AUniverse>(universe[0]);
    DirectionalLight = Cast<ADirectionalLight>(directionalLight[0]);

	GetWorld()->GetTimerManager().SetTimer( DistanceTimer, this, &AStar::CheckDistance, 0.1f, true );
	GetWorld()->GetTimerManager().SetTimer( RotationTimer, this, &AStar::RotateAroundPlanet, 0.1f, true );
}

void AStar::ScalePixelSphere()
{
	// get viewport size
	int32 viewportX, viewportY;
	float viewportSize = 0;
	GetWorld()->GetFirstPlayerController()->GetViewportSize( viewportX, viewportY );
	viewportSize = viewportX * viewportY;
	if ( viewportSize == 0 ) return;
	
	// find scale based on viewport size and star distance to pawn
	APawn* pawn = GetWorld()->GetFirstPlayerController()->GetPawn();
	float distance = FVector::Dist( pawn->GetActorLocation(), GetActorLocation() );
	float scale = ( distance * 40 ) / viewportSize;
	scale *= 0.5f;

	// scale the spheres
	PixelSphere-> SetWorldScale3D(FVector(scale));
	CursorSphere->SetWorldScale3D(FVector( scale * 20 ));
}

void AStar::CheckDistance()
{
	if (!PS) return;

	APawn* pawn = UGameplayStatics::GetPlayerPawn( GetWorld(), 0 );
	float distance = FVector::Distance( GetActorLocation(), pawn->GetActorLocation() );
	
	bool condition1 = distance < 5 * FMath::Pow( 10, 12 );
	if ( condition1 && Gate1a )
	{
        PS->ClosestStar = this;
        PS->InStarSystem = true;
        Universe->SpawnPlanets(GetActorLocation());
        CursorSphere->SetHiddenInGame(true);

		Gate1a = false;
		Gate1b = true;
	}
	if ( !condition1 && Gate1b && PS->ClosestStar == this )
	{
		CursorSphere->SetHiddenInGame(false);
		PS->InStarSystem = false;

		Gate1b = false;
		Gate1a = true;
	}

	bool condition2 = distance < FMath::Pow( 10, 12 );
	if ( condition2 && Gate2a )
	{
		Cast<ASpaceship>(pawn)->Speed = 100;
		PS->SpaceTraveling = false;

		Gate2a = false;
		Gate2b = true;
	}
	if ( !condition2 && Gate2b )
	{
		Gate2b = false;
		Gate2a = true;
	}
}

void AStar::RotateAroundPlanet()
{
	if ( !PS || PS->PlanetSpinning || PS->ClosestStar != this || !PS->ClosestPlanet ) return;

    float timedAngle = PC->GetTimedAngle();
    float deltaAngle = timedAngle - Angle;
    Angle += deltaAngle;

    FVector location = PC->RotateAroundOrigin( GetActorLocation(), deltaAngle );
    SetActorLocation(location);

    FRotator rotation = UKismetMathLibrary::FindLookAtRotation( GetActorLocation(), PS->ClosestPlanet->GetActorLocation() );
    DirectionalLight->SetActorRotation(rotation);
}