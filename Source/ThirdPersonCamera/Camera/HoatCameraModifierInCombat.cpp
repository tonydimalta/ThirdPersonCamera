#include "HoatCameraModifierInCombat.h"

#include "Camera/CameraTypes.h"
#include "Camera/CombatActorInterface.h"

bool UHoatCameraModifierInCombat::ModifyCamera(float DeltaTime, struct FMinimalViewInfo& InOutPOV)
{
    Super::ModifyCamera(DeltaTime, InOutPOV);

    // Get player character.
	ICombatActorInterface* combatActor = Cast<ICombatActorInterface>(GetViewTarget());

    if (!combatActor)
    {
        return false;
    }

    // Apply modifier.
    AppyCameraTransition(combatActor->IsInCombat() ? Modifiers : FHoatCameraInfo(), TransitionTime, InOutPOV,
                         DeltaTime);
    return false;
}
