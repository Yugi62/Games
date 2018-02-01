#include "GameProject.h"
#include "FastBullet.h"

AFastBullet::AFastBullet()
{
	PrimaryActorTick.bCanEverTick = false;

	SpriteComponent->DetachFromParent();
	RootComponent = SpriteComponent;
	ProjectileCollision->DestroyComponent();

	SpriteComponent->SetCollisionProfileName(TEXT("Projectile"));

	Sprite = ConstructorHelpers::FObjectFinderOptional<UPaperSprite>(TEXT("PaperSprite'/Game/Sprite/Projectile/SP_FastBullet.SP_FastBullet'")).Get();
	SpriteComponent->SetSprite(Sprite);

	ProjectileCollision->SetEnableGravity(false);

	ProjectileMovementComponent->InitialSpeed = 1000.0f;
	ProjectileMovementComponent->MaxSpeed = 1000.0f;
	ProjectileMovementComponent->bShouldBounce = true;
	ProjectileMovementComponent->Bounciness = 1.0f;
	ProjectileMovementComponent->ProjectileGravityScale = 0.0f;

	ProjectileMovementComponent->Velocity = FVector(-1.0f, 0.0f, 0.0f) * ProjectileMovementComponent->InitialSpeed;

	InitialLifeSpan = 3.0f;

	SpriteComponent->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OverlapStart);
}

void AFastBullet::BeginPlay()
{
	Super::BeginPlay();
}
