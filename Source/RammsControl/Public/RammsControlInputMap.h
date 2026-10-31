// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "RammsControlTypes.h"
#include "RammsControlInputMap.generated.h"

class UInputAction;
class UInputMappingContext;

/** How an input action's value reaches a control. */
UENUM(BlueprintType)
enum class ERammsControlInputMode : uint8
{
	/** The action's value (× Scale) is the axis value; Completed releases it.
	 *  A 2-D action feeds ControlId with X and ControlIdY with Y. */
	Axis UMETA(DisplayName = "Axis"),
	/** Fires the control once per press, when the action first triggers (a
	 *  Hold trigger on the mapping makes it wait): an Action control, or an
	 *  Enum one's next choice (drive.mode: the next drive mode). */
	Action UMETA(DisplayName = "Action"),
	/** While the action is held, the Position / Velocity control's target
	 *  moves at RatePerSecond × value (the held-key behaviour of the
	 *  keyboard teleop's motor groups), clamped to the control's range. */
	IncrementRate UMETA(DisplayName = "Increment at rate"),
	/** A drive stick, as Axis, with X following the surface's pairing: Y
	 *  feeds ControlIdY; X feeds the control the surface pairs with it
	 *  (FRammsControlAxis::PairedAxis: turn on a differential drive, strafe on
	 *  a holonomic one), else ControlId; Z feeds ControlIdZ unless X or Y
	 *  already does. So a 3-axis stick's twist turns a holonomic base and
	 *  does nothing on a differential one, whose stick already turns it --
	 *  re-resolved whenever the surface changes, as a drive-mode switch does. */
	Stick UMETA(DisplayName = "Stick (X follows the pairing)"),
};

USTRUCT(BlueprintType)
struct RAMMSCONTROL_API FRammsControlInputBinding
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Binding")
	TObjectPtr<UInputAction> Action;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Binding")
	ERammsControlInputMode Mode = ERammsControlInputMode::Axis;

	/** Control fed by the action's value (its X for a 2-D / 3-D action). May be
	 *  a wildcard ("linkage.*.height", "motor.*_front_crank"): every control on
	 *  the surface whose Id matches is driven. For Stick, the fallback for X
	 *  when ControlIdY has no partner on the surface. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Binding")
	FName ControlId;

	/** For a 2-D / 3-D action: the control(s) fed by its Y. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Binding")
	FName ControlIdY;

	/** For a 3-D action: the control(s) fed by its Z. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Binding")
	FName ControlIdZ;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Binding")
	float Scale = 1.0f;

	/** IncrementRate only: control units per second at full deflection. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Binding", meta = (ClampMin = "0.0"))
	float RatePerSecond = 1.0f;
};

/**
 * Keyboard / gamepad → control surface, as data: the Enhanced Input mapping
 * context that supplies the actions and how each action drives a control Id.
 * One asset per robot family (or one shared one — Ids are the contract, not
 * component types), so every key is visible and editable in one place and
 * gamepad bindings come with the mapping context for free.
 */
UCLASS(BlueprintType)
class RAMMSCONTROL_API URammsControlInputMap : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TObjectPtr<UInputMappingContext> MappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	int32 Priority = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TArray<FRammsControlInputBinding> Bindings;
};
