#pragma once
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Core/ShanHeStructs.h"
#include "ChildEducationSystem.generated.h"

UENUM(BlueprintType)
enum class TEducationType : uint8
{
    Family      UMETA(DisplayName="家庭教育"),
    PrivateTutor UMETA(DisplayName="私塾"),
    Academy     UMETA(DisplayName="书院"),
    Official    UMETA(DisplayName="官学"),
    Apprentice  UMETA(DisplayName="学徒"),
    Military    UMETA(DisplayName="武学")
};

USTRUCT(BlueprintType)
struct FEducationProgress
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FGuid ChildID;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) TEducationType EducationType;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 StartAge = 6;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 CurrentAge = 6;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) float Literacy = 0.0f;      // 识字率 0-100
    UPROPERTY(EditAnywhere, BlueprintReadWrite) float Knowledge = 0.0f;     // 学识 0-100
    UPROPERTY(EditAnywhere, BlueprintReadWrite) float MartialSkill = 0.0f;  // 武艺 0-100
    UPROPERTY(EditAnywhere, BlueprintReadWrite) float CommerceSkill = 0.0f; // 商才 0-100
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 TuitionPaid = 0;      // 已付学费
    UPROPERTY(EditAnywhere, BlueprintReadWrite) TArray<FName> LearnedSkills;
};

/**
 * 子女教育系统 - 玩家的后代从小接受教育，决定其人生方向
 * 教育类型影响属性成长和职业选择
 */
UCLASS()
class SHANHEERA_API UChildEducationSystem : public UObject
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintCallable, Category="山河纪元|教育")
    FEducationProgress StartEducation(FGuid ChildID, TEducationType Type, int32 ChildAge);

    UFUNCTION(BlueprintCallable, Category="山河纪元|教育")
    void AdvanceEducation(FEducationProgress& Progress, int32 Years);

    UFUNCTION(BlueprintCallable, Category="山河纪元|教育")
    FCharacterStats CalculateAdultStats(const FEducationProgress& Progress, const FCharacterStats& BaseStats) const;

    UFUNCTION(BlueprintCallable, Category="山河纪元|教育")
    TArray<FName> GetAvailableCareers(const FEducationProgress& Progress) const;

    UFUNCTION(BlueprintCallable, Category="山河纪元|教育")
    int32 GetTuitionCost(TEducationType Type, int32 Years) const;

private:
    float GetEducationQuality(TEducationType Type) const;
};
