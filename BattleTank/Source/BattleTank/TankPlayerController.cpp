// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleTank.h"
#include "TankPlayerController.h"

void ATankPlayerController::BeginPlay() {
	Super::BeginPlay();

	auto ControlledTank = GetControlledTank();
	if (!ControlledTank) {
		UE_LOG(LogTemp, Warning, TEXT("No Pawn in ATankPlayerController"));
	} else {
		UE_LOG(LogTemp, Warning, TEXT("ATankPlayerController Controlling: %s"), *ControlledTank->GetName());
	}
}

void ATankPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AimTowardsCrosshair();
}

ATank* ATankPlayerController::GetControlledTank() const {
	return Cast<ATank>(GetPawn());
}

void ATankPlayerController::AimTowardsCrosshair()
{
	if (!GetControlledTank()) { return; }

	FVector HitLocation;
	if (GetSightRayHitLocation(HitLocation)) {
		GetControlledTank()->AimAt(HitLocation);
	}
}

bool ATankPlayerController::GetSightRayHitLocation(FVector &HitLocation) const
{
	int32 ViewportSizeX, ViewportSizeY;
	GetViewportSize(ViewportSizeX, ViewportSizeY);
	FVector2D ScreenLocation = FVector2D(ViewportSizeX * CrosshairXLocation, ViewportSizeY * CrosshairYLocation);

	FVector LookDirection;
	if (GetLookDirection(ScreenLocation, LookDirection)) {
		if (GetLookVectorHitLocation(LookDirection, HitLocation)) {
			return true;
		}
	}

	return false;
}

bool ATankPlayerController::GetLookDirection(FVector2D &ScreenLocation, FVector &LookDirection) const
{
	FVector CameraWorldLocation;
	return DeprojectScreenPositionToWorld(ScreenLocation.X, ScreenLocation.Y, CameraWorldLocation, LookDirection);
}

bool ATankPlayerController::GetLookVectorHitLocation(FVector LookDirection, FVector &HitLocation) const
{
	FVector CameraLocation = PlayerCameraManager->GetCameraLocation();
	FHitResult HitResult;
	if (GetWorld()->LineTraceSingleByChannel(HitResult, CameraLocation, CameraLocation + LookDirection * LineTraceRange, ECollisionChannel::ECC_Visibility)) {
		HitLocation = HitResult.Location;
		return true;
	}
	HitLocation = FVector(0);
	return false;
}
