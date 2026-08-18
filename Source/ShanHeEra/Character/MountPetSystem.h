#pragma once
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ShanHeMountPetSystem.generated.h"

UENUM(BlueprintType)
enum class EMountType : uint8
{
    Horse       UMETA(DisplayName="马匹"),
    Donkey      UMETA(DisplayName="驴"),
    Camel       UMETA(DisplayName="骆驼"),
    Elephant    UMETA(DisplayName="大象")
};

UENUM(BlueprintType)
enum class EPetType : uint8
{
    Dog         UMETA(DisplayName="犬"),
    Cat         UMETA(DisplayName="猫"),
    Hawk        UMETA(DisplayName="鹰"),
    Hound       UMETA(DisplayName="猎犬"),
    Parrot      UMETA(DisplayName="鹦鹉"),
    Monkey      UMETA(DisplayName="猴子")
};

USTRUCT(BlueprintType)
struct FMount
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName MountID;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FText Name;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) EMountType Type;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Speed = 50; // 速度
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Stamina = 100; // 体力
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Loyalty = 50; // 忠诚度 0-100
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Age = 5;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Value = 1000;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) bool bIsWarMount = false;
};

USTRUCT(BlueprintType)
struct FPet
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName PetID;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FText Name;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) EPetType Type;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Affection = 50; // 亲密度
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 HuntingSkill = 30; // 狩猎技能
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 CombatSkill = 20; // 战斗辅助
    UPROPERTY(EditAnywhere, BlueprintReadWrite) bool bFollowsPlayer = false;
};

/**
 * 坐骑宠物系统 - 马匹养成、鹰犬狩猎、宠物跟随、骑乘战斗
 */
UCLASS()
class SHANHEERA_API UMountPetSystem : public UObject
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintCallable, Category="山河纪元|坐骑宠物")
    void InitializeMountsPets();

    UFUNCTION(BlueprintCallable, Category="山河纪元|坐骑宠物")
    bool AcquireMount(FMount NewMount);

    UFUNCTION(BlueprintCallable, Category="山河纪元|坐骑宠物")
    bool AcquirePet(FPet NewPet);

    UFUNCTION(BlueprintCallable, Category="山河纪元|坐骑宠物")
    void FeedMount(FName MountID);

    UFUNCTION(BlueprintCallable, Category="山河纪元|坐骑宠物")
    void TrainMount(FName MountID, float Hours);

    UFUNCTION(BlueprintCallable, Category="山河纪元|坐骑宠物")
    void PlayWithPet(FName PetID);

    UFUNCTION(BlueprintCallable, Category="山河纪元|坐骑宠物")
    int32 HuntWithPet(FName PetID); // 狩猎收益

    UFUNCTION(BlueprintCallable, Category="山河纪元|坐骑宠物")
    bool MountCombatBonus(FName MountID) const; // 骑乘战斗加成

    UPROPERTY(BlueprintReadOnly, Category="山河纪元|坐骑宠物")
    TArray<FMount> Mounts;

    UPROPERTY(BlueprintReadOnly, Category="山河纪元|坐骑宠物")
    TArray<FPet> Pets;

    UPROPERTY(BlueprintReadOnly, Category="山河纪元|坐骑宠物")
    FName ActiveMount = NAME_None;

    UPROPERTY(BlueprintReadOnly, Category="山河纪元|坐骑宠物")
    FName ActivePet = NAME_None;
};
