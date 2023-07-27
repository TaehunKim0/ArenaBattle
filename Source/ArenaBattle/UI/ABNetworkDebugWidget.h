// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "ABNetworkDebugWidget.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLE_API UABNetworkDebugWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta=(BindWidget))
	UTextBlock* NetworkMode;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* NetworkRole;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* Owner;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* Replicates;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* Relevancy;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* Host;
};
