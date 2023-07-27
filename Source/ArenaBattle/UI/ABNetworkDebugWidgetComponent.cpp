// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ABNetworkDebugWidgetComponent.h"
#include "ABNetworkDebugWidget.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

UABNetworkDebugWidgetComponent::UABNetworkDebugWidgetComponent()
{
	static ConstructorHelpers::FClassFinder<UUserWidget> NETDEBUGWIDGET(TEXT("/Game/ArenaBattle/UI/WBP_ABNetworkDebugWidget.WBP_ABNetworkDebugWidget_C"));
	if (NETDEBUGWIDGET.Class)
	{
		SetWidgetClass(NETDEBUGWIDGET.Class);
	}
}

void UABNetworkDebugWidgetComponent::BeginPlay()
{
	Super::BeginPlay();

	DebugWidget = Cast<UABNetworkDebugWidget>(GetWidget());
	checkf(IsValid(DebugWidget), TEXT("DebugWidget이 유효하지 않습니다."));
}

void UABNetworkDebugWidgetComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                                   FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	SetActorReplicationUI();
}

void UABNetworkDebugWidgetComponent::SetActorReplicationUI() const
{
	SetNetworkRoleUI();
	SetNetworkModeUI();
	SetIsReplicateUI();
	SetRelevancyUI();
	SetOwnerShipUI();
	SetHostUI();
}

void UABNetworkDebugWidgetComponent::SetNetworkRoleUI() const
{
	if (DebugWidget->IsValidLowLevel())
	{
		DebugWidget->NetworkRole->SetText(FText::FromString(FString::Printf(TEXT("Local: %s, Remote : %s"), *NetRoleToString(GetOwner()->GetLocalRole()), *NetRoleToString(GetOwner()->GetRemoteRole()))));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, FString::Printf(TEXT("DebugWidget 없음!")));
	}
}

void UABNetworkDebugWidgetComponent::SetNetworkModeUI() const
{
	if (DebugWidget->IsValidLowLevel())
	{
		const FString Text = NetModeToString(GetOwner()->GetNetMode());
		DebugWidget->NetworkMode->SetText(FText::FromString(Text));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, FString::Printf(TEXT("DebugWidget 없음!")));
	}
}

void UABNetworkDebugWidgetComponent::SetIsReplicateUI() const
{
	if (DebugWidget->IsValidLowLevel())
	{
		const FText Text = GetOwner()->GetIsReplicated() ? FText::FromString(TEXT("True")) : FText::FromString(TEXT("False"));
		DebugWidget->Replicates->SetText(Text);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, FString::Printf(TEXT("DebugWidget 없음!")));
	}
}

void UABNetworkDebugWidgetComponent::SetRelevancyUI() const
{
	if (DebugWidget->IsValidLowLevel())
	{
		FString Text = GetOwner()->bAlwaysRelevant ? TEXT("AlwaysRelevant") : TEXT("NotAlwaysRelevant");
		Text += TEXT(", ");
		Text += GetOwner()->bNetUseOwnerRelevancy ? TEXT("NetUseOwnerRelevancy") : TEXT("NotNetUseOwnerRelevancy");
		
		DebugWidget->Relevancy->SetText(FText::FromString(Text));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, FString::Printf(TEXT("DebugWidget 없음!")));
	}
}

void UABNetworkDebugWidgetComponent::SetOwnerShipUI() const
{
	if (DebugWidget->IsValidLowLevel())
	{
		const bool bHasLocalNetOwner = GetOwner()->HasLocalNetOwner();
		
		FString Text = bHasLocalNetOwner ? TEXT("HasLocalNetOwner") : TEXT("NotHasLocalNetOwner");
		Text += TEXT(", ");
		
		if(true == bHasLocalNetOwner)
			Text += Cast<ACharacter>(GetOwner())->GetController()->GetName();

		DebugWidget->Owner->SetText(FText::FromString(Text));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, FString::Printf(TEXT("DebugWidget 없음!")));
	}
}

void UABNetworkDebugWidgetComponent::SetHostUI() const
{
	if (DebugWidget->IsValidLowLevel())
	{
		auto Player = Cast<ACharacter>(GetOwner());
		if (Player->IsLocallyControlled())
		{
			if (Player->HasAuthority())
			{
				DebugWidget->Host->SetText(FText::FromString(TEXT("서버")));
			}
			else
			{
				DebugWidget->Host->SetText(FText::FromString(TEXT("클라")));
			}
		}
		else
		{
			if (Player->HasAuthority())
			{
				DebugWidget->Host->SetText(FText::FromString(TEXT("클라")));
			}
			else
			{
				DebugWidget->Host->SetText(FText::FromString(TEXT("서버")));
			}
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, FString::Printf(TEXT("DebugWidget 없음!")));
	}
}

FString UABNetworkDebugWidgetComponent::NetModeToString(ENetMode NetMode)
{
	switch (NetMode)
	{
	case NM_Standalone:
		return TEXT("Standalone");
	case NM_DedicatedServer:
		return TEXT("DedicatedServer");
	case NM_ListenServer:
		return TEXT("ListenServer");
	case NM_Client:
		return TEXT("Client");
	case NM_MAX:
		return TEXT("MAX");
	}
	return {};
}

FString UABNetworkDebugWidgetComponent::NetRoleToString(ENetRole NetRole)
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
