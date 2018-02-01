// Fill out your copyright notice in the Description page of Project Settings.

#include "GameProject.h"
#include "BossCameraShake.h"

UBossCameraShake::UBossCameraShake()
{
	CameraShake = CreateDefaultSubobject<UCameraShake>(TEXT("CameraShake"));

	CameraShake->OscillationDuration = 5.0f;
	CameraShake->OscillationBlendInTime = 0.1f;
	CameraShake->OscillationBlendOutTime = 0.2f;

	CameraShake->RotOscillation.Pitch.Amplitude = 0.5;
	CameraShake->RotOscillation.Pitch.Frequency = 40.0f;

	CameraShake->RotOscillation.Yaw.Amplitude = 1.0f;
	CameraShake->RotOscillation.Yaw.Frequency = 50.0f;
}


