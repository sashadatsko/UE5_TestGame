// Fill out your copyright notice in the Description page of Project Settings.


#include "AmmoConsumable.h"
#include "DronePlayer.h"

// Sets default values
AAmmoConsumable::AAmmoConsumable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    RootComponent =
        CreateDefaultSubobject<USceneComponent>(TEXT("RootSceneComponent"));
    StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
    Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
    StaticMesh->SetupAttachment(RootComponent);
    Box->SetupAttachment(StaticMesh);

    static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeVisualAsset(TEXT("/Game/Assets/Meshes/SM_ChamferCube.SM_ChamferCube"));
    static ConstructorHelpers::FObjectFinder<UMaterialInterface> MaterialAsset(TEXT("/Game/Assets/Materials/FirstPersonProjectileMaterial.FirstPersonProjectileMaterial"));

    if (CubeVisualAsset.Succeeded())
    {
        StaticMesh->SetStaticMesh(CubeVisualAsset.Object);
        if (MaterialAsset.Succeeded())
           StaticMesh->SetMaterial(0, MaterialAsset.Object);
    }

    Box->SetWorldScale3D(FVector(2.0f, 2.0f, 2.0f));
}

// Called when the game starts or when spawned
void AAmmoConsumable::BeginPlay()
{
	Super::BeginPlay();
    Box->OnComponentBeginOverlap.AddDynamic(this, &AAmmoConsumable::OnOverlapBegin);
}

void AAmmoConsumable::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Hit!"));
    ADronePlayer* Player = (ADronePlayer*)GetWorld()->GetFirstPlayerController()->GetPawn();
    if (OtherActor == Player)
    {
        Player->AddAmmo();
        Destroy();
    }
}

// Called every frame
void AAmmoConsumable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    FVector NewLocation = GetActorLocation();
    FRotator NewRotation = GetActorRotation();
    float RunningTime = GetGameTimeSinceCreation();
    float DeltaHeight = (FMath::Sin(RunningTime + DeltaTime) - FMath::Sin(RunningTime));
    NewLocation.Z += DeltaHeight * 20.0f;
    float DeltaRotation = DeltaTime * 20.0f;
    NewRotation.Yaw += DeltaRotation;
    SetActorLocationAndRotation(NewLocation, NewRotation);
}

