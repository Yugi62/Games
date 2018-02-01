#include "GameProject.h"
#include "SmallBullet.h"

ASmallBullet::ASmallBullet()
{
	PrimaryActorTick.bCanEverTick = false;

	ProjectileCollision->SetSphereRadius(7.0f);

	Sprite = ConstructorHelpers::FObjectFinderOptional<UPaperSprite>(TEXT("PaperSprite'/Game/Sprite/Projectile/SP_SmallBullet.SP_SmallBullet'")).Get();
	SpriteComponent->SetSprite(Sprite);

	ProjectileCollision->SetEnableGravity(false);

	ProjectileMovementComponent->InitialSpeed = 400.0f;
	ProjectileMovementComponent->MaxSpeed = 500.0f;
	ProjectileMovementComponent->bShouldBounce = true;
	ProjectileMovementComponent->Bounciness = 1.0f;
	ProjectileMovementComponent->ProjectileGravityScale = 0.0f;

	ProjectileMovementComponent->Velocity = FVector(-1.0f, 0.0f, 0.0f) * ProjectileMovementComponent->InitialSpeed;

	InitialLifeSpan = 4.0f;
}

void ASmallBullet::BeginPlay()
{
	Super::BeginPlay();
	
}
