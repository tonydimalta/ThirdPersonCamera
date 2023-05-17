#pragma once

#include "ThirdPersonCamera.h"
#include "Camera/HoatCameraModifier.h"
#include "HoatCameraModifierFocusTargetActor.generated.h"

/** Makes the camera look in the direction of the current target actor, if there is one. */
UCLASS(BlueprintType, Blueprintable)
class UHoatCameraModifierFocusTargetActor : public UHoatCameraModifier
{
    GENERATED_BODY()

public:
    virtual bool ProcessViewRotation(class AActor* ViewTarget, float DeltaTime, FRotator& OutViewRotation,
                                     FRotator& OutDeltaRot) override;

private:
    /** Rate at which the camera should move while not snapping to a target, in degrees per second. */
    UPROPERTY(EditDefaultsOnly, Category = Custom, meta = (AllowPrivateAccess = "true"))
    float RotationSpeed{ 300.0f };

    /** Rate at which the camera should snap to the target, in degrees per second. */
    UPROPERTY(EditDefaultsOnly, Category = Custom, meta = (AllowPrivateAccess = "true"))
    float SnapSpeed{ 50.0f };

    /** Target actor that was selected at the time of the previous camera update. */
    AActor* LastTarget;

    /** Desired rotation for focusing the target actor. */
    FRotator DesiredRotation;
};
