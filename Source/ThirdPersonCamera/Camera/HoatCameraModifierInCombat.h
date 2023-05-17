#pragma once

#include "ThirdPersonCamera.h"
#include "Camera/HoatCameraModifier.h"
#include "HoatCameraModifierInCombat.generated.h"

/** Applies modifications while the player is in combat. */
UCLASS(BlueprintType, Blueprintable)
class UHoatCameraModifierInCombat : public UHoatCameraModifier
{
    GENERATED_BODY()

public:
    virtual bool ModifyCamera(float DeltaTime, struct FMinimalViewInfo& InOutPOV) override;

private:
    /** Time until the modifications are fully applied, in seconds. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CameraModifiers, meta = (AllowPrivateAccess = "true"))
    float TransitionTime{ 1.0f };

    /** Value to add to the current properties of the camera of any player in combat. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CameraModifiers, meta = (AllowPrivateAccess = "true"))
    FHoatCameraInfo Modifiers;
};
