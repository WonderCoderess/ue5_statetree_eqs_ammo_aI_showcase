#include "AmmoPickup.h"

#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "AmmoComponent.h"

// AMMO PICKUP - Creates the sphere collision (root, overlaps Pawns only) and the non-colliding visual mesh.
AAmmoPickup::AAmmoPickup()
{
	PrimaryActorTick.bCanEverTick = false;

	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	RootComponent = SphereCollision;

	SphereCollision->SetSphereRadius(100.0f);
	SphereCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	SphereCollision->SetGenerateOverlapEvents(true);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(SphereCollision);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// BEGIN PLAY - Binds OnOverlap to the sphere collision's begin-overlap event.
void AAmmoPickup::BeginPlay()
{
	Super::BeginPlay();

	SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &AAmmoPickup::OnOverlap);
}

// ON OVERLAP - Called when an actor overlaps the sphere collision. If the pickup is available and the actor has a
// non-full UAmmoComponent, adds AmmoAmount, disables the pickup and starts the respawn timer.
void AAmmoPickup::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//UE_LOG(LogTemp, Warning, TEXT("AMMO PICKUP OVERLAP: %s"), *GetNameSafe(OtherActor));

	if (!bIsAvailable)
	{
		return;
	}

	if (OtherActor == nullptr)
	{
		return;
	}

	UAmmoComponent* AmmoComponent = OtherActor->FindComponentByClass<UAmmoComponent>();

	if (AmmoComponent == nullptr)
	{
		return;
	}

	if (AmmoComponent->IsFull())
	{
		return;
	}

	AmmoComponent->AddAmmo(AmmoAmount);

	SetAvailable(false);

	GetWorldTimerManager().SetTimer(RespawnTimer, this, &AAmmoPickup::RespawnPickup, RespawnTime, false);
}

// SET AVAILABLE - Sets availability and toggles the actor's visibility and collision to match.
void AAmmoPickup::SetAvailable(bool bNewAvailable)
{
	bIsAvailable = bNewAvailable;

	SetActorHiddenInGame(!bIsAvailable);
	SetActorEnableCollision(bIsAvailable);
}

// RESPAWN PICKUP - Timer callback that makes the pickup available again.
void AAmmoPickup::RespawnPickup()
{
	SetAvailable(true);
}

// END PLAY - Clears the respawn timer so it can't fire after the actor is removed.
void AAmmoPickup::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	GetWorldTimerManager().ClearTimer(RespawnTimer);

	Super::EndPlay(EndPlayReason);
}

// IS AVAILABLE - Returns true if the pickup is currently visible and can be collected.
bool AAmmoPickup::IsAvailable() const
{
	return bIsAvailable;
}
