#include "AmmoComponent.h"

// CONSTRUCTOR - Disables ticking; the component is fully event-driven.
UAmmoComponent::UAmmoComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

// BEGIN PLAY - Fills the reserve to MaxReserveAmmo when play starts.
void UAmmoComponent::BeginPlay()
{
	Super::BeginPlay();

	CurrentReserveAmmo = MaxReserveAmmo;

}

// HAS AMMO - Returns true if there is any ammo in reserve.
bool UAmmoComponent::HasAmmo() const
{
	return CurrentReserveAmmo > 0;
}

// TAKE AMMO - Takes up to Requested ammo from the reserve and returns the amount actually taken.
// Broadcasts OnAmmoChanged, and OnAmmoDepleted if the reserve reaches zero.
int32 UAmmoComponent::TakeAmmo(int32 Requested)
{
    int32 AmmoToTake = FMath::Clamp(Requested, 0, CurrentReserveAmmo);

    if (AmmoToTake == 0)
    {
        return 0;
    }

    CurrentReserveAmmo -= AmmoToTake;

    OnAmmoChanged.Broadcast(CurrentReserveAmmo, MaxReserveAmmo);

    if (CurrentReserveAmmo == 0)
    {
        OnAmmoDepleted.Broadcast();
    }

    return AmmoToTake;
}

// ADD AMMO - Adds ammo to the reserve, clamped to MaxReserveAmmo. Ignores non-positive amounts.
// Broadcasts OnAmmoChanged if the reserve count changed.
void UAmmoComponent::AddAmmo(int32 Amount)
{
    if (Amount <= 0)
    {
        return;
    }

    int32 OldAmmo = CurrentReserveAmmo;

    CurrentReserveAmmo = FMath::Clamp(CurrentReserveAmmo + Amount, 0, MaxReserveAmmo);

    if (CurrentReserveAmmo != OldAmmo)
    {
        OnAmmoChanged.Broadcast(CurrentReserveAmmo, MaxReserveAmmo);
    }
}

// IS FULL - Returns true if the reserve is at (or above) MaxReserveAmmo.
bool UAmmoComponent::IsFull() const
{
	return CurrentReserveAmmo >= MaxReserveAmmo;
}

// GET CURRENT AMMO STATUS - Outputs the current reserve ammo and the maximum reserve ammo.
void UAmmoComponent::GetCurrentAmmoStatus(int32& OutCurrentAmmo, int32& OutMaxAmmo) const
{
	OutCurrentAmmo = CurrentReserveAmmo;
	OutMaxAmmo = MaxReserveAmmo;
}

// GET CURRENT RESERVE AMMO - Returns the current reserve ammo count.
int32  UAmmoComponent::GetCurrentReserveAmmo() const
{
    return CurrentReserveAmmo;
}
