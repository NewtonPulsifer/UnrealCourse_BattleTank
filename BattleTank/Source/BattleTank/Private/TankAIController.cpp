// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleTank.h"
#include "TankAIController.h"
#include "../Public/TankAIController.h"

void ATankAIController::BeginPlay() {
	Super::BeginPlay();

	auto ControlledTank = GetControlledTank();
	if (!ControlledTank) {
		UE_LOG(LogTemp, Warning, TEXT("No Pawn in ATankAIController. Will not look for player."));
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("ATankAIController Controlling: %s"), *ControlledTank->GetName());

		auto PlayerTank = GetPlayerTank();
		if (!PlayerTank) {
			UE_LOG(LogTemp, Warning, TEXT("%s could not find the player tank."), *ControlledTank->GetName(), *PlayerTank->GetName());
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("%s found %s as player tank."), *ControlledTank->GetName(), *PlayerTank->GetName());
		}
	}
}

void ATankAIController::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	if (GetControlledTank()) {
		GetControlledTank()->AimAt(GetPlayerTank()->GetActorLocation());
	}
}

ATank* ATankAIController::GetControlledTank() const {
	return Cast<ATank>(GetPawn());
}

ATank * ATankAIController::GetPlayerTank() const
{
	return Cast<ATank>(GetWorld()->GetFirstPlayerController()->GetPawn());
}
