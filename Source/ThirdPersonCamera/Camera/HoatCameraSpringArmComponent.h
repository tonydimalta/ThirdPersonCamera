#pragma once

#include "ThirdPersonCamera.h"
#include "GameFramework/SpringArmComponent.h"
#include "HoatCameraSpringArmComponent.generated.h"

/**
 * Extends the default Unreal Engine spring arm camera behavior to match the specific needs of HOAT.
 */
UCLASS(Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent), hideCategories = (Mobility))
class UHoatCameraSpringArmComponent : public USpringArmComponent
{
    GENERATED_BODY()

public:
    UHoatCameraSpringArmComponent();

    /** Value to add to the target arm length, e.g. caused by camera modifiers. */
    float TargetArmLengthModifier{ 0.0f };

protected:
    virtual void UpdateDesiredArmLocation(bool bDoTrace, bool bDoLocationLag, bool bDoRotationLag,
                                          float DeltaTime) override;

    virtual FVector BlendLocations(const FVector& DesiredArmLocation, const FVector& TraceHitLocation,
                                   bool bHitSomething, float DeltaTime) override;

    // If bEnableCameraLag is true, controls how quickly camera pulls back to reach the target position (usually after colliding
    // with an obstacle). Low values are slower (more lag), high values are faster (less lag), while zero is instant (no lag).
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Lag, meta =
        (EditCondition = "bEnableCameraLag", ClampMin = "0.0", ClampMax = "1000.0", UIMin = "0.0", UIMax = "1000.0",
         DisplayAfter = "CameraLagMaxDistance"))
    float CameraLagPullBackSpeed{ 5.0f };

    // Min distance the camera target needs to travel to trigger the pull back lag. If set to zero, no min distance is enforced.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Lag, meta =
        (EditCondition = "bEnableCameraLag", ClampMin = "0.0", UIMin = "0.0", DisplayAfter = "CameraLagMaxDistance"))
    float CameraLagPullBackMinDistance{ 100.0f };

    float InitialTargetArmLength{ 0.0f };
    FVector FixedCameraPosition{ FVector::ZeroVector };
};
