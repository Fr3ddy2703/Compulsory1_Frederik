// Fill out your copyright notice in the Description page of Project Settings.


#include "Ball.h"
#include "Components/SphereComponent.h"
#include "Components/InputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include <UObject/ConstructorHelpers.h>
#include "Containers/Array.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"

// Defines the default values
ABall::ABall()
{
 	// The pawn will call Tick() every frame  
	PrimaryActorTick.bCanEverTick = true;

	Spheres.Init(NULL, 9);
	Spheres[0] = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh1"));
	Spheres[1] = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh2"));
	Spheres[2] = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh3"));
	Spheres[3] = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh4"));
	Spheres[4] = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh5"));
	Spheres[5] = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh6"));
	Spheres[6] = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh7"));
	Spheres[7] = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh8"));
	Spheres[8] = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh9"));

	// Defines what the RootComponent is
	RootComponent = Spheres[4];
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		MeshComponentAsset(TEXT("StaticMesh'/Game/StarterContent/Props/MaterialSphere.MaterialSphere'"));
	for (int i{}; i < 9; i++)
	{
		if (i != 4)
		{
			Spheres[i]->SetupAttachment(GetRootComponent());
		}
		Spheres[i]->SetStaticMesh(MeshComponentAsset.Object);
	}

	Red = CreateDefaultSubobject<UMaterial>(TEXT("RedMaterial"));
	Blue = CreateDefaultSubobject<UMaterial>(TEXT("BlueMaterial"));
	White = CreateDefaultSubobject<UMaterial>(TEXT("WhiteMaterial"));


	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->SetRelativeRotation(FRotator(-45.f, 0.f, 0.f));
	SpringArm->TargetArmLength = 500.f;

	// Attach the Camera to SpringArm
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);


	// Locations for the objects 
	Spheres[0]->SetRelativeLocation(FVector(200.f, -200.f, 0.f));
	Spheres[1]->SetRelativeLocation(FVector(200.f, 0.f, 0.f));
	Spheres[2]->SetRelativeLocation(FVector(200.f, 200.f, 0.f));
	Spheres[3]->SetRelativeLocation(FVector(0.f, -200.f, 0.f));
	Spheres[4]->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
	Spheres[5]->SetRelativeLocation(FVector(0.f, 200.f, 0.f));
	Spheres[6]->SetRelativeLocation(FVector(-200.f, -200.f, 0.f));
	Spheres[7]->SetRelativeLocation(FVector(-200.f, 0.f, 0.f));
	Spheres[8]->SetRelativeLocation(FVector(-200.f, 200.f, 0.f));

	TurnCounter = 0;
	GameOver = false;
	SphereStatus.Init(NULL, 9);
	WinStatus.Init(NULL, 9);
	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

// Called when the game starts or when spawned
void ABall::BeginPlay()
{
	Super::BeginPlay();
	for (int i{}; i < 9; i++)
	{
		Spheres[i]->SetMaterial(0, White);
	}
	GameOver = false;
}

// Called every frame
void ABall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Binds functionality to inputs
void ABall::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("TopLeft", IE_Pressed, this, &ABall::OnePressed);
	PlayerInputComponent->BindAction("TopMiddle", IE_Pressed, this, &ABall::TwoPressed);
	PlayerInputComponent->BindAction("TopRight", IE_Pressed, this, &ABall::ThreePressed);
	PlayerInputComponent->BindAction("MiddleLeft", IE_Pressed, this, &ABall::FourPressed);
	PlayerInputComponent->BindAction("Middle", IE_Pressed, this, &ABall::FivePressed);
	PlayerInputComponent->BindAction("MiddleRight", IE_Pressed, this, &ABall::SixPressed);
	PlayerInputComponent->BindAction("BottomLeft", IE_Pressed, this, &ABall::SevenPressed);
	PlayerInputComponent->BindAction("BottomMiddle", IE_Pressed, this, &ABall::EightPressed);
	PlayerInputComponent->BindAction("BottomRight", IE_Pressed, this, &ABall::NinePressed);
}


// PlayerTurns
void ABall::TurnController(int SphereIndex)
{
	if(SphereStatus[SphereIndex] == true)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("This ball is taken"));
		return;
	}


	if (TurnCounter % 2 == 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Red's turn"));
		Spheres[SphereIndex]->SetMaterial(0, Blue);
		WinStatus[SphereIndex] = 1;
	}
	else if (TurnCounter % 2 == 1)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("Blue's turn"));
		Spheres[SphereIndex]->SetMaterial(0, Red);
		WinStatus[SphereIndex] = -1;
	}
	SphereStatus[SphereIndex] = true;
	TurnCounter++;
	WinCondition();

}

void ABall::WinCondition()
{
	int Row1 = WinStatus[0] + WinStatus[1] + WinStatus[2];
	int Row2 = WinStatus[3] + WinStatus[4] + WinStatus[5];
	int Row3 = WinStatus[6] + WinStatus[7] + WinStatus[8];
	int Row4 = WinStatus[0] + WinStatus[4] + WinStatus[8];
	int Row5 = WinStatus[2] + WinStatus[4] + WinStatus[6];
	int Row6 = WinStatus[0] + WinStatus[3] + WinStatus[6];
	int Row7 = WinStatus[1] + WinStatus[4] + WinStatus[7];
	int Row8 = WinStatus[2] + WinStatus[5] + WinStatus[8];

	if(Row1 == 3 || Row2 == 3 || Row3 == 3 || Row4 == 3 || Row5 == 3 || Row6 == 3 || Row7 == 3 || Row8 == 3)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("Blue win"));
		GameOver = true;
	}
	else if(Row1 == -3 || Row2 == -3 || Row3 == -3 || Row4 == -3 || Row5 == -3 || Row6 == -3 || Row7 == -3 || Row8 == -3)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("Red win"));
		GameOver = true;	
	}
}


void ABall::OnePressed()
{
	TurnController(0);
}

void ABall::TwoPressed()
{
	TurnController(1);
}

void ABall::ThreePressed()
{
	TurnController(2);
}

void ABall::FourPressed()
{
	TurnController(3);
}

void ABall::FivePressed()
{
	TurnController(4);
}

void ABall::SixPressed()
{
	TurnController(5);
}

void ABall::SevenPressed()
{
	TurnController(6);
}

void ABall::EightPressed()
{
	TurnController(7);
}

void ABall::NinePressed()
{
	TurnController(8);
}
