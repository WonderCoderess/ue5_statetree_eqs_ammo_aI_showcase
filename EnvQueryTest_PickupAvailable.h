#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryTest.h"
#include "EnvQueryTest_PickupAvailable.generated.h"

/*
 * EQS test that checks whether actor items are AAmmoPickups that are currently available.
 * Bool test: actors that are not ammo pickups count as unavailable. Use the inherited BoolValue
 * to choose whether to match available (true) or unavailable (false) pickups.
 */
UCLASS()
class ASOULINTHREE_API UEnvQueryTest_PickupAvailable : public UEnvQueryTest
{
    GENERATED_BODY()

public:
    /* Sets a low cost, makes this a bool (not float) test and restricts it to actor items. */
    UEnvQueryTest_PickupAvailable();

    /* Returns the title shown for this test in the EQS editor ("Pickup Available"). */
    virtual FText GetDescriptionTitle() const override;

protected:
    /* Scores/filters every item by whether it is an available AAmmoPickup, compared against BoolValue. */
    virtual void RunTest(FEnvQueryInstance& QueryInstance) const override;
};
