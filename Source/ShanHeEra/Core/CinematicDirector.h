#pragma once
#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ShanHeCinematicDirector.generated.h"
class UCameraComponent;
class UAudioComponent;
class UMaterialInstanceDynamic;
UENUM(BlueprintType)
enum class EShotSize : uint8
{
    ExtremeWide   UMETA(DisplayName="大远景"),
    Wide          UMETA(DisplayName="远景"),
    MediumWide    UMETA(DisplayName="全景"),
    Medium        UMETA(DisplayName="中景"),
    MediumCloseUp UMETA(DisplayName="近景"),
    CloseUp       UMETA(DisplayName="特写"),
    ExtremeCloseUp UMETA(DisplayName="大特写"),
    Macro         UMETA(DisplayName="微距"),
    OverShoulder  UMETA(DisplayName="过肩镜头")
};
UENUM(BlueprintType)
enum class ECameraAngle : uint8
{
    EyeLevel     UMETA(DisplayName="平视"),
    LowAngle     UMETA(DisplayName="仰拍"),
    HighAngle    UMETA(DisplayName="俯拍"),
    Overhead     UMETA(DisplayName="顶视"),
    Profile      UMETA(DisplayName="侧面"),
    ThreeQuarter UMETA(DisplayName="3/4侧面"),
    DutchAngle   UMETA(DisplayName="倾斜角度")
};
UENUM(BlueprintType)
enum class ECameraMovement : uint8
{
    LockedOff    UMETA(DisplayName="固定镜头"),
    PushIn       UMETA(DisplayName="推镜"),
    PullBack     UMETA(DisplayName="拉镜"),
    LateralTrack UMETA(DisplayName="横移轨道"),
    Orbit        UMETA(DisplayName="环绕运镜"),
    Pan          UMETA(DisplayName="摇镜(水平)"),
    Tilt         UMETA(DisplayName="摇镜(垂直)"),
    Pedestal     UMETA(DisplayName="升降镜"),
    Crane        UMETA(DisplayName="摇臂/航拍"),
    Handheld     UMETA(DisplayName="手持感"),
    RackFocus    UMETA(DisplayName="移焦"),
    WhipPan      UMETA(DisplayName="甩镜转场"),
    DollyZoom    UMETA(DisplayName="希区柯克变焦"),
    Breathing    UMETA(DisplayName="呼吸感镜头")
};
UENUM(BlueprintType)
enum class ECameraSupport : uint8
{
    LockedOff   UMETA(DisplayName="固定脚架"),
    Handheld    UMETA(DisplayName="手持"),
    Slider      UMETA(DisplayName="滑轨"),
    Dolly       UMETA(DisplayName="轨道车"),
    Crane       UMETA(DisplayName="摇臂"),
    Drone       UMETA(DisplayName="无人机"),
    Gimbal      UMETA(DisplayName="稳定器"),
    Steadicam   UMETA(DisplayName="斯坦尼康")
};
UENUM(BlueprintType)
enum class EEndingProfile : uint8
{
    Resolve         UMETA(DisplayName="收束"),
    ExtensionAnchor UMETA(DisplayName="延续锚点"),
    LoopSeam        UMETA(DisplayName="循环接缝"),
    HeroHold        UMETA(DisplayName="主体定格"),
    EditPoint       UMETA(DisplayName="剪辑点"),
    RevealOrPunch   UMETA(DisplayName="揭示/冲击")
};
UENUM(BlueprintType)
enum class ETransitionType : uint8
{
    HardCut     UMETA(DisplayName="硬切"),
    Dissolve    UMETA(DisplayName="叠化"),
    WhipPan     UMETA(DisplayName="甩镜转场"),
    FadeBlack   UMETA(DisplayName="淡入淡出黑场"),
    FadeWhite   UMETA(DisplayName="淡入淡出白场"),
    MatchCut    UMETA(DisplayName="匹配剪辑"),
    Iris        UMETA(DisplayName="圈入圈出")
};
UENUM(BlueprintType)
enum class ELightQuality : uint8
{
    HardLight    UMETA(DisplayName="硬光"),
    SoftLight    UMETA(DisplayName="柔光"),
    Rembrandt    UMETA(DisplayName="伦勃朗光"),
    RimLight     UMETA(DisplayName="轮廓光/逆光"),
    LowKey       UMETA(DisplayName="低调光"),
    HighKey      UMETA(DisplayName="高调光"),
    CandleLight  UMETA(DisplayName="烛光"),
    Moonlight    UMETA(DisplayName="月光")
};
UENUM(BlueprintType)
enum class EColorGrade : uint8
{
    None            UMETA(DisplayName="无"),
    WarmGolden      UMETA(DisplayName="暖金盛世"),
    Desaturated     UMETA(DisplayName="低饱和怀古"),
    ColdBlue        UMETA(DisplayName="冷蓝肃杀"),
    Sepia           UMETA(DisplayName="棕褐怀旧"),
    HighContrast    UMETA(DisplayName="高对比史诗"),
    TealAndOrange   UMETA(DisplayName="青橙大片"),
    InkWash         UMETA(DisplayName="水墨国风"),
    FestivalRed     UMETA(DisplayName="节庆红"),
    NightNeon       UMETA(DisplayName="夜色霓虹")
};
USTRUCT(BlueprintType)
struct FShotContract
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName ShotID;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) float Duration = 4.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) EShotSize ShotSize = EShotSize::Medium;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) ECameraAngle Angle = ECameraAngle::ThreeQuarter;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) ECameraSupport Support = ECameraSupport::Gimbal;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) ECameraMovement Movement = ECameraMovement::LockedOff;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) EEndingProfile Ending = EEndingProfile::Resolve;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) ETransitionType TransitionOut = ETransitionType::HardCut;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) float FocalLength = 35.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) float Aperture = 2.8f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) float MovementSpeed = 1.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) float DutchAngle = 0.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) ELightQuality LightQuality = ELightQuality::SoftLight;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) EColorGrade ColorGrade = EColorGrade::None;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FVector StartLocation = FVector::ZeroVector;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FRotator StartRotation = FRotator::ZeroRotator;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FVector EndLocation = FVector::ZeroVector;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FRotator EndRotation = FRotator::ZeroRotator;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName TargetActorTag;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName MusicTrack;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName SFXID;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FText Subtitle;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) float SubtitleStartTime = 0.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) float SubtitleDuration = 0.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName EffectID;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) float EffectTriggerTime = 0.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) bool bCameraShake = false;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) float ShakeIntensity = 0.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) float SlowMotionFactor = 1.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) TArray<FName> FragileAnchors;
};
USTRUCT(BlueprintType)
struct FDirectorRead
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FText DramaticFunction;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FText Turn;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FText POV;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FText PowerShift;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FText HiddenWant;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FText ObstacleTactic;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FText Subtext;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FText SuppressedBehavior;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FText NonTransferableDetail;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FText StockRefused;
};
USTRUCT(BlueprintType)
struct FCinematicSequence
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName SequenceID;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FText Title;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FText Description;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FDirectorRead DirectorRead;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) TArray<FShotContract> Shots;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) bool bIsSkippable = true;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) float TotalDuration = 0.0f;
};
UCLASS()
class SHANHEERA_API UCinematicDirector : public UGameInstanceSubsystem
{
    GENERATED_BODY()
public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;
    UFUNCTION(BlueprintCallable, Category="山河纪元|导演|序列")
    void RegisterSequence(const FCinematicSequence& Sequence);
    UFUNCTION(BlueprintCallable, Category="山河纪元|导演|序列")
    void PlaySequence(FName SequenceID);
    UFUNCTION(BlueprintCallable, Category="山河纪元|导演|序列")
    void StopSequence();
    UFUNCTION(BlueprintCallable, Category="山河纪元|导演|序列")
    void SkipCurrentShot();
    UFUNCTION(BlueprintCallable, Category="山河纪元|导演|序列")
    bool IsPlaying() const { return bIsPlaying; }
    UFUNCTION(BlueprintCallable, Category="山河纪元|导演|序列")
    FCinematicSequence GetCurrentSequence() const { return CurrentSequence; }
    UFUNCTION(BlueprintCallable, Category="山河纪元|导演|序列")
    int32 GetCurrentShotIndex() const { return CurrentShotIndex; }
    UFUNCTION(BlueprintCallable, Category="山河纪元|导演|镜头")
    void PlayShot(const FShotContract& Shot);
    UFUNCTION(BlueprintCallable, Category="山河纪元|导演|镜头")
    FShotContract GetCurrentShot() const { return CurrentShot; }
    UFUNCTION(BlueprintCallable, Category="山河纪元|导演|色彩")
    void ApplyColorGrade(EColorGrade Grade, float TransitionTime = 1.0f);
    UFUNCTION(BlueprintCallable, Category="山河纪元|导演|色彩")
    EColorGrade GetCurrentColorGrade() const { return CurrentColorGrade; }
    UFUNCTION(BlueprintCallable, Category="山河纪元|导演|镜头")
    void TriggerCameraShake(float Intensity, float Duration);
    UFUNCTION(BlueprintCallable, Category="山河纪元|导演|镜头")
    void SetSlowMotion(float Factor, float Duration);
    UFUNCTION(BlueprintCallable, Category="山河纪元|导演|预设")
    void PlayPresetSequence(FName PresetID);
    UPROPERTY(BlueprintReadOnly, Category="山河纪元|导演")
    TMap<FName, FCinematicSequence> SequenceLibrary;
    UPROPERTY(BlueprintReadOnly, Category="山河纪元|导演")
    float PlaybackTime = 0.0f;
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnShotStarted, const FShotContract&, Shot);
    UPROPERTY(BlueprintAssignable, Category="山河纪元|导演|事件")
    FOnShotStarted OnShotStarted;
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSequenceFinished, FName, SequenceID);
    UPROPERTY(BlueprintAssignable, Category="山河纪元|导演|事件")
    FOnSequenceFinished OnSequenceFinished;
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSubtitleEvent, const FText&, Text, float, Duration);
    UPROPERTY(BlueprintAssignable, Category="山河纪元|导演|事件")
    FOnSubtitleEvent OnSubtitleEvent;
private:
    bool bIsPlaying = false;
    FCinematicSequence CurrentSequence;
    FShotContract CurrentShot;
    int32 CurrentShotIndex = 0;
    float CurrentShotTime = 0.0f;
    EColorGrade CurrentColorGrade = EColorGrade::None;
    bool bShotEffectsTriggered = false;
    void TickPlayback(float DeltaTime);
    void AdvanceToShot(int32 ShotIndex);
    void ApplyShotCamera(const FShotContract& Shot);
    void ApplyShotAudio(const FShotContract& Shot);
    void ApplyShotEffects(const FShotContract& Shot);
    void ApplyShotLighting(const FShotContract& Shot);
    void RegisterPresetSequences();
    TMap<EColorGrade, FString> ColorGradeLUTs;
};
