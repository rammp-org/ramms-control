// Copyright Epic Games, Inc. All Rights Reserved.

#include "RammsControlSurfaceRegistry.h"

#include "RammsControlSurfaceProvider.h"

void URammsControlSurfaceRegistry::CleanupStale()
{
	Registered.RemoveAll([](const TWeakObjectPtr<UObject>& Weak) { return !Weak.IsValid(); });
}

void URammsControlSurfaceRegistry::RegisterControlSurface(UObject* Provider)
{
	if (!Provider)
	{
		return;
	}
	if (!Provider->GetClass()->ImplementsInterface(URammsControlSurfaceProvider::StaticClass()))
	{
		UE_LOG(LogTemp, Warning,
			TEXT("[ControlSurfaceRegistry] '%s' does not implement IRammsControlSurfaceProvider; not registered."),
			*Provider->GetName());
		return;
	}
	CleanupStale();
	if (Registered.ContainsByPredicate(
			[Provider](const TWeakObjectPtr<UObject>& Weak) { return Weak.Get() == Provider; }))
	{
		return;
	}
	Registered.Add(Provider);
	OnControlSurfaceRegistryChanged.Broadcast(Provider, true);
}

void URammsControlSurfaceRegistry::UnregisterControlSurface(UObject* Provider)
{
	if (!Provider)
	{
		return;
	}
	const int32 Removed = Registered.RemoveAll(
		[Provider](const TWeakObjectPtr<UObject>& Weak) { return Weak.Get() == Provider; });
	CleanupStale();
	if (Removed > 0)
	{
		OnControlSurfaceRegistryChanged.Broadcast(Provider, false);
	}
}

UObject* URammsControlSurfaceRegistry::FindControlSurface()
{
	CleanupStale();
	return Registered.Num() > 0 ? Registered[0].Get() : nullptr;
}

UObject* URammsControlSurfaceRegistry::FindControlSurfaceByRobotName(const FString& RobotName)
{
	CleanupStale();
	for (const TWeakObjectPtr<UObject>& Weak : Registered)
	{
		UObject* Provider = Weak.Get();
		if (!Provider)
		{
			continue;
		}
		const FRammsControlSurface Surface =
			IRammsControlSurfaceProvider::Execute_GetControlSurface(Provider);
		if (Surface.RobotName.ToString() == RobotName)
		{
			return Provider;
		}
	}
	return nullptr;
}

TArray<UObject*> URammsControlSurfaceRegistry::GetAllControlSurfaces()
{
	CleanupStale();
	TArray<UObject*> Out;
	Out.Reserve(Registered.Num());
	for (const TWeakObjectPtr<UObject>& Weak : Registered)
	{
		if (UObject* Provider = Weak.Get())
		{
			Out.Add(Provider);
		}
	}
	return Out;
}

int32 URammsControlSurfaceRegistry::GetControlSurfaceCount()
{
	CleanupStale();
	return Registered.Num();
}
