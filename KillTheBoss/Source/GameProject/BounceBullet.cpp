#include "GameProject.h"
#include "BounceBullet.h"

ABounceBullet::ABounceBullet()
{
	PrimaryActorTick.bCanEverTick = false;

	ProjectileCollision->SetCollisionProfileName(TEXT("BounceBullet"));
	ProjectileCollision->SetSphereRadius(14.0f);

	Sprite = ConstructorHelpers::FObjectFinderOptional<UPaperSprite>(TEXT("PaperSprite'/Game/Sprite/Projectile/SP_BounceBullet.SP_BounceBullet'")).Get();
	SpriteComponent->SetSprite(Sprite);

	ProjectileCollision->SetEnableGravity(true);

	ProjectileMovementComponent->InitialSpeed = 500.0f;
	ProjectileMovementComponent->MaxSpeed = 500.0f;
	ProjectileMovementComponent->bShouldBounce = true;
	ProjectileMovementComponent->Bounciness = 1.0f;
	ProjectileMovementComponent->ProjectileGravityScale = 0.0f;

	ProjectileMovementComponent->Velocity = FVector(-1.0f, 0.0f, 0.0f) * ProjectileMovementComponent->InitialSpeed;

	InitialLifeSpan = 6.0f;
}

void ABounceBullet::BeginPlay()
{
	Super::BeginPlay();
	
}
