#pragma once
#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Family/FamilyTree.h"
#include "Core/ShanHeStructs.h"
#include "ShanHeGenerationSwitchSystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnGenerationSwitch, FGuid, OldCharacterID, FGuid, NewCharacterID);

/**
 * 世代切换系统 - 玩家角色死亡后，选择一个后代继续游戏
 * 这是太吾绘卷式多代传承的核心
 */
UCLASS()
class SHANHEERA_API UGenerationSwitchSystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()
public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;

    UFUNCTION(BlueprintCallable, Category="山河纪元|传承")
    void OnPlayerCharacterDeath(FGuid DeceasedID, UFamilyTree* FamilyTree);

    UFUNCTION(BlueprintCallable, Category="山河纪元|传承")
    TArray<FFamilyMember> GetEligibleHeirs(FGuid DeceasedID, UFamilyTree* FamilyTree) const;

    UFUNCTION(BlueprintCallable, Category="山河纪元|传承")
    void SwitchToHeir(FGuid HeirID);

    UFUNCTION(BlueprintCallable, Category="山河纪元|传承")
    int32 GetCurrentGeneration() const { return CurrentGeneration; }

    UFUNCTION(BlueprintCallable, Category="山河纪元|传承")
    int32 GetTotalGenerationsPlayed() const { return TotalGenerations; }

    UPROPERTY(BlueprintAssignable, Category="山河纪元|传承")
    FOnGenerationSwitch OnGenerationSwitch;

    UPROPERTY(BlueprintReadOnly, Category="山河纪元|传承")
    FGuid CurrentPlayerID;

    UPROPERTY(BlueprintReadOnly, Category="山河纪元|传承")
    int32 CurrentGeneration = 1;

    UPROPERTY(BlueprintReadOnly, Category="山河纪元|传承")
    int32 TotalGenerations = 1;

    UPROPERTY(BlueprintReadOnly, Category="山河纪元|传承")
    bool bCanSwitchGeneration = false;

private:
    void ShowHeirSelectionUI(const TArray<FFamilyMember>& Heirs);
    void ApplyInheritanceBonuses(FGuid HeirID, FGuid DeceasedID);
};
