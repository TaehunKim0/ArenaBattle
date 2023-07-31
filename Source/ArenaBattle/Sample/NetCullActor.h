// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NetCullActor.generated.h"

UENUM()
enum ENetRelevancy
{
	AlwaysRelevant UMETA(DisplayName = "bAlwaysRelevant"),
	NetUseOwnerRelevancy UMETA(DisplayName = "bAlwaysRelevant"),
	OnlyRelevantToOwner UMETA(DisplayName = "bOnlyRelevantToOwner"),
	AttachedToSkeleton UMETA(DisplayName = "bAttachedToSkeleton"),
	IsHidden UMETA(DisplayName = "bIsHidden"),
	UseDistanceBasedRelevancy UMETA(DisplayName = "bUseDistanceBasedRelevancy")
};

UCLASS()
class ARENABATTLE_API ANetCullActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANetCullActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual bool IsNetRelevantFor(const AActor* RealViewer, const AActor* ViewTarget, const FVector& SrcLocation) const override;
	bool CheckNetRelevancy();

private:
	bool bResult;
	
	UPROPERTY()
	class UStaticMeshComponent *CubeMesh;

	ENetRelevancy EMyNetRelevancy;
	
};
