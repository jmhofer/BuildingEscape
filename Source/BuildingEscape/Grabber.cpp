// (c) 2018 Joachim Hofer
#include "Grabber.h"
#include "Engine/World.h"
#include "Public/DrawDebugHelpers.h"

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

	UE_LOG(LogTemp, Warning, TEXT("Grabber reporting for duty!"))
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FVector ViewPointLocation;
	FRotator ViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT ViewPointLocation, OUT ViewPointRotation);

	FVector LineTraceEnd = ViewPointLocation + ViewPointRotation.Vector() * 100.f;

	DrawDebugLine(GetWorld(), ViewPointLocation, LineTraceEnd, FColor::Red, false, -1.f, 0, 7.f);

	FHitResult HitResult;
	FCollisionQueryParams IgnorePawn(FName(TEXT("")), false, GetOwner());

	GetWorld()->LineTraceSingleByObjectType(OUT HitResult, ViewPointLocation, LineTraceEnd, 
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		IgnorePawn
	);

	if (HitResult.IsValidBlockingHit()) {
		FString NameOfHitActor = HitResult.GetActor()->GetName();
		UE_LOG(LogTemp, Warning, TEXT("We hit: %s"), *NameOfHitActor)
	}
}
