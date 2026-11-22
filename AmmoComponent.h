#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AmmoComponent.generated.h"


/* Broadcast whenever the reserve ammo count changes. Passes the new reserve count and the maximum reserve. */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAmmoChanged, int32, CurrentAmmo, int32, MaxAmmo);

/* Broadcast when the reserve ammo reaches zero. */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAmmoDepleted);

/*
 * Holds a limited reserve of ammo for its owner (e.g. an enemy). Weapons take ammo from it
 * when reloading, and pickups such as AAmmoPickup refill it.
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ASOULINTHREE_API UAmmoComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	/* Disables ticking; the component is fully event-driven. */
	UAmmoComponent();

	/* Returns true if there is any ammo in reserve. */
	UFUNCTION(BlueprintPure, Category = "Ammo")
	bool HasAmmo() const;

	/*
	 * Takes up to Requested ammo from the reserve. Returns the amount actually taken
	 * (less than Requested if the reserve runs short). Broadcasts OnAmmoChanged, and
	 * OnAmmoDepleted if the reserve reaches zero.
	 */
	UFUNCTION(BlueprintCallable, Category = "Ammo")
	int32 TakeAmmo(int32 Requested);

	/*
	 * Adds ammo to the reserve, clamped to MaxReserveAmmo. Ignores non-positive amounts.
	 * Broadcasts OnAmmoChanged if the reserve count changed.
	 */
	UFUNCTION(BlueprintCallable, Category = "Ammo")
	void AddAmmo(int32 Amount);

	/* Returns true if the reserve is at (or above) MaxReserveAmmo. */
	UFUNCTION(BlueprintPure, Category = "Ammo")
	bool IsFull() const;

	/* Fired whenever the reserve ammo count changes (ammo taken or added). */
	UPROPERTY(BlueprintAssignable, Category = "Ammo")
	FOnAmmoChanged OnAmmoChanged;

	/* Fired when taking ammo empties the reserve. */
	UPROPERTY(BlueprintAssignable, Category = "Ammo")
	FOnAmmoDepleted OnAmmoDepleted;

	/* Outputs the current reserve ammo and the maximum reserve ammo. */
	UFUNCTION(BlueprintPure, Category = "Ammo")
	void GetCurrentAmmoStatus(int32& OutCurrentAmmo, int32& OutMaxAmmo) const;

	/* Returns the current reserve ammo count. */
	UFUNCTION(BlueprintPure, Category = "Ammo")
	int32 GetCurrentReserveAmmo() const;

protected:
	/* Fills the reserve to MaxReserveAmmo when play starts. */
	virtual void BeginPlay() override;

	/* Maximum ammo that can be held in reserve. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ammo", meta = (ClampMin = 0))
	int32 MaxReserveAmmo = 100;

	/* Current ammo in reserve. This is the ammo that can be used to reload the weapon. Set to MaxReserveAmmo in BeginPlay. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ammo")
	int32 CurrentReserveAmmo = 100;
};
