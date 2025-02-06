// Fill out your copyright notice in the Description page of Project Settings.


#include "DronePlayer.h"

#include "Camera/CameraComponent.h"
#include "GameModeRestart.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/LocalPlayer.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

// Sets default values
ADronePlayer::ADronePlayer()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    AutoPossessPlayer = EAutoReceiveInput::Player0;

    RootComponent =
        CreateDefaultSubobject<USceneComponent>(TEXT("RootSceneComponent"));
    SpringArm =
        CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
    Camera =
        CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));

    Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
    GetCapsuleComponent()->SetupAttachment(RootComponent);

    SpringArm->SetupAttachment(GetCapsuleComponent());
    SpringArm->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 50.0f),
        FRotator(-60.0f, 0.0f, 0.0f));
    SpringArm->TargetArmLength = 100.f;
    SpringArm->bEnableCameraLag = true;
    SpringArm->CameraLagSpeed = 10.0f;

    SetActorRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));
    GetCapsuleComponent()->InitCapsuleSize(40, 40);
}

// Called when the game starts or when spawned
void ADronePlayer::BeginPlay()
{
	Super::BeginPlay();

    /* if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
    {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
        {
            Subsystem->AddMappingContext(0, 0);
        }
    }*/
	
}

// Called every frame
void ADronePlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    
    FRotator NewRotation = SpringArm->GetComponentRotation();
    NewRotation.Yaw +=
        cameraInput.X;
    SpringArm->SetWorldRotation(NewRotation);

    NewRotation = SpringArm->GetComponentRotation();
    NewRotation.Pitch =
        FMath::Clamp(NewRotation.Pitch + cameraInput.Y, -80.0f, -15.0f);
    SpringArm->SetWorldRotation(NewRotation);

    if (!movementInput.IsZero()) 
    {    
        movementInput = movementInput.GetSafeNormal() * 400.0f;
        FVector NewLocation = GetActorLocation();
        NewLocation += GetActorForwardVector() * movementInput.X * DeltaTime;
        NewLocation += GetActorRightVector() * movementInput.Y * DeltaTime;
        NewLocation += GetActorUpVector() * movementInput.Z * DeltaTime;
        SetActorLocation(NewLocation);
    }
}

void ADronePlayer::MoveForward(float AxisValue) 
{
    movementInput.X = FMath::Clamp<float>(AxisValue, -1.0f, 1.0f);
}

void ADronePlayer::MoveRight(float AxisValue) 
{
    movementInput.Y = FMath::Clamp<float>(AxisValue, -1.0f, 1.0f);
}

void ADronePlayer::MoveUpDown(float AxisValue) 
{
    movementInput.Z = FMath::Clamp<float>(AxisValue, -1.0f, 1.0f);
}

void ADronePlayer::PitchCamera(float AxisValue) 
{
    cameraInput.Y = AxisValue;
}

void ADronePlayer::YawCamera(float AxisValue) 
{
    cameraInput.X = AxisValue;
}

void ADronePlayer::AddAmmo()
{
    if (ammo > 20)
        ammo = 20;
    else
        ammo += 5;
}

void ADronePlayer::AddHealth()
{
    if (health > 100)
        health = 100;
    else
        health += 20;
}

void ADronePlayer::TakeDamage()
{
    if (health > 20)
    {
        health -= 20;
    }
    else
    {
        ((AGameModeRestart*)GetWorld()->GetAuthGameMode())->RestartGame();
    }
}

// Called to bind functionality to input
void ADronePlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

    InputComponent->BindAxis("MoveX", this, &ADronePlayer::MoveForward);
    InputComponent->BindAxis("MoveY", this, &ADronePlayer::MoveRight);
    InputComponent->BindAxis("MoveZ", this, &ADronePlayer::MoveUpDown);
    InputComponent->BindAxis("CameraPitch", this, &ADronePlayer::PitchCamera);
    InputComponent->BindAxis("CameraYaw", this, &ADronePlayer::YawCamera);
    InputComponent->BindAxis("CameraYaw", this, &ADronePlayer::YawCamera);
}