#pragma once

#include "ThirdPersonCamera.h"
#include "Camera/HoatCameraModifier.h"
#include "HoatCameraModifierFocusWalkDirection.generated.h"

/** Makes the camera always look in walk direction. */
UCLASS(BlueprintType, Blueprintable)
class UHoatCameraModifierFocusWalkDirection : public UHoatCameraModifier
{
    GENERATED_BODY()

public:
    virtual bool ModifyCamera(float DeltaTime, struct FMinimalViewInfo& InOutPOV) override;
    virtual bool ProcessViewRotation(class AActor* ViewTarget, float DeltaTime, FRotator& OutViewRotation, FRotator& OutDeltaRot) override;

private:
    /** Rate at which the camera should follow the turning character, in degrees per second. */
    UPROPERTY(EditDefaultsOnly, Category = Custom, meta = (AllowPrivateAccess = "true"))
    float RotationSpeed{ 10.0f };

    /** Minimum time between two changes in direction of automatic camera rotation, in seconds. */
    UPROPERTY(EditDefaultsOnly, Category = Custom, meta = (AllowPrivateAccess = "true"))
    float DirectionChangeCooldown{ 0.0f };

    /** Whether to always rotate the camera to focus walk direction, or only while the player is providing movement input. */
    UPROPERTY(EditDefaultsOnly, Category = Custom, meta = (AllowPrivateAccess = "true"))
    bool RotateOnlyWhileCharacterIsMoving{ false };

    float PreviousYawDeltaSign;
    float DirectionChangeCooldownRemaining;
};
