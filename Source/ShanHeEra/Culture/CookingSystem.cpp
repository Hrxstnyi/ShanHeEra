#include "Culture/CookingSystem.h"
#include "Core/ShanHeLog.h"

void UCookingSystem::InitializeCooking()
{
    // 初始化食材
    struct FIngDef { FName ID; const TCHAR* Name; int32 Price; int32 Nut; };
    FIngDef Ingredients[] = {
        {TEXT("Rice"), NSLOCTEXT("Ing","Rice","大米"), 5, 20},
        {TEXT("Wheat"), NSLOCTEXT("Ing","Wheat","面粉"), 4, 18},
        {TEXT("Pork"), NSLOCTEXT("Ing","Pork","猪肉"), 20, 30},
        {TEXT("Beef"), NSLOCTEXT("Ing","Beef","牛肉"), 30, 35},
        {TEXT("Chicken"), NSLOCTEXT("Ing","Chicken","鸡肉"), 15, 25},
        {TEXT("Fish"), NSLOCTEXT("Ing","Fish","鱼"), 25, 28},
        {TEXT("Shrimp"), NSLOCTEXT("Ing","Shrimp","虾"), 40, 25},
        {TEXT("Cabbage"), NSLOCTEXT("Ing","Cabbage","白菜"), 3, 10},
        {TEXT("Tofu"), NSLOCTEXT("Ing","Tofu","豆腐"), 8, 15},
        {TEXT("Egg"), NSLOCTEXT("Ing","Egg","鸡蛋"), 10, 12},
        {TEXT("Mushroom"), NSLOCTEXT("Ing","Mushroom","蘑菇"), 15, 15},
        {TEXT("Tea"), NSLOCTEXT("Ing","Tea","茶叶"), 50, 5},
    };
    for (const auto& I : Ingredients)
    {
        FIngredient D;
        D.IngredientID = I.ID; D.Name = I.Name;
        D.Price = I.Price; D.Nutrition = I.Nut;
        IngredientDatabase.Add(I.ID, D);
    }

    // 初始化菜品
    FDish Rice;
    Rice.DishID = TEXT("SteamedRice");
    Rice.DishName = NSLOCTEXT("Dish","Rice","白米饭");
    Rice.Cuisine = ECuisineType::Lu;
    Rice.Ingredients = {TEXT("Rice")};
    Rice.Satiety = 20;
    DishDatabase.Add(Rice);

    FDish Dumpling;
    Dumpling.DishID = TEXT("Dumpling");
    Dumpling.DishName = NSLOCTEXT("Dish","Dumpling","饺子");
    Dumpling.Cuisine = ECuisineType::Lu;
    Dumpling.Ingredients = {TEXT("Wheat"), TEXT("Pork"), TEXT("Cabbage")};
    Dumpling.RequiredCookingSkill = 10;
    Dumpling.Satiety = 40; Dumpling.Happiness = 10;
    DishDatabase.Add(Dumpling);

    FDish Mapo;
    Mapo.DishID = TEXT("MapoTofu");
    Mapo.DishName = NSLOCTEXT("Dish","Mapo","麻婆豆腐");
    Mapo.Cuisine = ECuisineType::Chuan;
    Mapo.Ingredients = {TEXT("Tofu"), TEXT("Pork")};
    Mapo.RequiredCookingSkill = 20;
    Mapo.Satiety = 30; Mapo.Happiness = 15;
    DishDatabase.Add(Mapo);

    FDish Mooncake;
    Mooncake.DishID = TEXT("Mooncake");
    Mooncake.DishName = NSLOCTEXT("Dish","Mooncake","月饼");
    Mooncake.Cuisine = ECuisineType::Yue;
    Mooncake.Ingredients = {TEXT("Wheat")};
    Mooncake.RequiredCookingSkill = 30;
    Mooncake.Satiety = 15; Mooncake.Happiness = 30;
    DishDatabase.Add(Mooncake);

    UE_LOG(LogShanHe, Log, TEXT("烹饪系统初始化 - %d 食材，%d 菜品"), IngredientDatabase.Num(), DishDatabase.Num());
}

bool UCookingSystem::CookDish(FName DishID, TMap<FName, int32>& Inventory, int32 CookingSkill)
{
    for (const FDish& D : DishDatabase)
    {
        if (D.DishID == DishID)
        {
            if (CookingSkill < D.RequiredCookingSkill) return false;
            for (FName Ing : D.Ingredients)
            {
                if (const int32* Have = Inventory.Find(Ing))
                { if (*Have < 1) return false; }
                else return false;
            }
            for (FName Ing : D.Ingredients)
            {
                int32& Have = Inventory.FindOrAdd(Ing);
                Have--;
            }
            UE_LOG(LogShanHe, Log, TEXT("烹饪成功：%s"), *D.DishName.ToString());
            return true;
        }
    }
    return false;
}

void UCookingSystem::EatDish(FName DishID)
{
    for (const FDish& D : DishDatabase)
    {
        if (D.DishID == DishID)
        {
            CurrentSatiety = FMath::Min(100, CurrentSatiety + D.Satiety);
            UE_LOG(LogShanHe, Log, TEXT("享用：%s，饱食度+%d"), *D.DishName.ToString(), D.Satiety);
            return;
        }
    }
}

TArray<FDish> UCookingSystem::GetAvailableDishes(int32 CookingSkill) const
{
    TArray<FDish> Result;
    for (const FDish& D : DishDatabase)
        if (D.RequiredCookingSkill <= CookingSkill) Result.Add(D);
    return Result;
}
