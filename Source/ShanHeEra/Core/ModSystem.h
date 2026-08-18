#pragma once
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ModSystem.generated.h"

USTRUCT(BlueprintType)
struct FModInfo
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName ModID;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FText ModName;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FText Author;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FText Version;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FText Description;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FString ScriptPath;  // Lua/Python脚本路径
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FString ContentPath; // 资源路径
    UPROPERTY(EditAnywhere, BlueprintReadWrite) bool bIsEnabled = false;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) bool bIsLoaded = false;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 LoadOrder = 0;
};

/**
 * Mod支持系统 - Lua/Python脚本接口、数据驱动配置、Mod加载器
 */
UCLASS()
class SHANHEERA_API UModSystem : public UObject
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintCallable, Category="山河纪元|Mod")
    void ScanMods();

    UFUNCTION(BlueprintCallable, Category="山河纪元|Mod")
    bool EnableMod(FName ModID);

    UFUNCTION(BlueprintCallable, Category="山河纪元|Mod")
    void DisableMod(FName ModID);

    UFUNCTION(BlueprintCallable, Category="山河纪元|Mod")
    void LoadAllEnabledMods();

    UFUNCTION(BlueprintCallable, Category="山河纪元|Mod")
    void UnloadAllMods();

    UFUNCTION(BlueprintCallable, Category="山河纪元|Mod")
    TArray<FModInfo> GetAllMods() const { return InstalledMods; }

    UFUNCTION(BlueprintCallable, Category="山河纪元|Mod")
    bool ExecuteModScript(FName ModID, const FString& FunctionName);

    UPROPERTY(BlueprintReadOnly, Category="山河纪元|Mod")
    TArray<FModInfo> InstalledMods;

    UPROPERTY(BlueprintReadOnly, Category="山河纪元|Mod")
    FString ModDirectory;
};
