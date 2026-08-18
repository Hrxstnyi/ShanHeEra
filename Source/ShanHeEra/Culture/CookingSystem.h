#pragma once
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CookingSystem.generated.h"

UENUM(BlueprintType)
enum class ECuisineType : uint8
{
    Lu      UMETA(DisplayName="鲁菜"),
    Chuan   UMETA(DisplayName="川菜"),
    Yue     UMETA(DisplayName="粤菜"),
    Su      UMETA(DisplayName="苏菜"),
    Min     UMETA(DisplayName="闽菜"),
    Zhe     UMETA(DisplayName="浙菜"),
    Xiang   UMETA(DisplayName="湘菜"),
    Hui     UMETA(DisplayName="徽菜")
};

USTRUCT(BlueprintType)
struct FIngredient
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName IngredientID;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FText Name;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Price = 10;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Nutrition = 10;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName OriginRegion;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Season = 0; // 0=全年
};

USTRUCT(BlueprintType)
struct FDish
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName DishID;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FText DishName;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) ECuisineType Cuisine;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) TArray<FName> Ingredients;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 RequiredCookingSkill = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Satiety = 30;       // 饱食度
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Happiness = 5;      // 快乐度
    UPROPERTY(EditAnywhere, BlueprintReadWrite) TMap<FName, int32> Buffs; // 临时buff
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 CookTime = 1;       // 时辰
};

/**
 * 饮食烹饪系统 - 食材采集、菜谱、饱食度、菜品buff、八大菜系
 */
UCLASS()
class SHANHEERA_API UCookingSystem : public UObject
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintCallable, Category="山河纪元|烹饪")
    void InitializeCooking();

    UFUNCTION(BlueprintCallable, Category="山河纪元|烹饪")
    bool CookDish(FName DishID, TMap<FName, int32>& Inventory, int32 CookingSkill);

    UFUNCTION(BlueprintCallable, Category="山河纪元|烹饪")
    void EatDish(FName DishID);

    UFUNCTION(BlueprintCallable, Category="山河纪元|烹饪")
    TArray<FDish> GetAvailableDishes(int32 CookingSkill) const;

    UPROPERTY(BlueprintReadOnly, Category="山河纪元|烹饪")
    TMap<FName, FIngredient> IngredientDatabase;

    UPROPERTY(BlueprintReadOnly, Category="山河纪元|烹饪")
    TArray<FDish> DishDatabase;

    UPROPERTY(BlueprintReadOnly, Category="山河纪元|烹饪")
    int32 CurrentSatiety = 100; // 0-100
};
