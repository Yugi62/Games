#pragma once

#include "GameFramework/Character.h"
#include "PaperCharacter.h"
#include "Item.h"
#include "PaperFlipbookComponent.h"
#include "PaperSpriteComponent.h"
#include "PaperFlipbook.h"
#include "PaperSprite.h"
#include "PlayerCharacter.generated.h"

#define MAX_WEAPON_ITEM 5
#define MAX_ARMOR_ITEM 5

UCLASS()
class GAMEPROJECT_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:

	APlayerCharacter();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	// 카메라 손잡이
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	USpringArmComponent* CameraBoom;

	// 카메라
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* Camera;

	// 캐릭터 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sprite")
	class UPaperFlipbookComponent* CharacterComponent;

	// 무기 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sprite")
	UStaticMeshComponent* WeaponComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sprite")
	UStaticMeshComponent* HelmetComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sprite")
	UStaticMeshComponent* SuitComponent;

	// 대기 애니메이션
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation")
	class UPaperFlipbook* IdleAnimation;

	// 이동 애니메이션
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation")
	class UPaperFlipbook* RunningAnimation;

	// 점프 애니메이션
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation")
	class UPaperFlipbook* JumpingAnimation;

	// 공격 애니메이션
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation")
	class UPaperFlipbook* AttackingAnimation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation")
	class UPaperFlipbook* DeathAnimation;

	// 인벤토리
	UPROPERTY(EditAnywhere, Category = "Inventory")
	TArray<AItem*> WeaponInventory;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	TArray<AItem*> ArmorInventory;

	UPROPERTY(VisibleAnywhere, Category = "Inventory")
	TArray<int32> WeaponDurability;

	UPROPERTY(VisibleAnywhere, Category = "Inventory")
	TArray<int32> ArmorDurability;

	// 겹쳤을 시 True
	UFUNCTION()
	void OverlapStart(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// 겹치지 않을 시 False
	UFUNCTION()
	void OverlapStop(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


	// 플레이어 움직임 함수
	void Move(float Value);
	void Attack();
	void JumpStart();
	void JumpStop();

	// 캐릭터 애니메이션 함수
	void UpdateAnimation();
	void UpdateCharacter();

	// 점프 계산 변수 
	float DELTATIME;
	int32 JumpValue;

	// 무기 스프라이트 받는 변수
	class UPaperSprite* WeaponSprite;

	// 공격 타이머 계산
	void AttackTimer();
	FTimerHandle CountdownHandle;
	int32 CountTemp;
	bool SetAttackAnimation;

	///// 임시 ///////
	void GetWeapon();

	bool IsOverlapping;
	bool IsHoldingWeapon;
	float HoldKeyTime = 0.0f;

	float WeaponRotation[4] = { 0.0f, 0.0f, -50.0f, -75.0f };

	// 겹쳐지고 있는 액터
	UPROPERTY()
	AItem* OverlapActor;

	// 실제 인벤토리
	void OpenOption();

	FTimerHandle InvincibleTimer;
	void StartInvincibleTimer();
	void SetCharacterInvincibleTimer();
	int32 InvincibleTimeCount = 0;

	bool CameraStopOverlap;
	FVector PlayerLocation;

	void ShakeCamera();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sprite")
	class UPaperSpriteComponent* BackGroundComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sprite")
	class UPaperSprite* BackGround;

	///////////////////////////////////////////////////////////////

	UPROPERTY(EditAnywhere, Category = "Music")
	UAudioComponent* AudioComponent;

	UPROPERTY(EditAnywhere, Category = "Music")
	USoundCue* Jump_Cue;

	UPROPERTY(EditAnywhere, Category = "Music")
	USoundCue* Equip_Sword_Cue;

	UPROPERTY(EditAnywhere, Category = "Music")
	USoundCue* Equip_Armor_Cue;

	UPROPERTY(EditAnywhere, Category = "Music")
	USoundCue* Damaged_Cue;

	UPROPERTY(EditAnywhere, Category = "Music")
	USoundCue* Full_Cue;

	FTimerHandle DeathTimer;
	void Death();

	bool IsDeath;
};