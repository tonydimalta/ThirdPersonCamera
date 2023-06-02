#pragma once

#include "ThirdPersonCamera.h"
#include "Camera/CameraModifier.h"
#include "HoatCameraModifierApplyPitchCurves.generated.h"

/** Applies curves in order to modify camera properties based on its current pitch. */
UCLASS(BlueprintType, Blueprintable)
class UHoatCameraModifierApplyPitchCurves : public UCameraModifier
{
    GENERATED_BODY()

public:
    virtual bool ModifyCamera(float DeltaTime, struct FMinimalViewInfo& InOutPOV) override;

private:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Custom, meta =
        (AllowPrivateAccess = "true", InlineEditConditionToggle))
    bool bEnablePitchToDistance{ false };

    // Curve to convert changes in pitch to changes in camera distance.
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Custom, meta =
        (AllowPrivateAccess = "true", EditCondition = "bEnablePitchToDistance"))
    TObjectPtr<UCurveFloat> PitchToDistanceCurve;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Custom, meta =
        (AllowPrivateAccess = "true", InlineEditConditionToggle))
    bool bEnablePitchToFOV{ false };

    // Curve to convert changes in pitch to changes in field of view.
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Custom, meta =
        (AllowPrivateAccess = "true", EditCondition = "bEnablePitchToFOV"))
    TObjectPtr<UCurveFloat> PitchToFOVCurve;
};
