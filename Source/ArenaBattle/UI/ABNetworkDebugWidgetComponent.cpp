// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ABNetworkDebugWidgetComponent.h"
#include "ABNetworkDebugWidget.h"

UABNetworkDebugWidgetComponent::UABNetworkDebugWidgetComponent()
{
	static ConstructorHelpers::FClassFinder<UUserWidget>NETDEBUGWIDGET(TEXT("/Game/ArenaBattle/UI/WBP_ABNetworkDebugWidget.WBP_ABNetworkDebugWidget_C"));
	if (NETDEBUGWIDGET.Class)
	{
		SetWidgetClass(NETDEBUGWIDGET.Class);
	}
}

void UABNetworkDebugWidgetComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                                   FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	const auto DebugWidget = Cast<UABNetworkDebugWidget>(GetWidget());
	
	if (DebugWidget->IsValidLowLevel())
	{
		DebugWidget->NetworkRole->SetText(FText::FromString(FString::Printf(TEXT("Local: %s, Remote : %s"), *NetRole2String(GetOwner()->GetLocalRole()), *NetRole2String(GetOwner()->GetRemoteRole()))));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, FString::Printf(TEXT("없음!")));
	}
}

FString UABNetworkDebugWidgetComponent::NetRole2String(ENetRole NetRole)
{
	switch (NetRole)
	{
	case ROLE_Authority:
		return TEXT("Authority");
	case ROLE_AutonomousProxy:
		return TEXT("AutonomousProxy");
	case ROLE_SimulatedProxy:
		return TEXT("SimulatedProxy");
	case ROLE_None:
		return TEXT("None");
	case ROLE_MAX:
		return TEXT("MAX");
	}
	return {};
}
