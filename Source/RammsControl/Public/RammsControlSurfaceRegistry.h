// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"

#include "RammsControlSurfaceRegistry.generated.h"

/**
 * Every control surface in the world, so nothing has to name a robot.
 *
 * A robot registers the object implementing IRammsControlSurfaceProvider (and
 * usually IRammsControlSink) on BeginPlay and unregisters on EndPlay. Panels,
 * input components and external clients ask here instead of searching the
 * level for a class they would then be coupled to.
 *
 * This lives beside the control model rather than in the UI plugin, because
 * the robot side needs it too: registering used to mean depending on RammsUI,
 * which meant depending on Slate and the streaming plugin to be discoverable.
 *
 * Entries are weak, and a robot that is destroyed without unregistering is
 * dropped the next time anyone looks.
 */
UCLASS()
class RAMMSCONTROL_API URammsControlSurfaceRegistry : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	/** Register a provider (must implement IRammsControlSurfaceProvider). */
	UFUNCTION(BlueprintCallable, Category = "Ramms|Control Surfaces")
	void RegisterControlSurface(UObject* Provider);

	/** Unregister a provider. Call from EndPlay. */
	UFUNCTION(BlueprintCallable, Category = "Ramms|Control Surfaces")
	void UnregisterControlSurface(UObject* Provider);

	/** First registered, still-valid provider; nullptr if none. */
	UFUNCTION(BlueprintCallable, Category = "Ramms|Control Surfaces")
	UObject* FindControlSurface();

	/** Registered provider whose surface's RobotName matches; nullptr if none. */
	UFUNCTION(BlueprintCallable, Category = "Ramms|Control Surfaces")
	UObject* FindControlSurfaceByRobotName(const FString& RobotName);

	/** All registered, still-valid providers. */
	UFUNCTION(BlueprintCallable, Category = "Ramms|Control Surfaces")
	TArray<UObject*> GetAllControlSurfaces();

	UFUNCTION(BlueprintPure, Category = "Ramms|Control Surfaces")
	int32 GetControlSurfaceCount();

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnControlSurfaceRegistryChanged, UObject*, Provider, bool, bRegistered);

	/** Fired on every register / unregister, so a panel can re-resolve. */
	UPROPERTY(BlueprintAssignable, Category = "Ramms|Control Surfaces")
	FOnControlSurfaceRegistryChanged OnControlSurfaceRegistryChanged;

private:
	TArray<TWeakObjectPtr<UObject>> Registered;
	void							CleanupStale();
};
