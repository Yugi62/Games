// Fill out your copyright notice in the Description page of Project Settings.

#include "GameProject.h"
#include "Ghost.h"


AGhost::AGhost()
{
	PrimaryActorTick.bCanEverTick = true;

	SpriteComponent = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("SpriteComponent"));
	RootComponent = SpriteComponent;

	Sprite = ConstructorHelpers::FObjectFinderOptional<UPaperSprite>(TEXT("PaperSprite'/Game/Sprite/Character/SP_Jumping_1.SP_Jumping_1'")).Get();
	SpriteComponent->SetSprite(Sprite);

	UMaterialInterface* Material = ConstructorHelpers::FObjectFinderOptional<UMaterialInterface>(TEXT("MaterialInstanceConstant'/Paper2D/TranslucentUnlitSpriteMaterial.TranslucentUnlitSpriteMaterial'")).Get();
	SpriteComponent->SetMaterial(0, Material);
	SpriteComponent->SetSpriteColor(FLinearColor(1.0, 1.0, 1.0, 0.6));

	SpriteComponent->SetCollisionProfileName(TEXT("Sprite"));

	InitialLifeSpan = 6.0f;
}

void AGhost::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	FVector NewLocation = this->GetActorLocation();
	NewLocation.Z += 1.0f;

	this->SetActorLocation(NewLocation);
}

