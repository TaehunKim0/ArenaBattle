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
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	FString NetRole2String(ENetRole NetRole);
};
