// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "SpawnItem.generated.h"

UCLASS()
class GAMEPROJECT_API ASpawnItem : public AActor
{
	GENERATED_BODY()
	
public:	
	ASpawnItem();
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Collision")
	UBoxComponent* BoxComponent;

	UPROPERTY(EditAnywhere, Category = "Value")
	bool SpawnGoldenHelmet;

	UPROPERTY(EditAnywhere, Category = "Value")
	bool SpawnGoldenSuit;

	UPROPERTY(EditAnywhere, Category = "Value")
	bool SpawnIronHelmet;

	UPROPERTY(EditAnywhere, Category = "Value")
	bool SpawnIronSuit;

	UPROPERTY(EditAnywhere, Category = "Value")
	bool SpawnClaymore;

	UPROPERTY(EditAnywhere, Category = "Value")
	bool SpawnExcaliber;
};
