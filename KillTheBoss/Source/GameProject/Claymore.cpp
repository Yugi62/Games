// Fill out your copyright notice in the Description page of Project Settings.

#include "GameProject.h"
#include "Claymore.h"
#include "PaperSpriteComponent.h"
#include "PaperSprite.h"

AClaymore::AClaymore()
{
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = EmptyComponent;
	CapsuleComponent->SetupAttachment(EmptyComponent);

	CapsuleComponent->SetCapsuleRadius(8.0f);
	CapsuleComponent->SetCapsuleHalfHeight(30.0f);
	CapsuleComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 27.5f));

	Sprite = ConstructorHelpers::FObjectFinderOptional<UPaperSprite>(TEXT("PaperSprite'/Game/Item/Sprite/Claymore_H_Sprite.Claymore_H_Sprite'")).Get();
	HighlightedSprite = ConstructorHelpers::FObjectFinderOptional<UPaperSprite>(TEXT("PaperSprite'/Game/Item/Sprite/Claymore_S_Sprite.Claymore_S_Sprite'")).Get();

	SpriteComponent->SetSprite(Sprite);

	SpriteComponent->SetupAttachment(CapsuleComponent);


	static ConstructorHelpers::FObjectFinder<UTexture2D> TextureTemp(TEXT("Texture2D'/Game/Item/Claymore_H.Claymore_H'"));
	Texture = TextureTemp.Object;


	Durability = 5;
	Damage = 5.0f;
}