#pragma once
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ShanHeMartialArtsSystem.generated.h"

UENUM(BlueprintType)
enum class ESkillType : uint8
{
    Internal    UMETA(DisplayName="内功"),
    External    UMETA(DisplayName="外功"),
    Lightness   UMETA(DisplayName="轻功"),
    Hidden      UMETA(DisplayName="暗器")
};

UENUM(BlueprintType)
enum class ESkillGrade : uint8
{
    Common      UMETA(DisplayName="凡品"),
    Fine        UMETA(DisplayName="良品"),
    Excellent   UMETA(DisplayName="精品"),
    Master      UMETA(DisplayName="极品"),
    Legendary   UMETA(DisplayName="神品")
};

USTRUCT(BlueprintType)
struct FMartialSkill
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName SkillID;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FText SkillName;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) ESkillType Type;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) ESkillGrade Grade;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Level = 0; // 0-100
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 MaxLevel = 100;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) float BaseDamage = 10.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) float StaminaCost = 10.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) TArray<FName> RequiredAttributes; // 修炼条件
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FText Description;
};

USTRUCT(BlueprintType)
struct FSkillManual
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName ManualID;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName SkillID;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FText ManualName;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) ESkillGrade Grade;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 ComprehensionDifficulty = 50; // 参悟难度
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FText Source; // 获取来源
};

/**
 * 武功秘籍系统 - 内功/外功/轻功/暗器，修炼体系，秘籍获取与参悟
 */
UCLASS()
class SHANHEERA_API UMartialArtsSystem : public UObject
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintCallable, Category="山河纪元|武功")
    void InitializeSkills();

    UFUNCTION(BlueprintCallable, Category="山河纪元|武功")
    bool LearnSkill(FName SkillID);

    UFUNCTION(BlueprintCallable, Category="山河纪元|武功")
    bool PracticeSkill(FName SkillID, float Hours);

    UFUNCTION(BlueprintCallable, Category="山河纪元|武功")
    bool ComprehendManual(FName ManualID, int32 Intelligence);

    UFUNCTION(BlueprintCallable, Category="山河纪元|武功")
    float GetSkillPower(FName SkillID) const;

    UFUNCTION(BlueprintCallable, Category="山河纪元|武功")
    TArray<FMartialSkill> GetLearnedSkills() const;

    UFUNCTION(BlueprintCallable, Category="山河纪元|武功")
    bool CanLearnSkill(FName SkillID, int32 Strength, int32 Intelligence, int32 Agility) const;

    UPROPERTY(BlueprintReadOnly, Category="山河纪元|武功")
    TMap<FName, FMartialSkill> SkillDatabase;

    UPROPERTY(BlueprintReadOnly, Category="山河纪元|武功")
    TMap<FName, FSkillManual> ManualDatabase;

    UPROPERTY(BlueprintReadOnly, Category="山河纪元|武功")
    TArray<FName> LearnedSkills;
};
