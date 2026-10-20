// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "RammsHUDPanelProvider.generated.h"

class SWidget;

/**
 * A panel a component offers the HUD: its content as a Slate widget, made on
 * demand, and whether it is wanted now. The HUD (RammsUI) hosts it like its
 * own panels -- a title bar to move it, minimise, hide or pop it out into a
 * window of its own, a grip to resize it, its place remembered per user --
 * without the component knowing anything of the HUD, or the HUD of the
 * component.
 */
struct FRammsHUDPanelDesc
{
	/** Unique per panel; also the key its layout is remembered by. */
	FName Id;
	FText Title;
	/** Makes the content; called once per panel the HUD hosts. The widget
	 *  should outlive its maker gracefully (hold the component weakly). */
	TFunction<TSharedRef<SWidget>()> MakeContent;
	/** Whether the panel should show now (the HUD polls it a few times a
	 *  second); unset is always. */
	TFunction<bool()> IsWanted;
	/** Where it first appears: its top-left as a fraction of the room the
	 *  viewport leaves around it (0,0 top-left; 1,1 bottom-right). */
	FVector2D DefaultPlacement = FVector2D(0.5, 1.0);
	/** Its first size (Slate units); 0 on an axis keeps the content's own. */
	FVector2D DefaultSize = FVector2D::ZeroVector;
	bool	  bResizable = true;
};

UINTERFACE(MinimalAPI)
class URammsHUDPanelProvider : public UInterface
{
	GENERATED_BODY()
};

/**
 * Implemented by a component (on the pawn a player controls) that offers HUD
 * panels: FRammsHUDPanelDesc. C++ only: the content is Slate.
 */
class RAMMSCONTROL_API IRammsHUDPanelProvider
{
	GENERATED_BODY()

public:
	virtual void GetHUDPanels(TArray<FRammsHUDPanelDesc>& OutPanels) const = 0;
};
