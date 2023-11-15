// Copyright Sergio Calahorro

#pragma once

// Headers - Unreal Engine
#include "CoreMinimal.h"

/** Depth stencil value for red outline */
#define CUSTOM_DEPTH_RED 250

#pragma region COLLISIONS

/** Projectile's object channel */
#define ECC_Projectile ECollisionChannel::ECC_GameTraceChannel1

/** Target's object channel */
#define ECC_Target ECollisionChannel::ECC_GameTraceChannel2

#pragma endregion COLLISIONS