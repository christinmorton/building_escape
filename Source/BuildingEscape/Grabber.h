// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
	float reach = 150.f;

	// FVector LineTraceEnd;

	UPhysicsHandleComponent* PhysicsHandle = nullptr;

	UInputComponent* inputComponent = nullptr;

	FHitResult possessedItem;

	void Grab();
	void Release();
	void FindPhysicsHandle();
	void InitInputComponent();

	//Return the first actor within reach of the physic body.
	FHitResult GetFirstPhysicsBodyInReach() const;
};
