#pragma once
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AttributeSystem.generated.h"

UENUM(BlueprintType)
enum class EAttributeType : uint8
{
    Strength    UMETA(DisplayName="膂力"),  // 力量，影响近战伤害/负重
    Intelligence UMETA(DisplayName="悟性"), // 学习速度/科举/医术
    Courage     UMETA(DisplayName="胆识"),  // 士气/战场表现/江湖事件
    Vitality    UMETA(DisplayName="根骨"),  // 生命值/抗病/寿命
    Agility     UMETA(DisplayName="灵敏"),  // 闪避/远程/轻功
    Luck        UMETA(DisplayName="福源")   // 随机事件/掉落/奇遇
};

UENUM(BlueprintType)
enum class ESkillType : uint8
{
    // 战斗
    Sword       UMETA(DisplayName="剑法"),
    Spear       UMETA(DisplayName="枪法"),
    Archery     UMETA(DisplayName="箭术"),
    Unarmed     UMETA(DisplayName="拳脚"),
    Defense     UMETA(DisplayName="格挡"),
    // 生活
    Smithing    UMETA(DisplayName="锻造"),
    Cooking     UMETA(DisplayName="烹饪"),
    Medicine    UMETA(DisplayName="医术"),
    Farming     UMETA(DisplayName="农耕"),
    Trading     UMETA(DisplayName="经商"),
    // 文化
    Literature  UMETA(DisplayName="文学"),
    Calligraphy UMETA(DisplayName="书法"),
    Painting    UMETA(DisplayName="绘画"),
    Music       UMETA(DisplayName="音律"),
    // 社交
    Persuasion  UMETA(DisplayName="口才"),
    Intimidation UMETA(DisplayName="威吓"),
    Stealth     UMETA(DisplayName="潜行")
};

USTRUCT(BlueprintType)
struct FCharacterAttributes
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Strength = 10;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Intelligence = 10;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Courage = 10;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Vitality = 10;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Agility = 10;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Luck = 10;

    int32 GetAttribute(EAttributeType Type) const
    {
        switch(Type)
        {
            case EAttributeType::Strength: return Strength;
            case EAttributeType::Intelligence: return Intelligence;
            case EAttributeType::Courage: return Courage;
            case EAttributeType::Vitality: return Vitality;
            case EAttributeType::Agility: return Agility;
            case EAttributeType::Luck: return Luck;
            default: return 10;
        }
    }
};

USTRUCT(BlueprintType)
struct FSkillData
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite) ESkillType SkillType;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Level = 0;       // 0-100
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Experience = 0;  // 当前经验
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 ExpToNext = 100; // 升级所需
};

USTRUCT(BlueprintType)
struct FStatusEffect
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName EffectID;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FText EffectName;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 DurationDays = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) TMap<EAttributeType, int32> AttributeModifiers;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) bool bIsBuff = true;
};

/**
 * 属性技能系统 - 六维属性、17种技能、buff/debuff
 * 属性影响技能上限和学习速度，技能通过使用获得经验
 */
UCLASS()
class SHANHEERA_API UAttributeSystem : public UObject
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintCallable, Category="山河纪元|属性")
    void InitializeAttributes(FCharacterAttributes InitialAttrs);

    UFUNCTION(BlueprintCallable, Category="山河纪元|属性")
    void AddSkillExperience(ESkillType Skill, int32 Amount);

    UFUNCTION(BlueprintCallable, Category="山河纪元|属性")
    int32 GetSkillLevel(ESkillType Skill) const;

    UFUNCTION(BlueprintCallable, Category="山河纪元|属性")
    void AddStatusEffect(FStatusEffect Effect);

    UFUNCTION(BlueprintCallable, Category="山河纪元|属性")
    void RemoveStatusEffect(FName EffectID);

    UFUNCTION(BlueprintCallable, Category="山河纪元|属性")
    void SimulateEffects(int32 Days);

    UFUNCTION(BlueprintCallable, Category="山河纪元|属性")
    int32 GetEffectiveAttribute(EAttributeType Type) const; // 含buff修正

    UPROPERTY(BlueprintReadOnly, Category="山河纪元|属性")
    FCharacterAttributes BaseAttributes;

    UPROPERTY(BlueprintReadOnly, Category="山河纪元|属性")
    TMap<ESkillType, FSkillData> Skills;

    UPROPERTY(BlueprintReadOnly, Category="山河纪元|属性")
    TArray<FStatusEffect> ActiveEffects;

private:
    void CheckLevelUp(ESkillType Skill);
};
