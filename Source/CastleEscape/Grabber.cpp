// Fill out your copyright notice in the Description page of Project Settings.


#include "Grabber.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/PlayerController.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Components/InputComponent.h"

#define OUT
// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	PlayerController = GetWorld()->GetFirstPlayerController();
	PhysicsHandleCheck();
	SetupInputComponent();
}

void UGrabber::PhysicsHandleCheck() {
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

	if (!PhysicsHandle) {
		UE_LOG(LogTemp, Error, TEXT("No Physics Handle Attached."));
	}
}

void UGrabber::SetupInputComponent() {
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent) {
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::GrabKeyPressed);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::GrabKeyReleased);
	}
}

void UGrabber::GrabKeyPressed() {
	FHitResult Hit = GetFirstPhysicsBodyInReach();
	UPrimitiveComponent* ActorThatWasHit = Hit.GetComponent();

	if (ActorThatWasHit) {
		PhysicsHandle->GrabComponentAtLocation(ActorThatWasHit, FName(TEXT("")), GetPlayerReach());
	}
}

void UGrabber::GrabKeyReleased() {
	if (PhysicsHandle->GrabbedComponent) {
		PhysicsHandle->ReleaseComponent();
	}
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	LineTrace();
	if (PhysicsHandle->GrabbedComponent) {
		PhysicsHandle->SetTargetLocation(GetPlayerReach());
	}
}

void UGrabber::LineTrace() {
	FVector PlayerViewpointLocation;
	FRotator PlayerViewpointRotation;

	PlayerController->GetPlayerViewPoint(OUT PlayerViewpointLocation, OUT PlayerViewpointRotation);

	LineTraceEnd = PlayerViewpointLocation + Reach * PlayerViewpointRotation.Vector();
	
	DrawDebugLine(
		GetWorld(),
		OUT PlayerViewpointLocation,
		OUT LineTraceEnd,
		FColor(255, 0, 0),
		false,
		0.f,
		0,
		3.f
	);
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach() {
	FHitResult Result;

	FCollisionQueryParams QueryParams = FCollisionQueryParams(FName(TEXT("")), false, GetOwner());

	GetWorld()->LineTraceSingleByObjectType(
		OUT Result,
		GetPlayerViewpointLocation(),
		GetPlayerReach(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		QueryParams
	);
	return Result;
}

FVector UGrabber::GetPlayerViewpointLocation() {
	FVector PlayerViewpointLocation;
	FRotator PlayerViewpointRotation;

	PlayerController->GetPlayerViewPoint(OUT PlayerViewpointLocation, OUT PlayerViewpointRotation);

	return PlayerViewpointLocation;
}

FVector UGrabber::GetPlayerReach() {
	FVector PlayerViewpointLocation;
	FRotator PlayerViewpointRotation;

	PlayerController->GetPlayerViewPoint(OUT PlayerViewpointLocation, OUT PlayerViewpointRotation);

	LineTraceEnd = PlayerViewpointLocation + Reach * PlayerViewpointRotation.Vector();
	return LineTraceEnd;
}