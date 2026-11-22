#pragma once

#include "CoreMinimal.h"
#include "StateTreeConditionBase.h"
#include "AmmoStateTreeConditions.generated.h"

class AShooterNPC;

USTRUCT()
struct FStateTreeOutOfAmmoConditionInstanceData
{
    GENERATED_BODY()

    /** The NPC for which to check ammo status */
    UPROPERTY(EditAnywhere, Category = "Context")
    TObjectPtr<AShooterNPC> Character = nullptr;

	/** Whether the condition should be true when the character is out of ammo (true) or has ammo (false) */
    UPROPERTY(EditAnywhere, Category = "Condition")
    bool bMustBeOutOfAmmo = true;
};

USTRUCT(DisplayName = "Is Out Of Ammo", Category = "Shooter")
struct ASOULINTHREE_API FStateTreeOutOfAmmoCondition : public FStateTreeConditionCommonBase
{
    GENERATED_BODY()

    using FInstanceDataType = FStateTreeOutOfAmmoConditionInstanceData;

    virtual const UStruct* GetInstanceDataType() const override
    {
        return FInstanceDataType::StaticStruct();
    }

    virtual bool TestCondition(FStateTreeExecutionContext& Context) const override;

#if WITH_EDITOR
    virtual FText GetDescription(
        const FGuid& ID,
        FStateTreeDataView InstanceDataView,
        const IStateTreeBindingLookup& BindingLookup,
        EStateTreeNodeFormatting Formatting = EStateTreeNodeFormatting::Text
    ) const override;
#endif
};