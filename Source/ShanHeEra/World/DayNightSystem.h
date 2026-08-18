#pragma once
#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "DayNightSystem.generated.h"

UENUM(BlueprintType)
enum class ETimeOfDay : uint8
{
    Dawn        UMETA(DisplayName="黎明"),
    Morning     UMETA(DisplayName="上午"),
    Noon        UMETA(DisplayName="正午"),
    Afternoon   UMETA(DisplayName="下午"),
    Dusk        UMETA(DisplayName="黄昏"),
    Night       UMETA(DisplayName="夜晚"),
    Midnight    UMETA(DisplayName="午夜")
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTimeOfDayChanged, ETimeOfDay, NewTime);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHourChanged, int32, NewHour);

/**
 * 昼夜循环+动态光照系统
 * 日出日落、月光、火把、Lumen全局光照、体积雾、星空
 */
UCLASS()
class SHANHEERA_API UDayNightSystem : public UWorldSubsystem, public FTickableGameObject
{
    GENERATED_BODY()
public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Tick(float DeltaTime) override;
    virtual bool IsTickable() const override { return true; }
    virtual bool IsTickableInEditor() const override { return false; }
    virtual TStatId GetStatId() const override { RETURN_QUICK_DECLARE_CYCLE_STAT(UDayNightSystem, STATGROUP_Tickables); }

    UFUNCTION(BlueprintCallable, Category="山河纪元|昼夜")
    void SetTimeOfDay(float InHour); // 0-24

    UFUNCTION(BlueprintCallable, Category="山河纪元|昼夜")
    float GetCurrentHour() const { return CurrentHour; }

    UFUNCTION(BlueprintCallable, Category="山河纪元|昼夜")
    ETimeOfDay GetTimeOfDay() const;

    UFUNCTION(BlueprintCallable, Category="山河纪元|昼夜")
    float GetSunIntensity() const;

    UFUNCTION(BlueprintCallable, Category="山河纪元|昼夜")
    float GetMoonIntensity() const;

    UFUNCTION(BlueprintCallable, Category="山河纪元|昼夜")
    FLinearColor GetAmbientColor() const;

    UFUNCTION(BlueprintCallable, Category="山河纪元|昼夜")
    float GetFogDensity() const;

    UFUNCTION(BlueprintCallable, Category="山河纪元|昼夜")
    bool IsNight() const;

    UPROPERTY(BlueprintAssignable, Category="山河纪元|昼夜")
    FOnTimeOfDayChanged OnTimeOfDayChanged;

    UPROPERTY(BlueprintAssignable, Category="山河纪元|昼夜")
    FOnHourChanged OnHourChanged;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="山河纪元|昼夜")
    float DayLengthInMinutes = 30.0f; // 游戏内一天 = 现实30分钟

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="山河纪元|昼夜")
    float TimeScale = 1.0f;

    UPROPERTY(BlueprintReadOnly, Category="山河纪元|昼夜")
    float CurrentHour = 8.0f; // 默认早上8点

    UPROPERTY(BlueprintReadOnly, Category="山河纪元|昼夜")
    int32 CurrentDay = 1;

    UPROPERTY(EditAnywhere, Category="山河纪元|昼夜|光照")
    float DawnHour = 5.0f;
    UPROPERTY(EditAnywhere, Category="山河纪元|昼夜|光照")
    float SunriseHour = 6.5f;
    UPROPERTY(EditAnywhere, Category="山河纪元|昼夜|光照")
    float SunsetHour = 18.5f;
    UPROPERTY(EditAnywhere, Category="山河纪元|昼夜|光照")
    float DuskHour = 20.0f;

private:
    ETimeOfDay LastTimeOfDay = ETimeOfDay::Morning;
    int32 LastHour = 8;
    void UpdateLighting();
};
