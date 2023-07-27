// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "ABNetworkDebugWidgetComponent.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLE_API UABNetworkDebugWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()

public:
	UABNetworkDebugWidgetComponent();
	
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	//** Actor Replication **//
	void SetActorReplicationUI() const;
	
	void SetNetworkRoleUI() const;
	void SetNetworkModeUI() const;
	void SetIsReplicateUI() const;
	void SetRelevancyUI() const;
	void SetOwnerShipUI() const;
	void SetHostUI() const;

	static FString NetRoleToString(ENetRole NetRole);
	static FString NetModeToString(ENetMode NetMode);
	
private:
	UPROPERTY()
	class UABNetworkDebugWidget* DebugWidget;
};
