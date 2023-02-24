#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "DreamCar.generated.h"

UCLASS()
class GAME_API ADreamCar : public APawn
{
	GENERATED_BODY()

public:
	ADreamCar();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	FVector GetAirResistance();
	FVector GetRollingResistance();
	void UpdateLocation(float Value);
	void UpdateRotation(float Value);

private:
	void MoveForward(float Value);
	void MoveRight(float Value);

	UFUNCTION(Reliable, Server, WithValidation)
	void Server_MoveForward(float Value);

	UFUNCTION(Reliable, Server, WithValidation)
	void Server_MoveRight(float Value);

private:
	UPROPERTY(EditAnywhere)
		float Mass = 1000;

	UPROPERTY(EditAnywhere)
		float MaxForce = 10000;

	UPROPERTY(EditAnywhere)
		float TurningRadius = 10;

	UPROPERTY(EditAnywhere)
		float MaxDegreePerSecond = 90;
	
	UPROPERTY(EditAnywhere)
		float DragCoefficient = 16.f;
	// 왜 16?
	//AirResistance = (invdir *) Speed ^ 2 * DragCoefficient
	//-> DragCoefficient = AirResistance / Speed ^ 2

	UPROPERTY(EditAnywhere)
		float RollingCoefficient = 0.015f;

private:
	FVector Velocity;

	float Throttle;
	float Steering;

	UPROPERTY(Replicated) // 패킷을 타는 변수
		FVector ReplicatedLocation;
	
	UPROPERTY(Replicated)
		FRotator ReplicatedRotation;
};
