#pragma once
#include "CoreMinimal.h"
#include "ShanHeEnums.generated.h"

UENUM(BlueprintType)
enum class EEraType : uint8
{
    Ancient         UMETA(DisplayName="远古时代"),
    Feudal          UMETA(DisplayName="诸侯时代"),
    Unified         UMETA(DisplayName="统一王朝"),
    Prosperous      UMETA(DisplayName="盛世时代"),
    Decline         UMETA(DisplayName="衰落时代"),
    Chaos           UMETA(DisplayName="乱世时代"),
    Modern          UMETA(DisplayName="近现代")
};

UENUM(BlueprintType)
enum class ESeason : uint8
{
    Spring      UMETA(DisplayName="春"),
    Summer      UMETA(DisplayName="夏"),
    Autumn      UMETA(DisplayName="秋"),
    Winter      UMETA(DisplayName="冬")
};

UENUM(BlueprintType)
enum class ESocialClass : uint8
{
    Royalty     UMETA(DisplayName="皇室"),
    Official    UMETA(DisplayName="官员"),
    Scholar     UMETA(DisplayName="士人"),
    Gentry      UMETA(DisplayName="乡绅"),
    Merchant    UMETA(DisplayName="商人"),
    Artisan     UMETA(DisplayName="工匠"),
    Farmer      UMETA(DisplayName="农民"),
    Laborer     UMETA(DisplayName="雇工"),
    Outcast     UMETA(DisplayName="江湖人")
};

UENUM(BlueprintType)
enum class ERegionType : uint8
{
    NorthPlain  UMETA(DisplayName="北方平原"),
    Jiangnan    UMETA(DisplayName="江南水乡"),
    Northwest   UMETA(DisplayName="西北边疆"),
    Southwest   UMETA(DisplayName="西南山区"),
    Coast       UMETA(DisplayName="沿海地区"),
    Inland      UMETA(DisplayName="内陆乡村"),
    Taiwan      UMETA(DisplayName="东南海岛")
};

UENUM(BlueprintType)
enum class ECharacterTrait : uint8
{
    Kind        UMETA(DisplayName="仁厚"),
    Brave       UMETA(DisplayName="勇武"),
    Wise        UMETA(DisplayName="智谋"),
    Ambitious   UMETA(DisplayName="野心"),
    Loyal       UMETA(DisplayName="忠义"),
    Greedy      UMETA(DisplayName="贪婪"),
    Honest      UMETA(DisplayName="清廉"),
    Corrupt     UMETA(DisplayName="贪腐"),
    Childlike   UMETA(DisplayName="童真"),
    Reliable    UMETA(DisplayName="可靠"),
    Cunning     UMETA(DisplayName="狡诈"),
    Compassionate UMETA(DisplayName="慈悲")
};

UENUM(BlueprintType)
enum class EDisasterType : uint8
{
    Flood       UMETA(DisplayName="洪水"),
    Drought     UMETA(DisplayName="干旱"),
    Locust      UMETA(DisplayName="蝗灾"),
    Earthquake  UMETA(DisplayName="地震"),
    Plague      UMETA(DisplayName="疫病"),
    Cold        UMETA(DisplayName="寒灾"),
    Famine      UMETA(DisplayName="饥荒")
};

UENUM(BlueprintType)
enum class EWarResult : uint8
{
    Victory     UMETA(DisplayName="胜利"),
    Defeat      UMETA(DisplayName="失败"),
    Stalemate   UMETA(DisplayName="僵持"),
    Pyrrhic     UMETA(DisplayName="惨胜")
};

UENUM(BlueprintType)
enum class ELanguage : uint8
{
    Chinese     UMETA(DisplayName="中文"),
    English     UMETA(DisplayName="English"),
    Japanese    UMETA(DisplayName="日本語")
};
