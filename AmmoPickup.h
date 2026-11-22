#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "AmmoPickup.generated.h"

class USphereComponent;
class UStaticMeshComponent;

/*
 * World pickup that refills the ammo reserve of any actor with a UAmmoComponent.
 * After being collected it hides itself and respawns after RespawnTime seconds.
 */
UCLASS()
class ASOULINTHREE_API AAmmoPickup : public AActor
{
	GENERATED_BODY()

public:
	/* Creates the sphere collision (root, overlaps Pawns only) and the non-colliding visual mesh. */
	AAmmoPickup();

	/* Returns true if the pickup is currently visible and can be collected. */
	UFUNCTION(BlueprintPure, Category = "Ammo")
	bool IsAvailable() const;

protected:
	/* Binds OnOverlap to the sphere collision's begin-overlap event. */
	virtual void BeginPlay() override;

	/* Root trigger volume; detects Pawns entering the pickup radius. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USphereComponent* SphereCollision;

	/* Visual representation of the pickup; has no collision. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* Mesh;

	/* Amount of ammo added to the collector's AmmoComponent reserve. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ammo", meta = (ClampMin = "1"))
	int32 AmmoAmount = 30;

	/* Time in seconds after being collected before the pickup becomes available again. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ammo", meta = (ClampMin = "0.0"))
	float RespawnTime = 10.0f;

	/* Whether the pickup is currently available (visible and collectable). */
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Ammo")
	bool bIsAvailable = true;

	/* Handle for the timer that calls RespawnPickup after the pickup is collected. */
	FTimerHandle RespawnTimer;

	/*
	 * Called when an actor overlaps the sphere collision. If the pickup is available and the
	 * actor has a non-full UAmmoComponent, adds AmmoAmount, disables the pickup and starts the respawn timer.
	 */
	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/* Sets availability and toggles the actor's visibility and collision to match. */
	void SetAvailable(bool bNewAvailable);

	/* Timer callback that makes the pickup available again. */
	void RespawnPickup();

	/* Clears the respawn timer so it can't fire after the actor is removed. */
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

};
