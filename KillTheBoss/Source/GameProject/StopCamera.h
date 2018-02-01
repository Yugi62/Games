// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "StopCamera.generated.h"

UCLASS()
class GAMEPROJECT_API AStopCamera : public AActor
{
	GENERATED_BODY()
	
public:	

	AStopCamera();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Collision")
	UBoxComponent* Layer;

	
	
};
