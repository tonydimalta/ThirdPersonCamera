#include "HoatCameraSpringArmComponent.h"

UHoatCameraSpringArmComponent::UHoatCameraSpringArmComponent()
{
    // Enable camera controls.
    bUsePawnControlRotation = true;

    // Set reasonable default values for camera distance and perspective.
    InitialTargetArmLength = TargetArmLength = 300.0f;
    SocketOffset = FVector(0.0f, 40.0f, 0.0f);
    TargetOffset = FVector(0.0f, 0.0f, 40.0f);

    // Enable camera lag.
    bEnableCameraLag = true;
    CameraLagSpeed = 5.0f;
}

void UHoatCameraSpringArmComponent::UpdateDesiredArmLocation(bool bDoTrace, bool bDoLocationLag, bool bDoRotationLag,
                                                             float DeltaTime)
{
    TargetArmLength = InitialTargetArmLength + TargetArmLengthModifier;
    Super::UpdateDesiredArmLocation(bDoTrace, bDoLocationLag, bDoRotationLag, DeltaTime);
    TargetArmLengthModifier = 0.0f;

    if (!bIsCameraFixed)
    {
        FixedCameraPosition = UnfixedCameraPosition;
    }
}

FVector UHoatCameraSpringArmComponent::BlendLocations(const FVector& DesiredArmLocation, const FVector& TraceHitLocation, bool bHitSomething, float DeltaTime)
{
    FVector TargetLocation = Super::BlendLocations(DesiredArmLocation, TraceHitLocation, bHitSomething, DeltaTime);
    
    if (bEnableCameraLag && CameraLagPullBackSpeed > 0.f)
    {
        // Note that "previous" values have just been updated in the caller UpdateDesiredArmLocation so they are equal to "current".
        FVector CameraToFixedPos = FixedCameraPosition - PreviousArmOrigin;
        FVector CameraToTarget = TargetLocation - PreviousArmOrigin;
        // Introducing a minimum distance to prevent excessive interpolation breaking the pawn's camera rotation.
        double MinDistanceSquared = CameraLagPullBackMinDistance * CameraLagPullBackMinDistance;
        // If the new target is further away, it means we're pulling the camera away from the player.
        bool bIsPullingAway = (CameraToTarget.SizeSquared() - CameraToFixedPos.SizeSquared()) >= MinDistanceSquared;

        if (bIsPullingAway)
        {
            // Ease out when the camera pulls back from the player.
            TargetLocation = FMath::VInterpTo(FixedCameraPosition, TargetLocation, DeltaTime, CameraLagPullBackSpeed);
        }
    }

    if (TargetLocation != DesiredArmLocation)
    {
        FixedCameraPosition = TargetLocation;
    }

    return TargetLocation;
}
