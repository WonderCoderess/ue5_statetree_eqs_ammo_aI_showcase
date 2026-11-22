#include "AmmoStateTreeConditions.h"
#include "Variant_Shooter/AI/ShooterNPC.h"
#include "StateTreeExecutionContext.h"

// TEST CONDITION - Returns true if the NPC is out of ammo (or has ammo) based on the bMustBeOutOfAmmo flag.
bool FStateTreeOutOfAmmoCondition::TestCondition(FStateTreeExecutionContext& Context) const
{
	const FInstanceDataType& InstanceData = Context.GetInstanceData(*this);

	if (!IsValid(InstanceData.Character))
	{
		return false;
	}

	const bool bIsOutOfAmmo = !InstanceData.Character->HasAnyAmmo();

	return bIsOutOfAmmo == InstanceData.bMustBeOutOfAmmo;
}

#if WITH_EDITOR
FText FStateTreeOutOfAmmoCondition::GetDescription(
	const FGuid& ID,
	FStateTreeDataView InstanceDataView,
	const IStateTreeBindingLookup& BindingLookup,
	EStateTreeNodeFormatting Formatting
) const
{
	return FText::FromString(TEXT("Check NPC ammo availability"));
}
#endif