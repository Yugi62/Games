#include "GameProject.h"
#include "Slime.h"

#include "PlayerCharacter.h"


ASlime::ASlime()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = GetCapsuleComponent();

	GetCapsuleComponent()->SetCapsuleRadius(28.0f);
	GetCapsuleComponent()->SetCapsuleHalfHeight(28.0f);

	Sight->DestroyComponent();

	MovingAnimation = ConstructorHelpers::FObjectFinderOptional<UPaperFlipbook>(TEXT("PaperFlipbook'/Game/Sprite/Enemy/Flipbook/Slime_Move.Slime_Move'")).Get();
	FlipbookComponent->SetFlipbook(MovingAnimation);

	DELTATIME = 0.0f;
	TurnTime = 3.0f;
	Speed = 5.0f;
	Direction = -1.0f;
	temp = false;
}

void ASlime::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	DELTATIME += DeltaTime;

	AddMovementInput(FVector(1.0f * Direction, 0.0f, 0.0f), Speed);

	if (DELTATIME >= TurnTime)
	{
		DELTATIME = 0.0f;
		Direction *= -1.0f;

		if (temp == false)
		{
			RootComponent->SetRelativeRotation(FRotator(0.0f, 180.0f, 0.0f));
			temp = true;
		}
		else if (temp == true)
		{
			RootComponent->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
			temp = false;
		}
	}
}

