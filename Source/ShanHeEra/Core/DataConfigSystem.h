#pragma once
#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "UObject/NoExportTypes.h"
#include "DataConfigSystem.generated.h"

USTRUCT(BlueprintType)
struct FItemConfigRow : public FTableRowBase
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName ItemID;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FText ItemName;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FText Description;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FString ItemType;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 BasePrice = 10;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Weight = 1;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 MaxStack = 1;
};

USTRUCT(BlueprintType)
struct FNPCConfigRow : public FTableRowBase
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName NPCID;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FText NPCName;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FString Role;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 BaseStrength = 10;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 BaseIntelligence = 10;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FString DefaultFaction;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FString SpawnLocation;
};

USTRUCT(BlueprintType)
struct FRegionConfigRow : public FTableRowBase
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName RegionID;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FText RegionName;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FString Climate;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 BasePopulation = 100000;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) float BaseWealth = 1.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) float BaseStability = 50.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) TArray<FString> Resources;
};

/**
 * 数据驱动配置系统 - 从DataTable/JSON加载所有数值配置
 * 方便Mod制作和数值平衡调整
 */
UCLASS()
class SHANHEERA_API UDataConfigSystem : public UObject
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintCallable, Category="山河纪元|配置")
    void LoadAllConfigs();

    UFUNCTION(BlueprintCallable, Category="山河纪元|配置")
    bool LoadItemConfig(const FString& TablePath);

    UFUNCTION(BlueprintCallable, Category="山河纪元|配置")
    bool LoadNPCConfig(const FString& TablePath);

    UFUNCTION(BlueprintCallable, Category="山河纪元|配置")
    bool LoadRegionConfig(const FString& TablePath);

    UFUNCTION(BlueprintCallable, Category="山河纪元|配置")
    FItemConfigRow GetItemConfig(FName ItemID) const;

    UFUNCTION(BlueprintCallable, Category="山河纪元|配置")
    FNPCConfigRow GetNPCConfig(FName NPCID) const;

    UFUNCTION(BlueprintCallable, Category="山河纪元|配置")
    FRegionConfigRow GetRegionConfig(FName RegionID) const;

    UFUNCTION(BlueprintCallable, Category="山河纪元|配置")
    TArray<FItemConfigRow> GetAllItems() const;

    UFUNCTION(BlueprintCallable, Category="山河纪元|配置")
    bool ExportConfigToJSON(const FString& ExportPath);

    UFUNCTION(BlueprintCallable, Category="山河纪元|配置")
    bool ImportConfigFromJSON(const FString& ImportPath);

    UPROPERTY(BlueprintReadOnly, Category="山河纪元|配置")
    TObjectPtr<UDataTable> ItemConfigTable = nullptr;

    UPROPERTY(BlueprintReadOnly, Category="山河纪元|配置")
    TObjectPtr<UDataTable> NPCConfigTable = nullptr;

    UPROPERTY(BlueprintReadOnly, Category="山河纪元|配置")
    TObjectPtr<UDataTable> RegionConfigTable = nullptr;

    UPROPERTY(BlueprintReadOnly, Category="山河纪元|配置")
    bool bConfigsLoaded = false;
};
