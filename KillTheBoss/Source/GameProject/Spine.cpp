#include "GameProject.h"
#include "Spine.h"

#include "PlayerCharacter.h"

ASpine::ASpine()
{
	PrimaryActorTick.bCanEverTick = false;

	SpriteComponent = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("SpriteComponent"));
	Sprite = ConstructorHelpers::FObjectFinderOptional<UPaperSprite>(TEXT("PaperSprite'/Game/Sprite/Object/Sprine_Sprite.Sprine_Sprite'")).Get();
	SpriteComponent->SetSprite(Sprite);

	SpriteComponent->SetCollisionProfileName(TEXT("Enemy"));

	RootComponent = SpriteComponent;

	SpriteComponent->OnComponentBeginOverlap.AddDynamic(this, &ASpine::OverlapStart);
}

void ASpine::OverlapStart(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	{
		if (OtherActor->GetActorClass()->IsChildOf(APlayerCharacter::StaticClass()))
		{
			APlayerCharacter* Character = Cast<APlayerCharacter>(OtherActor);

			if (Character->GetActorLocation().X - RootComponent->GetComponentLocation().X >= 0)
			{
				Character->LaunchCharacter(FVector(25.0f, 0.0f, 5.0f) * 100, false, false);
			}
			else
			{
				Character->LaunchCharacter(FVector(-25.0f, 0.0f, 5.0f) * 100, false, false);
			}

			Character->StartInvincibleTimer();
		}
	}
}


