#include "Core/ItemSystem.h"
#include "Core/ShanHeLog.h"

void UItemSystem::InitializeItems()
{
    // 初始化基础物品
    struct FItemDef { FName ID; const TCHAR* Name; EItemType Type; EItemRarity Rarity; int32 Price; bool Equip; };
    FItemDef Items[] = {
        {TEXT("IronSword"), TEXT("铁剑"), EItemType::Weapon, EItemRarity::Common, 100, true},
        {TEXT("SteelSword"), TEXT("钢剑"), EItemType::Weapon, EItemRarity::Fine, 500, true},
        {TEXT("LegendaryBlade"), TEXT("屠龙刀"), EItemType::Weapon, EItemRarity::Legendary, 50000, true},
        {TEXT("LeatherArmor"), TEXT("皮甲"), EItemType::Armor, EItemRarity::Common, 80, true},
        {TEXT("IronArmor"), TEXT("铁甲"), EItemType::Armor, EItemRarity::Fine, 400, true},
        {TEXT("HealthPotion"), TEXT("金疮药"), EItemType::Consumable, EItemRarity::Common, 50, false},
        {TEXT("Ginseng"), TEXT("百年人参"), EItemType::Consumable, EItemRarity::Rare, 2000, false},
        {TEXT("IronOre"), TEXT("铁矿石"), EItemType::Material, EItemRarity::Common, 10, false},
        {TEXT("Steel"), TEXT("精钢"), EItemType::Material, EItemRarity::Fine, 100, false},
        {TEXT("Jade"), TEXT("和田玉"), EItemType::Valuable, EItemRarity::Rare, 3000, false},
        {TEXT("Analects"), TEXT("论语"), EItemType::Book, EItemRarity::Fine, 500, false},
        {TEXT("SunTzu"), TEXT("孙子兵法"), EItemType::Book, EItemRarity::Epic, 5000, false},
    };
    for (const auto& I : Items)
    {
        FItemData D;
        D.ItemID = I.ID; D.ItemName = FText::FromString(I.Name); D.Type = I.Type;
        D.Rarity = I.Rarity; D.BasePrice = I.Price; D.bIsEquippable = I.Equip;
        ItemDatabase.Add(I.ID, D);
    }

    // 初始化配方
    FRecipe SwordRecipe;
    SwordRecipe.RecipeID = TEXT("Craft_IronSword");
    SwordRecipe.ResultItemID = TEXT("IronSword");
    SwordRecipe.Materials.Add(TEXT("IronOre"), 3);
    SwordRecipe.RequiredSkill = 10;
    SwordRecipe.SkillType = TEXT("Smithing");
    SwordRecipe.CraftTime = 2;
    Recipes.Add(SwordRecipe);

    UE_LOG(LogShanHe, Log, TEXT("物品系统初始化 - %d 物品，%d 配方"), ItemDatabase.Num(), Recipes.Num());
}

FItemData UItemSystem::GetItem(FName ItemID) const
{
    if (const FItemData* Found = ItemDatabase.Find(ItemID))
        return *Found;
    return FItemData();
}

bool UItemSystem::CraftItem(FName RecipeID, TMap<FName, int32>& Inventory, int32 SkillLevel)
{
    for (const FRecipe& R : Recipes)
    {
        if (R.RecipeID == RecipeID)
        {
            if (SkillLevel < R.RequiredSkill) return false;
            // 检查材料
            for (const auto& Mat : R.Materials)
            {
                if (const int32* Have = Inventory.Find(Mat.Key))
                {
                    if (*Have < Mat.Value) return false;
                }
                else return false;
            }
            // 扣除材料
            for (const auto& Mat : R.Materials)
            {
                int32& Have = Inventory.FindOrAdd(Mat.Key);
                Have -= Mat.Value;
            }
            // 添加产物
            int32& Result = Inventory.FindOrAdd(R.ResultItemID);
            Result += R.ResultCount;
            UE_LOG(LogShanHe, Log, TEXT("制作成功：%s x%d"), *R.ResultItemID.ToString(), R.ResultCount);
            return true;
        }
    }
    return false;
}

TArray<FRecipe> UItemSystem::GetAvailableRecipes(FName SkillType, int32 SkillLevel) const
{
    TArray<FRecipe> Result;
    for (const FRecipe& R : Recipes)
        if (R.SkillType == SkillType && R.RequiredSkill <= SkillLevel)
            Result.Add(R);
    return Result;
}
