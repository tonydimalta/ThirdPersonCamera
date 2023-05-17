#include "HoatCameraSpringArmComponent.h"

UHoatCameraSpringArmComponent::UHoatCameraSpringArmComponent()
{
    // Enable camera controls.
    bUsePawnControlRotation = true;

    // Set reasonable default values for camera distance and perspective.
    TargetArmLength = BaseTargetArmLength;
    SocketOffset = FVector(0.0f, 40.0f, 40.0f);

    // Enable camera lag.
    bEnableCameraLag = true;
    CameraLagSpeed = 5.0f;
}

void UHoatCameraSpringArmComponent::UpdateDesiredArmLocation(bool bDoTrace, bool bDoLocationLag, bool bDoRotationLag,
                                                             float DeltaTime)
{
    TargetArmLength = BaseTargetArmLength + TargetArmLengthModifier;
    Super::UpdateDesiredArmLocation(bDoTrace, bDoLocationLag, bDoRotationLag, DeltaTime);
    TargetArmLengthModifier = 0.0f;
}
