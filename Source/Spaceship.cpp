// Copyright (c) Parallax 2022

#include "Spaceship.h"
#include "Universe.h"
#include "Planet.h"
#include "PlayerStateND.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Universe.h"
#include "GameInstanceND.h"

ASpaceship::ASpaceship()
	: Speed(10), MaxSpeed(1000000)
{
	PrimaryActorTick.bCanEverTick = false;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SpringArmCameraBack = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmCameraBack"));

	RootComponent = Root;
	SpringArmCameraBack->SetupAttachment(Root);
}

void ASpaceship::BeginPlay()
{
	Super::BeginPlay();
	
	PS = Cast<APlayerStateND>(GetWorld()->GetFirstPlayerController()->PlayerState);

	GetWorld()->GetTimerManager().SetTimer( MovementTimer, this, &ASpaceship::MoveForward, 0.001f, true );
	GetWorld()->GetTimerManager().SetTimer( ScaleTimer,    this, &ASpaceship::ScaleUniverse, 1.0f, true );
	GetWorld()->GetTimerManager().SetTimer( SaveTimer,     this, &ASpaceship::Save, 1.0f, true );
}

void ASpaceship::SetupPlayerInputComponent( UInputComponent* input )
{
	Super::SetupPlayerInputComponent(input);

	input->BindAxis( "Roll",  this, &ASpaceship::Roll  );
	input->BindAxis( "Pitch", this, &ASpaceship::Pitch );
	input->BindAxis( "Yaw",   this, &ASpaceship::Yaw   );

	input->BindAction( "SpeedUp",     IE_Pressed, this, &ASpaceship::SpeedUp     );
	input->BindAction( "SpeedDown",   IE_Pressed, this, &ASpaceship::SpeedDown   );
	input->BindAction( "ToggleScan",  IE_Pressed, this, &ASpaceship::ToggleScan  );
	input->BindAction( "SpaceTravel", IE_Pressed, this, &ASpaceship::SpaceTravel );
}

void ASpaceship::MoveForward()
{
	FVector offset = GetActorForwardVector() * Speed;
	AddActorWorldOffset(offset);
}

void ASpaceship::Pitch( float axisValue )
{
	if ( !PS->Scanning && !PS->SpaceTraveling )
		AddActorLocalRotation( FRotator( axisValue * 0.5f, 0, 0 ));
}

void ASpaceship::Roll( float axisValue )
{
	if ( !PS->Scanning && !PS->SpaceTraveling )
		AddActorLocalRotation( FRotator( 0, 0, axisValue * 0.5f ));
}

void ASpaceship::Yaw( float axisValue )
{
	if ( !PS->Scanning && !PS->SpaceTraveling )
		AddActorLocalRotation( FRotator( 0, axisValue * 0.5f, 0 ));
}

void ASpaceship::ScaleUniverse()
{
	if (!Universe)
	{
		TArray<AActor*> universe;
		UGameplayStatics::GetAllActorsOfClass( GetWorld(), AUniverse::StaticClass(), universe );
		if ( universe.Num() > 0 )
			Universe = Cast<AUniverse>(universe[0]);
	}
	else Universe->Scale();
}

void ASpaceship::SpeedUp()
{
	if ( !PS->SpaceTraveling && Speed < MaxSpeed )
		Speed *= 10;
}

void ASpaceship::SpeedDown()
{
	if ( !PS->SpaceTraveling && Speed > 1 )
		Speed /= 10;
}

void ASpaceship::ToggleScan()
{
	TArray<AActor*> planets;
	UGameplayStatics::GetAllActorsOfClass( GetWorld(), APlanet::StaticClass(), planets );
	
	if ( PS && !PS->SpaceTraveling )
	{
		if (!PS->Scanning)
		{
			PS->Scanning = true;
			if (PS->InStarSystem)
			{
				for ( AActor* planet : planets )
					Cast<APlanet>(planet)->ScanningSphere->SetHiddenInGame(false);
			}
		}
		else
		{
			PS->Scanning = false;
			if (PS->InStarSystem)
			{
				for ( AActor* planet : planets )
					Cast<APlanet>(planet)->ScanningSphere->SetHiddenInGame(true);
			}
		}
	}
}

void ASpaceship::SpaceTravel()
{
	AActor* destination = nullptr;

	if (PS->ScanningStar)
	{
		if ( !PS->InStarSystem || ( PS->InStarSystem && ( PS->ScannedStar != PS->ClosestStar )))
			destination = PS->ScannedStar;
	}
	else if ( PS->ScanningPlanet && PS->ScannedPlanet != PS->ClosestPlanet )
		destination = PS->ScannedPlanet;

	if (!destination) return;

	FRotator rotation = UKismetMathLibrary::FindLookAtRotation( GetActorLocation(), destination->GetActorLocation() );
	SetActorRotation(rotation);

    if (PS->ScanningStar) Speed = FMath::Pow( 10, 9 );
	else Speed = 2 * FMath::Pow( 10, 7 );

	PS->Scanning = false;
	PS->ScanningStar = false;
	PS->ScanningPlanet = false;
	PS->SpaceTraveling = true;

	TArray<AActor*> planets;
	UGameplayStatics::GetAllActorsOfClass( GetWorld(), APlanet::StaticClass(), planets );
	for ( AActor* planet : planets )
		Cast<APlanet>(planet)->ScanningSphere->SetHiddenInGame(true);

    SpringArmCameraBack->bEnableCameraRotationLag = false;
	FTimerHandle delay;
	GetWorld()->GetTimerManager().SetTimer( delay, [&]()
	{
		SpringArmCameraBack->bEnableCameraRotationLag = true;
	}, 0.1f, false );
}

float ASpaceship::GetSpeed()
{
	return Speed;
}

void ASpaceship::Save()
{
    TArray<AActor*> universeArray;
    UGameplayStatics::GetAllActorsOfClass( GetWorld(), AUniverse::StaticClass(), universeArray );
    AUniverse* universe = Cast<AUniverse>(universeArray[0]);

    UGameInstanceND* gameInstance = Cast<UGameInstanceND>(GetWorld()->GetGameInstance());
    FString slot = gameInstance->SaveGamePlayerSlot;
    if (!UGameplayStatics::DoesSaveGameExist( slot, 0 )) return;
    USaveGamePlayer* saveGame = Cast<USaveGamePlayer>(UGameplayStatics::LoadGameFromSlot( slot, 0 ));
    FVector offset = FVector( universe->GetOffsetX(), universe->GetOffsetY(), universe->GetOffsetZ() );
    saveGame->Location = GetActorLocation() + offset;
    saveGame->Rotation = GetActorRotation();
    UGameplayStatics::SaveGameToSlot( saveGame, slot, 0 );
}