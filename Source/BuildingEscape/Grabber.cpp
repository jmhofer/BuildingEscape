// (c) 2018 Joachim Hofer
#include "Grabber.h"
#include "Engine/World.h"
#include "Public/DrawDebugHelpers.h"
#include "Components/PrimitiveComponent.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	InitializePhysicsHandle();
	BindInputs();
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (PhysicsHandle->GetGrabbedComponent()) {
		PhysicsHandle->SetTargetLocation(GetLineTraceEnd());
	}
}

void UGrabber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("Grabbed!"))

	auto HitResult = GetFirstPhysicsBodyInReach();
	if (HitResult.IsValidBlockingHit()) {
		auto ComponentToGrab = HitResult.GetComponent();
		PhysicsHandle->GrabComponent(ComponentToGrab, NAME_None, ComponentToGrab->GetOwner()->GetActorLocation(), true);
	}
}

void UGrabber::Release()
{
	UE_LOG(LogTemp, Warning, TEXT("Released!"))

	if (PhysicsHandle->GetGrabbedComponent()) {
		PhysicsHandle->ReleaseComponent();
	}
}

const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	FVector LineTraceEnd = GetLineTraceEnd();
	FHitResult HitResult;
	FCollisionQueryParams IgnorePawn(FName(TEXT("")), false, GetOwner());

	FVector ViewPointLocation;
	FRotator ViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT ViewPointLocation, OUT ViewPointRotation);

	GetWorld()->LineTraceSingleByObjectType(OUT HitResult, ViewPointLocation, LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		IgnorePawn
	);

	return HitResult;
}

const FVector UGrabber::GetLineTraceEnd() 
{
	FVector ViewPointLocation;
	FRotator ViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT ViewPointLocation, OUT ViewPointRotation);

	return ViewPointLocation + ViewPointRotation.Vector() * 100.f;
}

void UGrabber::InitializePhysicsHandle()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle) {
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Did not find physics handle for %s"), *GetOwner()->GetName())
	}
}

void UGrabber::BindInputs()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent) {
		InputComponent->BindAction(FName("Grab"), EInputEvent::IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction(FName("Grab"), EInputEvent::IE_Released, this, &UGrabber::Release);
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Did not find input component for %s"), *GetOwner()->GetName())
	}
}
