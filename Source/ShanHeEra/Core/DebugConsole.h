#pragma once
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ShanHeDebugConsole.generated.h"

USTRUCT(BlueprintType)
struct FDebugCommand
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName Command;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FText Description;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FText Usage;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) bool bCheat = false;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnConsoleOutput, FString, Output);

/**
 * 调试控制台 - 世界状态查看器、数值调试、快速跳转、开发者工具
 */
UCLASS()
class SHANHEERA_API UDebugConsole : public UObject
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintCallable, Category="山河纪元|调试")
    void InitializeConsole();

    UFUNCTION(BlueprintCallable, Category="山河纪元|调试")
    FString ExecuteCommand(const FString& Command);

    UFUNCTION(BlueprintCallable, Category="山河纪元|调试")
    TArray<FDebugCommand> GetAllCommands() const { return Commands; }

    UFUNCTION(BlueprintCallable, Category="山河纪元|调试")
    FString GetWorldStatus();

    UFUNCTION(BlueprintCallable, Category="山河纪元|调试")
    void ToggleGodMode() { bGodMode = !bGodMode; }

    UFUNCTION(BlueprintCallable, Category="山河纪元|调试")
    void AddSilver(int32 Amount);

    UFUNCTION(BlueprintCallable, Category="山河纪元|调试")
    void SetTimeScale(float Scale);

    UPROPERTY(BlueprintAssignable, Category="山河纪元|调试")
    FOnConsoleOutput OnOutput;

    UPROPERTY(BlueprintReadOnly, Category="山河纪元|调试")
    TArray<FDebugCommand> Commands;

    UPROPERTY(BlueprintReadOnly, Category="山河纪元|调试")
    bool bGodMode = false;

    UPROPERTY(BlueprintReadOnly, Category="山河纪元|调试")
    bool bShowDebugOverlay = false;

private:
    FString ProcessHelp();
    FString ProcessStatus();
    FString ProcessTeleport(const FString& Args);
};
