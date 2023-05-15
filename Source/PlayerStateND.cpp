// Copyright (c) Parallax 2023

#include "PlayerStateND.h"

APlayerStateND::APlayerStateND()
{
    Health = 0;
    ClosestStar   = nullptr;
    ClosestPlanet = nullptr;
    ScannedStar   = nullptr;
    ScannedPlanet = nullptr;
    InSpaceship    = false;
    InStarSystem   = false;
    InPlanet       = false;
    InPlanetOrbit  = false;
    Scanning       = false;
    ScanningStar   = false;
    ScanningPlanet = false;
    SpaceTraveling = false;
    PlanetSpinning = false;
}