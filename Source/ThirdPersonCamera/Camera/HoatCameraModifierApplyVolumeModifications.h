#pragma once

#include "ThirdPersonCamera.h"
#include "Camera/HoatCameraModifier.h"
#include "HoatCameraModifierApplyVolumeModifications.generated.h"

/** Applies modifications caused by level volumes the player enters. */
UCLASS(BlueprintType, Blueprintable)
class UHoatCameraModifierApplyVolumeModifications : public UHoatCameraModifier
{
    GENERATED_BODY()

public:
    virtual bool ModifyCamera(float DeltaTime, struct FMinimalViewInfo& InOutPOV) override;
};
