#pragma once

#include "ThirdPersonCamera.h"
#include "Camera/HoatCameraModifier.h"
#include "Engine/EngineTypes.h"
#include "HoatCameraModifierKeepLineOfSightPreemptive.generated.h"

/**
 * Makes the camera try to always keep line of sight to its target.
 * Preemptively swings away from obstacles before they occlude the character.
 */
UCLASS(BlueprintType, Blueprintable)
class UHoatCameraModifierKeepLineOfSightPreemptive : public UHoatCameraModifier
{
    GENERATED_BODY()

public:
    virtual bool ProcessViewRotation(class AActor* ViewTarget, float DeltaTime, FRotator& OutViewRotation,
                                     FRotator& OutDeltaRot) override;
    virtual bool ModifyCamera(float DeltaTime, struct FMinimalViewInfo& InOutPOV) override;

private:
    /** Checks whether To can be seen from From, with respect to ProbeSize and ProbeChannel. */
    bool IsInLineOfSight(const FVector& From, const FVector& To) const;

    /** Rotates InOutViewLocation and InOutViewRotation around TargetLocation by AngleInRadians. */
    void RotateAroundLocation(const FVector& TargetLocation, FVector& InOutViewLocation, FRotator& InOutViewRotation,
                              const float AngleInRadians);

    /** How far away from the current camera angle we should look for potential obstacles. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
    float MaximumAngleInRadians{ PI / 2 };

    /** How precisely we should preemptively look for obstacles. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
    float StepSizeInRadians{ 0.1f };

    /** Rate at which the camera should follow the turning character, in radians per second. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
    float RotationSpeed{ 1.0f };

    /** Collision channel of the query probe for verifying line of sight between camera and view target. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
    TEnumAsByte<ECollisionChannel> LineOfSightProbeChannel{ ECC_Camera };
};
