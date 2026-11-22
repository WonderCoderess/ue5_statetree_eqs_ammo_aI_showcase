#include "EnvQueryTest_PickupAvailable.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_ActorBase.h"
#include "AmmoPickup.h"

// ENV QUERY TEST PICKUP AVAILABLE - Sets a low cost, makes this a bool (not float) test and restricts it to actor items.
UEnvQueryTest_PickupAvailable::UEnvQueryTest_PickupAvailable()
{
    Cost = EEnvTestCost::Low;

    SetWorkOnFloatValues(false);

    ValidItemType = UEnvQueryItemType_ActorBase::StaticClass();
}

// RUN TEST - Scores/filters every item by whether it is an available AAmmoPickup.
// Items match when their availability equals BoolValue; non-pickup actors count as unavailable.
void UEnvQueryTest_PickupAvailable::RunTest(FEnvQueryInstance& QueryInstance) const
{
    UObject* QueryOwner = QueryInstance.Owner.Get();

    if (QueryOwner == nullptr)
    {
        return;
    }

    // Resolve BoolValue (may be bound to a query param) for this query instance
    BoolValue.BindData(QueryOwner, QueryInstance.QueryID);

    // Whether we want available (true) or unavailable (false) pickups
    bool bWantsAvailable = BoolValue.GetValue();

    for (FEnvQueryInstance::ItemIterator It(this, QueryInstance); It; ++It)
    {
        AActor* Actor = GetItemActor(QueryInstance, It.GetIndex());

        // Check if the actor is an ammo pickup
        AAmmoPickup* Pickup = Cast<AAmmoPickup>(Actor);

        // Check if the pickup exists and is available
        bool bIsAvailable = Pickup && Pickup->IsAvailable();

        // Pass if bIsAvailable matches bWantsAvailable
        It.SetScore(TestPurpose, FilterType, bIsAvailable, bWantsAvailable);
    }
}

// GET DESCRIPTION TITLE - Returns the title shown for this test in the EQS editor.
FText UEnvQueryTest_PickupAvailable::GetDescriptionTitle() const
{
    return FText::FromString(TEXT("Pickup Available"));
}
