#pragma once

#include "ThirdPersonCamera.h"
#include "Camera/PlayerCameraManager.h"
#include "HoatPlayerCameraManager.generated.h"

/** Defines the final view properties for the HOAT camera. */
UCLASS(notplaceable, transient, BlueprintType, Blueprintable)
class AHoatPlayerCameraManager : public APlayerCameraManager
{
    GENERATED_BODY()

public:
    AHoatPlayerCameraManager();

    /** Radius of the query probe for verifying line of sight between camera and view target (in cm). */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
    float LineOfSightProbeSize{ 12.0f };

    /** Collision channel of the query probe for verifying line of sight between camera and view target. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
    TEnumAsByte<ECollisionChannel> LineOfSightProbeChannel{ ECC_Camera };
};
