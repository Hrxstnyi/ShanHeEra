#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SectWidget.generated.h"

class UVerticalBox;
class UTextBlock;
class UButton;
class UImage;
class UProgressBar;

/**
 * 帮派界面 - 门派成员/任务/战争/外交管理
 */
UCLASS()
class SHANHEERA_API USectWidget : public UUserWidget
{
    GENERATED_BODY()
public:
    // 概览页
    UPROPERTY(meta = (BindWidget)) UTextBlock* SectNameText;
    UPROPERTY(meta = (BindWidget)) UTextBlock* SectRankText;
    UPROPERTY(meta = (BindWidget)) UTextBlock* MemberCountText;
    UPROPERTY(meta = (BindWidget)) UTextBlock* InfluenceText;
    UPROPERTY(meta = (BindWidget)) UTextBlock* WealthText;
    UPROPERTY(meta = (BindWidget)) UProgressBar* InfluenceBar;

    // 成员页
    UPROPERTY(meta = (BindWidget)) UVerticalBox* MemberList;
    UPROPERTY(meta = (BindWidget)) UTextBlock* SelectedMemberName;
    UPROPERTY(meta = (BindWidget)) UTextBlock* SelectedMemberRank;
    UPROPERTY(meta = (BindWidget)) UTextBlock* SelectedMemberContribution;

    // 任务页
    UPROPERTY(meta = (BindWidget)) UVerticalBox* MissionList;
    UPROPERTY(meta = (BindWidget)) UTextBlock* MissionTitleText;
    UPROPERTY(meta = (BindWidget)) UTextBlock* MissionDescText;
    UPROPERTY(meta = (BindWidget)) UTextBlock* MissionRewardText;
    UPROPERTY(meta = (BindWidget)) UButton* AcceptMissionButton;

    // 战争/外交页
    UPROPERTY(meta = (BindWidget)) UVerticalBox* EnemyList;
    UPROPERTY(meta = (BindWidget)) UVerticalBox* AllyList;
    UPROPERTY(meta = (BindWidget)) UButton* DeclareWarButton;
    UPROPERTY(meta = (BindWidget)) UButton* ProposeAllyButton;

    // 标签页切换
    UPROPERTY(meta = (BindWidget)) UButton* TabOverview;
    UPROPERTY(meta = (BindWidget)) UButton* TabMembers;
    UPROPERTY(meta = (BindWidget)) UButton* TabMissions;
    UPROPERTY(meta = (BindWidget)) UButton* TabWar;

    UFUNCTION(BlueprintCallable, Category="山河纪元|UI")
    void RefreshSect();

    UFUNCTION(BlueprintCallable, Category="山河纪元|UI")
    void SelectMember(FName MemberID);

    UFUNCTION(BlueprintCallable, Category="山河纪元|UI")
    void SelectMission(FName MissionID);

    UFUNCTION(BlueprintCallable, Category="山河纪元|UI")
    void AcceptMission();

    UFUNCTION(BlueprintCallable, Category="山河纪元|UI")
    void DeclareWar(FName EnemySectID);

    UFUNCTION(BlueprintCallable, Category="山河纪元|UI")
    void ProposeAlliance(FName AllySectID);

    UFUNCTION(BlueprintCallable, Category="山河纪元|UI")
    void SwitchTab(int32 TabIndex); // 0=概览 1=成员 2=任务 3=战争

protected:
    virtual void NativeConstruct() override;
    int32 CurrentTab = 0;
    FName SelectedMemberID;
    FName SelectedMissionID;
};
