#include "HoatCameraModifierApplyPitchCurves.h"

#include "Camera/CameraTypes.h"
#include "Camera/HoatPlayerCameraManager.h"

bool UHoatCameraModifierApplyPitchCurves::ModifyCamera(float DeltaTime, struct FMinimalViewInfo& InOutPOV)
{
    Super::ModifyCamera(DeltaTime, InOutPOV);

    AHoatPlayerCameraManager* cameraManager = Cast<AHoatPlayerCameraManager>(CameraOwner);

    if (!IsValid(cameraManager))
    {
        return false;
    }

    // Get camera location and rotation.
    const FVector cameraLocation = InOutPOV.Location;
    const FRotator cameraRotation = InOutPOV.Rotation;

    // Apply distance curve.
    if (bEnablePitchToDistance && IsValid(PitchToDistanceCurve))
    {
        // Drive camera away based on evaluated curve.
        const float pitchToDistance = PitchToDistanceCurve->GetFloatValue(cameraRotation.Pitch);
        const FVector actorLocation = GetViewTarget()->GetActorLocation();
        const FVector cameraDirection = cameraRotation.RotateVector(FVector::ForwardVector);
        const FVector desiredLocation = actorLocation - cameraDirection * pitchToDistance;

        FVector newLocation;
        FCollisionQueryParams queryParams(SCENE_QUERY_STAT(SpringArm), false, GetViewTarget());
        FHitResult result;
        GetWorld()->SweepSingleByChannel(result, actorLocation, desiredLocation, FQuat::Identity,
            cameraManager->LineOfSightProbeChannel,
            FCollisionShape::MakeSphere(cameraManager->LineOfSightProbeSize), queryParams);
        newLocation = result.bBlockingHit ? result.Location : desiredLocation;

        // Note that changing the location will override any change made in the spring arm (or other modifiers).
        InOutPOV.Location = newLocation;
    }

    // Apply FOV curve.
    if (bEnablePitchToFOV && IsValid(PitchToFOVCurve))
    {
        const float pitchToFov = PitchToFOVCurve->GetFloatValue(cameraRotation.Pitch);
        InOutPOV.FOV += pitchToFov;
    }

    return false;
}
