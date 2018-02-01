#include "GameProject.h"
#include "IronHelmet.h"

AIronHelmet::AIronHelmet()
{
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = EmptyComponent;
	CapsuleComponent->SetupAttachment(EmptyComponent);

	CapsuleComponent->SetCapsuleRadius(22.0f);
	CapsuleComponent->SetCapsuleHalfHeight(22.0f);
	CapsuleComponent->SetCollisionProfileName("Armor");

	Sprite = ConstructorHelpers::FObjectFinderOptional<UPaperSprite>(TEXT("PaperSprite'/Game/Item/Sprite/IronHelmet_H_Sprite.IronHelmet_H_Sprite'")).Get();
	HighlightedSprite = ConstructorHelpers::FObjectFinderOptional<UPaperSprite>(TEXT("PaperSprite'/Game/Item/Sprite/IronHelmet_S_Sprite.IronHelmet_S_Sprite'")).Get();

	SpriteComponent->SetSprite(Sprite);
	SpriteComponent->SetupAttachment(CapsuleComponent);


	static ConstructorHelpers::FObjectFinder<UTexture2D> TextureTemp(TEXT("Texture2D'/Game/Item/IronHelmet_H.IronHelmet_H'"));
	Texture = TextureTemp.Object;

	Durability = 2;
}

void AIronHelmet::BeginPlay()
{
	Super::BeginPlay();
	
}

void AIronHelmet::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

