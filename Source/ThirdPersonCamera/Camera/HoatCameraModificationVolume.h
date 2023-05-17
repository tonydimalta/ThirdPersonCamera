#pragma once

#include "ThirdPersonCamera.h"
#include "Engine/TriggerVolume.h"
#include "Camera/HoatCameraInfo.h"
#include "HoatCameraModificationVolume.generated.h"

/** Volume that modifies the camera of entering players. */
UCLASS()
class AHoatCameraModificationVolume : public ATriggerVolume
{
    GENERATED_BODY()

public:
    /** Sets the currently applied camera modification volume for the specified actor. */
    virtual void NotifyActorBeginOverlap(AActor* Other) override;

    // Resets the currently applied camera modification volume for the specified actor. */
    virtual void NotifyActorEndOverlap(AActor* Other) override;

    /** Time until the modifications are fully applied, in seconds. */
    UPROPERTY(Category = CameraModifiers, BlueprintReadWrite, EditAnywhere)
    float TransitionTime{ 1.0f };

    /** Value to add to the current properties of the camera of any player entering the volume. */
    UPROPERTY(Category = CameraModifiers, BlueprintReadWrite, EditAnywhere)
    FHoatCameraInfo Modifiers;
};
