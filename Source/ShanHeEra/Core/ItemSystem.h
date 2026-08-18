#pragma once
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ItemSystem.generated.h"

UENUM(BlueprintType)
enum class EItemType : uint8
{
    Weapon      UMETA(DisplayName="武器"),
    Armor       UMETA(DisplayName="防具"),
    Accessory   UMETA(DisplayName="饰品"),
    Consumable  UMETA(DisplayName="消耗品"),
    Material    UMETA(DisplayName="材料"),
    Valuable    UMETA(DisplayName="贵重品"),
    Book        UMETA(DisplayName="书籍"),
    Quest       UMETA(DisplayName="任务物品")
};

UENUM(BlueprintType)
enum class EItemRarity : uint8
{
    Common      UMETA(DisplayName="普通"),
    Fine        UMETA(DisplayName="精良"),
    Rare        UMETA(DisplayName="稀有"),
    Epic        UMETA(DisplayName="史诗"),
    Legendary   UMETA(DisplayName="传说")
};

USTRUCT(BlueprintType)
struct FItemData
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName ItemID;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FText ItemName;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FText Description;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) EItemType Type;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) EItemRarity Rarity;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 BasePrice = 10;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Weight = 1;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 StackSize = 1;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Durability = 100;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) TMap<FName, int32> Stats; // 属性加成
    UPROPERTY(EditAnywhere, BlueprintReadWrite) bool bIsEquippable = false;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) bool bIsStackable = true;
};

USTRUCT(BlueprintType)
struct FRecipe
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName RecipeID;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName ResultItemID;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 ResultCount = 1;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) TMap<FName, int32> Materials;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 RequiredSkill = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName SkillType; // 锻造/烹饪/制药/缝纫
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 CraftTime = 1; // 天
};

USTRUCT(BlueprintType)
struct FInventorySlot
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName ItemID;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Count = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 CurrentDurability = 100;
};

/**
 * 物品装备制作系统 - 物品分类、装备属性、锻造/烹饪/制药配方
 */
UCLASS()
class SHANHEERA_API UItemSystem : public UObject
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintCallable, Category="山河纪元|物品")
    void InitializeItems();

    UFUNCTION(BlueprintCallable, Category="山河纪元|物品")
    FItemData GetItem(FName ItemID) const;

    UFUNCTION(BlueprintCallable, Category="山河纪元|物品")
    bool CraftItem(FName RecipeID, TMap<FName, int32>& Inventory, int32 SkillLevel);

    UFUNCTION(BlueprintCallable, Category="山河纪元|物品")
    TArray<FRecipe> GetAvailableRecipes(FName SkillType, int32 SkillLevel) const;

    UPROPERTY(BlueprintReadOnly, Category="山河纪元|物品")
    TMap<FName, FItemData> ItemDatabase;

    UPROPERTY(BlueprintReadOnly, Category="山河纪元|物品")
    TArray<FRecipe> Recipes;
};
