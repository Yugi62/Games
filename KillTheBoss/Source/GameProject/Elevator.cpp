// Fill out your copyright notice in the Description page of Project Settings.

#include "GameProject.h"
#include "Elevator.h"

#include "PaperSpriteComponent.h"


AElevator::AElevator()
{
	PrimaryActorTick.bCanEverTick = true;

	SpriteComponent = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("SpriteComponent"));
	
	Sprite = ConstructorHelpers::FObjectFinderOptional<UPaperSprite>(TEXT("PaperSprite'/Game/Sprite/Object/Elevator_Sprite.Elevator_Sprite'")).Get();
	SpriteComponent->SetSprite(Sprite);

	RootComponent = SpriteComponent;

	MoveSpeed = 0.0f;
	MoveTime = 0.0f;

	ElevatorAxis = false;
}

void AElevator::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(TimerHandle, this, &AElevator::MoveElevator, MoveTime, true);
}

void AElevator::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	FVector NewLocation = GetActorLocation();

	if(!ElevatorAxis)
	NewLocation.Z += MoveSpeed;

	else if (ElevatorAxis)
	NewLocation.X += MoveSpeed;

	SetActorLocation(NewLocation);
}

void AElevator::MoveElevator()
{
	MoveSpeed *= -1;
}

