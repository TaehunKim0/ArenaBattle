// Fill out your copyright notice in the Description page of Project Settings.


#include "Sample/NetCullActor.h"

#include "GameFramework/GameNetworkManager.h"

// Sets default values
ANetCullActor::ANetCullActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    NetCullDistanceSquared = 50.f;
	bReplicates = true;
	
	CubeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CubeMesh"));
	RootComponent = CubeMesh;
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeStaticMesh(TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'"));

    if (CubeStaticMesh.Succeeded()) CubeMesh->SetStaticMesh(CubeStaticMesh.Object);

}

// Called when the game starts or when spawned
void ANetCullActor::BeginPlay()
{
	Super::BeginPlay();

	// UWorld *World = GetWorld();
	// // 폰을 가져옵니다.
	// APawn* Pawn = World->GetFirstPlayerController()->GetPawn();
	// if (!Pawn)
	// {
	// 	UE_LOG(LogTemp, Warning, TEXT("폰 없음"));
	// 	return;
	// }
	//
	// USkeletalMeshComponent* MeshComponent = Pawn->FindComponentByClass<USkeletalMeshComponent>();
	// if (!MeshComponent)
	// {
	// 	UE_LOG(LogTemp, Warning, TEXT("메시 컴포넌트 없음"));
	// 	return;
	// }
	//
	// FName SocketName = FName("Bip001");
	//
	// FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, false);
	// AttachToComponent(MeshComponent, AttachmentRules, SocketName);
}

// Called every frame
void ANetCullActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// auto RelevantCheck = IsNetRelevantFor(GetWorld()->GetFirstPlayerController(), GetWorld()->GetFirstPlayerController(), GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation());
	// GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, FString::Printf(TEXT("%d"), RelevantCheck));
	//
	// if(CheckNetRelevancy())
	// {
	// 	const UEnum* RelevancyEnumType = FindObject<UEnum>(nullptr, TEXT("/Script/ArenaBattle.ENetRelevancy"));
	// 	checkf(RelevancyEnumType, TEXT("ENetRelevancy 없음"));
	//
	// 	FString RelevancyMetaData = RelevancyEnumType->GetDisplayNameTextByValue(static_cast<int64>(EMyNetRelevancy)).ToString();
	// 	UE_LOG(LogTemp, Log, TEXT("Relevancy Type : %s "),*RelevancyMetaData); // int 형 말고 , Meta의 DisplayName 을 출력하자
	//
	// 	FRotator Rotator = this->GetActorRotation();
	// 	Rotator.Yaw += DeltaTime * 50.f;
	// 	SetActorRotation(Rotator);
	// 	return;
	// }
	//
	// if (GetNetMode() == NM_Client)
	// {
	// 	FRotator Rotator = this->GetActorRotation();
	// 	Rotator.Yaw += DeltaTime * 50.f;
	// 	SetActorRotation(Rotator);
	// }
	

	
}

bool ANetCullActor::IsNetRelevantFor(const AActor* RealViewer, const AActor* ViewTarget, const FVector& SrcLocation) const
{
	Super::IsNetRelevantFor(RealViewer, ViewTarget, SrcLocation)
	? GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, FString::Printf(TEXT("연관있음")))
	: GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, FString::Printf(TEXT("연관없음")));
	
	return Super::IsNetRelevantFor(RealViewer, ViewTarget, SrcLocation);
}

bool ANetCullActor::CheckNetRelevancy()
{
	auto ActorToCheck = this;
	
    if (!ActorToCheck)
    {
        UE_LOG(LogTemp, Warning, TEXT("ActorToCheck is null."));
        return true;
    }

    // 1. 액터가 bAlwaysRelevant인지 검사
    if (ActorToCheck->bAlwaysRelevant)
    {
        UE_LOG(LogTemp, Warning, TEXT("Actor is always relevant."));
    	EMyNetRelevancy = ENetRelevancy::AlwaysRelevant;
        return true;
    }

    // 2. 액터가 bNetUseOwnerRelevancy이고 Owner가 nullptr이 아닌지 검사
    if (ActorToCheck->bNetUseOwnerRelevancy && ActorToCheck->Owner)
    {
        UE_LOG(LogTemp, Warning, TEXT("Actor is relevant based on owner's relevancy."));
    	EMyNetRelevancy = ENetRelevancy::NetUseOwnerRelevancy;
        return true;
    }

    // 3. 액터가 bOnlyRelevantToOwner인지 검사
    if (ActorToCheck->bOnlyRelevantToOwner)
    {
        UE_LOG(LogTemp, Warning, TEXT("Actor is only relevant to its owner."));
    	EMyNetRelevancy = ENetRelevancy::OnlyRelevantToOwner;
        return true;
    }

    // 4. 액터가 다른 액터의 스켈레톤에 붙어있는지 검사
    if (ActorToCheck->RootComponent && ActorToCheck->RootComponent->GetAttachParent() && ActorToCheck->RootComponent->GetAttachParent()->GetOwner())
    {
        if (Cast<USkeletalMeshComponent>(ActorToCheck->RootComponent->GetAttachParent()) || (ActorToCheck->RootComponent->GetAttachParent()->GetOwner() == ActorToCheck->Owner))
        {
            UE_LOG(LogTemp, Warning, TEXT("Actor is attached to another actor's skeleton or attached to its owner."));
        	EMyNetRelevancy = ENetRelevancy::AttachedToSkeleton;
            return true;
        }
    }

    // 5. 액터가 숨겨져 있는지와 충돌이 비활성화되어 있는지 검사
    if (ActorToCheck->IsHidden() && (!ActorToCheck->RootComponent || !ActorToCheck->RootComponent->IsCollisionEnabled()))
    {
        UE_LOG(LogTemp, Warning, TEXT("Actor is hidden and has no collision enabled."));
    	EMyNetRelevancy = ENetRelevancy::IsHidden;
        return true;
    }

    // 6. 액터가 네트워크 거리 기반 연관성 검사를 사용하는지 검사하고, SrcLocation과의 거리를 검사
    if (!GetDefault<AGameNetworkManager>()->bUseDistanceBasedRelevancy || ActorToCheck->IsWithinNetRelevancyDistance(ActorToCheck->GetActorLocation()))
    {
        UE_LOG(LogTemp, Warning, TEXT("Actor is relevant based on distance : %f"), );
    	EMyNetRelevancy = ENetRelevancy::UseDistanceBasedRelevancy;
        return true;
    }
	
    // 액터가 위의 조건들을 모두 만족하지 않는 경우
    UE_LOG(LogTemp, Warning, TEXT("Actor is not relevant."));
	return false;
}
