#pragma once
#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "ShanHeWeatherSystem.generated.h"

UENUM(BlueprintType)
enum class ESolarTerm : uint8
{
    Lichun      UMETA(DisplayName="立春"), Yushui UMETA(DisplayName="雨水"),
    Jingzhe     UMETA(DisplayName="惊蛰"), Chunfen UMETA(DisplayName="春分"),
    Qingming    UMETA(DisplayName="清明"), Guyu UMETA(DisplayName="谷雨"),
    Lixia       UMETA(DisplayName="立夏"), Xiaoman UMETA(DisplayName="小满"),
    Mangzhong   UMETA(DisplayName="芒种"), Xiazhi UMETA(DisplayName="夏至"),
    Xiaoshu     UMETA(DisplayName="小暑"), Dashu UMETA(DisplayName="大暑"),
    Liqiu       UMETA(DisplayName="立秋"), Chushu UMETA(DisplayName="处暑"),
    Bailu       UMETA(DisplayName="白露"), Qiufen UMETA(DisplayName="秋分"),
    Hanlu       UMETA(DisplayName="寒露"), Shuangjiang UMETA(DisplayName="霜降"),
    Lidong      UMETA(DisplayName="立冬"), Xiaoxue UMETA(DisplayName="小雪"),
    Daxue       UMETA(DisplayName="大雪"), Dongzhi UMETA(DisplayName="冬至"),
    Xiaohan     UMETA(DisplayName="小寒"), Dahan UMETA(DisplayName="大寒")
};

UENUM(BlueprintType)
enum class EWeatherType : uint8
{
    Clear       UMETA(DisplayName="晴"),
    Cloudy      UMETA(DisplayName="阴"),
    Rain        UMETA(DisplayName="雨"),
    HeavyRain   UMETA(DisplayName="暴雨"),
    Snow        UMETA(DisplayName="雪"),
    Fog         UMETA(DisplayName="雾"),
    Wind        UMETA(DisplayName="大风"),
    Hail        UMETA(DisplayName="冰雹")
};

USTRUCT(BlueprintType)
struct FWeatherState
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite) EWeatherType CurrentWeather;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) ESolarTerm CurrentTerm;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) float Temperature = 20.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) float Humidity = 50.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) float WindSpeed = 0.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 DaysInTerm = 0;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWeatherChanged, EWeatherType, NewWeather);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSolarTermChanged, ESolarTerm, NewTerm);

/**
 * 24节气天气系统 - 节气影响农业、节日、生活
 * 天气影响出行、战斗、农业产出
 */
UCLASS()
class SHANHEERA_API UWeatherSystem : public UWorldSubsystem
{
    GENERATED_BODY()
public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;

    UFUNCTION(BlueprintCallable, Category="山河纪元|天气")
    void AdvanceDay();

    UFUNCTION(BlueprintCallable, Category="山河纪元|天气")
    ESolarTerm GetSolarTerm(int32 Month, int32 Day) const;

    UFUNCTION(BlueprintCallable, Category="山河纪元|天气")
    float GetAgricultureBonus(ESolarTerm Term) const;

    UFUNCTION(BlueprintCallable, Category="山河纪元|天气")
    FText GetTermDescription(ESolarTerm Term) const;

    UPROPERTY(BlueprintReadOnly, Category="山河纪元|天气")
    FWeatherState State;

    UPROPERTY(BlueprintAssignable, Category="山河纪元|天气")
    FOnWeatherChanged OnWeatherChanged;

    UPROPERTY(BlueprintAssignable, Category="山河纪元|天气")
    FOnSolarTermChanged OnSolarTermChanged;

private:
    void UpdateWeather();
    EWeatherType GetWeatherForTerm(ESolarTerm Term) const;
};
