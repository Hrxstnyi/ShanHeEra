#include "Core/CinematicDirector.h"
#include "Core/ShanHeLog.h"
#include "Audio/AudioManager.h"
#include "Audio/CGManager.h"
#include "Core/EffectSystem.h"
void UCinematicDirector::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
    ColorGradeLUTs.Add(EColorGrade::WarmGolden, TEXT("/Game/ColorGrading/LUT_WarmGolden"));
    ColorGradeLUTs.Add(EColorGrade::Desaturated, TEXT("/Game/ColorGrading/LUT_Desaturated"));
    ColorGradeLUTs.Add(EColorGrade::ColdBlue, TEXT("/Game/ColorGrading/LUT_ColdBlue"));
    ColorGradeLUTs.Add(EColorGrade::Sepia, TEXT("/Game/ColorGrading/LUT_Sepia"));
    ColorGradeLUTs.Add(EColorGrade::HighContrast, TEXT("/Game/ColorGrading/LUT_HighContrast"));
    ColorGradeLUTs.Add(EColorGrade::TealAndOrange, TEXT("/Game/ColorGrading/LUT_TealOrange"));
    ColorGradeLUTs.Add(EColorGrade::InkWash, TEXT("/Game/ColorGrading/LUT_InkWash"));
    ColorGradeLUTs.Add(EColorGrade::FestivalRed, TEXT("/Game/ColorGrading/LUT_FestivalRed"));
    ColorGradeLUTs.Add(EColorGrade::NightNeon, TEXT("/Game/ColorGrading/LUT_NightNeon"));
    RegisterPresetSequences();
    UE_LOG(LogShanHe, Log, TEXT("电影导演系统初始化 - Seedance导演方法论已加载，%d个预设分镜序列，%d种色彩分级LUT"),
        SequenceLibrary.Num(), ColorGradeLUTs.Num());
}
void UCinematicDirector::Deinitialize()
{
    StopSequence();
    Super::Deinitialize();
}
void UCinematicDirector::RegisterSequence(const FCinematicSequence& Sequence)
{
    SequenceLibrary.Add(Sequence.SequenceID, Sequence);
}
void UCinematicDirector::PlaySequence(FName SequenceID)
{
    if (const FCinematicSequence* Found = SequenceLibrary.Find(SequenceID))
    {
        CurrentSequence = *Found;
        CurrentShotIndex = 0;
        CurrentShotTime = 0.0f;
        PlaybackTime = 0.0f;
        bIsPlaying = true;
        bShotEffectsTriggered = false;
        if (CurrentSequence.Shots.Num() > 0)
        {
            AdvanceToShot(0);
        }
    }
}
void UCinematicDirector::StopSequence()
{
    if (bIsPlaying)
    {
        bIsPlaying = false;
        FName FinishedID = CurrentSequence.SequenceID;
        OnSequenceFinished.Broadcast(FinishedID);
    }
}
void UCinematicDirector::SkipCurrentShot()
{
    if (!bIsPlaying) return;
    if (CurrentShotIndex + 1 < CurrentSequence.Shots.Num())
    {
        AdvanceToShot(CurrentShotIndex + 1);
    }
    else
    {
        StopSequence();
    }
}
void UCinematicDirector::PlayShot(const FShotContract& Shot)
{
    CurrentShot = Shot;
    CurrentShotTime = 0.0f;
    bShotEffectsTriggered = false;
    bIsPlaying = true;
    ApplyShotCamera(Shot);
    ApplyShotAudio(Shot);
    ApplyShotLighting(Shot);
    OnShotStarted.Broadcast(Shot);
}
void UCinematicDirector::AdvanceToShot(int32 ShotIndex)
{
    if (ShotIndex < 0 || ShotIndex >= CurrentSequence.Shots.Num())
    {
        StopSequence();
        return;
    }
    CurrentShotIndex = ShotIndex;
    CurrentShot = CurrentSequence.Shots[ShotIndex];
    CurrentShotTime = 0.0f;
    bShotEffectsTriggered = false;
    ApplyShotCamera(CurrentShot);
    ApplyShotAudio(CurrentShot);
    ApplyShotLighting(CurrentShot);
    OnShotStarted.Broadcast(CurrentShot);
}
void UCinematicDirector::TickPlayback(float DeltaTime)
{
    if (!bIsPlaying) return;
    PlaybackTime += DeltaTime;
    CurrentShotTime += DeltaTime;
    if (!bShotEffectsTriggered && CurrentShotTime >= CurrentShot.EffectTriggerTime)
    {
        ApplyShotEffects(CurrentShot);
        bShotEffectsTriggered = true;
    }
    if (!CurrentShot.Subtitle.IsEmpty() &&
        CurrentShotTime >= CurrentShot.SubtitleStartTime &&
        CurrentShotTime < CurrentShot.SubtitleStartTime + CurrentShot.SubtitleDuration)
    {
        static float LastSubtitleTime = -1.0f;
        if (FMath::Abs(LastSubtitleTime - CurrentShotTime) > 0.1f)
        {
            OnSubtitleEvent.Broadcast(CurrentShot.Subtitle, CurrentShot.SubtitleDuration);
            LastSubtitleTime = CurrentShotTime;
        }
    }
    if (CurrentShotTime >= CurrentShot.Duration)
    {
        if (CurrentShotIndex + 1 < CurrentSequence.Shots.Num())
        {
            AdvanceToShot(CurrentShotIndex + 1);
        }
        else
        {
            StopSequence();
        }
    }
}
void UCinematicDirector::ApplyShotCamera(const FShotContract& Shot)
{
    if (Shot.bCameraShake)
    {
        TriggerCameraShake(Shot.ShakeIntensity, Shot.Duration * 0.3f);
    }
    if (Shot.SlowMotionFactor < 1.0f)
    {
        SetSlowMotion(Shot.SlowMotionFactor, Shot.Duration);
    }
}
void UCinematicDirector::ApplyShotAudio(const FShotContract& Shot)
{
    if (UWorld* World = GetWorld())
    {
        if (UAudioManager* AudioMgr = World->GetGameInstance()->GetSubsystem<UAudioManager>())
        {
            if (!Shot.MusicTrack.IsNone())
            {
                AudioMgr->PlayBackgroundMusic(Shot.MusicTrack);
            }
        }
    }
}
void UCinematicDirector::ApplyShotEffects(const FShotContract& Shot)
{
    if (!Shot.EffectID.IsNone())
    {
    }
}
void UCinematicDirector::ApplyShotLighting(const FShotContract& Shot)
{
    if (Shot.ColorGrade != EColorGrade::None)
    {
        ApplyColorGrade(Shot.ColorGrade, 0.5f);
    }
}
void UCinematicDirector::ApplyColorGrade(EColorGrade Grade, float TransitionTime)
{
    CurrentColorGrade = Grade;
}
void UCinematicDirector::TriggerCameraShake(float Intensity, float Duration)
{
}
void UCinematicDirector::SetSlowMotion(float Factor, float Duration)
{
}
void UCinematicDirector::PlayPresetSequence(FName PresetID)
{
    PlaySequence(PresetID);
}
void UCinematicDirector::RegisterPresetSequences()
{
    // 预设1：开国大典
    {
        FCinematicSequence Founding;
        Founding.SequenceID = TEXT("CINE_Founding");
        Founding.Title = NSLOCTEXT("Cinematic", "Founding", "开国大典");
        Founding.Description = NSLOCTEXT("Cinematic", "FoundingDesc", "王朝建立，天下初定，万民朝拜");
        Founding.DirectorRead.DramaticFunction = NSLOCTEXT("Cinematic", "FoundingDF", "收束：从战乱到统一的历史性落点");
        Founding.DirectorRead.Turn = NSLOCTEXT("Cinematic", "FoundingTurn", "从废墟残阳到金殿晨光，从破碎到一统");
        Founding.DirectorRead.POV = NSLOCTEXT("Cinematic", "FoundingPOV", "天下百姓的集体视角，从仰望到臣服");
        Founding.DirectorRead.PowerShift = NSLOCTEXT("Cinematic", "FoundingPS", "开始：群雄割据无人掌控；结束：新帝一人定鼎天下");
        Founding.DirectorRead.HiddenWant = NSLOCTEXT("Cinematic", "FoundingHW", "新帝欲证明自己的正统性，让天下信服");
        Founding.DirectorRead.ObstacleTactic = NSLOCTEXT("Cinematic", "FoundingOT", "障碍：战乱遗留的怀疑与破碎；策略：以盛大仪式和万民朝拜来确立权威");
        Founding.DirectorRead.Subtext = NSLOCTEXT("Cinematic", "FoundingST", "表面是庆典，底层是权力的确立和秩序的重建");
        Founding.DirectorRead.SuppressedBehavior = NSLOCTEXT("Cinematic", "FoundingSB", "新帝紧握玉玺的手指微微发白，随即松开接受朝拜");
        Founding.DirectorRead.NonTransferableDetail = NSLOCTEXT("Cinematic", "FoundingNTD", "传国玉玺上的残缺角，以金镶玉修补——这是这个王朝独有的信物");
        Founding.DirectorRead.StockRefused = NSLOCTEXT("Cinematic", "FoundingSR", "拒绝：慢镜头+金光四射+万人欢呼的俗套开国；替代：以玉玺细节和沉默的群臣建立重量感");
        FShotContract Shot1;
        Shot1.ShotID = TEXT("Founding_01_WideReveal");
        Shot1.Duration = 5.0f;
        Shot1.ShotSize = EShotSize::ExtremeWide;
        Shot1.Angle = ECameraAngle::HighAngle;
        Shot1.Support = ECameraSupport::Crane;
        Shot1.Movement = ECameraMovement::PullBack;
        Shot1.Ending = EEndingProfile::ExtensionAnchor;
        Shot1.TransitionOut = EShanHeTransitionType::Dissolve;
        Shot1.FocalLength = 24.0f;
        Shot1.Aperture = 8.0f;
        Shot1.MovementSpeed = 0.5f;
        Shot1.LightQuality = ELightQuality::RimLight;
        Shot1.ColorGrade = EColorGrade::WarmGolden;
        Shot1.MusicTrack = TEXT("EpicTheme");
        Shot1.Subtitle = NSLOCTEXT("Cinematic", "FoundingS1", "山河破碎，百年战乱终");
        Shot1.SubtitleStartTime = 1.0f;
        Shot1.SubtitleDuration = 3.0f;
        Founding.Shots.Add(Shot1);
        FShotContract Shot2;
        Shot2.ShotID = TEXT("Founding_02_CourtPushIn");
        Shot2.Duration = 4.0f;
        Shot2.ShotSize = EShotSize::Medium;
        Shot2.Angle = ECameraAngle::EyeLevel;
        Shot2.Support = ECameraSupport::Dolly;
        Shot2.Movement = ECameraMovement::PushIn;
        Shot2.Ending = EEndingProfile::ExtensionAnchor;
        Shot2.TransitionOut = EShanHeTransitionType::HardCut;
        Shot2.FocalLength = 35.0f;
        Shot2.Aperture = 4.0f;
        Shot2.LightQuality = ELightQuality::Rembrandt;
        Shot2.ColorGrade = EColorGrade::WarmGolden;
        Founding.Shots.Add(Shot2);
        FShotContract Shot3;
        Shot3.ShotID = TEXT("Founding_03_SealMacro");
        Shot3.Duration = 3.5f;
        Shot3.ShotSize = EShotSize::Macro;
        Shot3.Angle = ECameraAngle::Overhead;
        Shot3.Support = ECameraSupport::LockedOff;
        Shot3.Movement = ECameraMovement::Breathing;
        Shot3.Ending = EEndingProfile::HeroHold;
        Shot3.TransitionOut = EShanHeTransitionType::Dissolve;
        Shot3.FocalLength = 100.0f;
        Shot3.Aperture = 2.8f;
        Shot3.LightQuality = ELightQuality::CandleLight;
        Shot3.ColorGrade = EColorGrade::Sepia;
        Shot3.SFXID = TEXT("Seal_Stamp");
        Shot3.EffectID = TEXT("Seal_InkSpread");
        Shot3.EffectTriggerTime = 1.5f;
        Shot3.FragileAnchors = { TEXT("传国玉玺"), TEXT("金镶玉缺角") };
        Founding.Shots.Add(Shot3);
        FShotContract Shot4;
        Shot4.ShotID = TEXT("Founding_04_EmperorCloseUp");
        Shot4.Duration = 4.0f;
        Shot4.ShotSize = EShotSize::CloseUp;
        Shot4.Angle = ECameraAngle::LowAngle;
        Shot4.Support = ECameraSupport::LockedOff;
        Shot4.Movement = ECameraMovement::PushIn;
        Shot4.Ending = EEndingProfile::Resolve;
        Shot4.TransitionOut = EShanHeTransitionType::FadeBlack;
        Shot4.FocalLength = 50.0f;
        Shot4.Aperture = 2.0f;
        Shot4.MovementSpeed = 0.3f;
        Shot4.LightQuality = ELightQuality::Rembrandt;
        Shot4.ColorGrade = EColorGrade::HighContrast;
        Shot4.Subtitle = NSLOCTEXT("Cinematic", "FoundingS4", "新纪元，始");
        Shot4.SubtitleStartTime = 2.0f;
        Shot4.SubtitleDuration = 2.0f;
        Founding.Shots.Add(Shot4);
        Founding.TotalDuration = 16.5f;
        RegisterSequence(Founding);
    }
    // 预设2：烽火连天
    {
        FCinematicSequence War;
        War.SequenceID = TEXT("CINE_War");
        War.Title = NSLOCTEXT("Cinematic", "War", "烽火连天");
        War.Description = NSLOCTEXT("Cinematic", "WarDesc", "战争爆发，山河破碎，生灵涂炭");
        War.DirectorRead.DramaticFunction = NSLOCTEXT("Cinematic", "WarDF", "转折：太平被打破，进入战乱");
        War.DirectorRead.Turn = NSLOCTEXT("Cinematic", "WarTurn", "从宁静村庄到烽火战场，从安宁到恐惧");
        War.DirectorRead.POV = NSLOCTEXT("Cinematic", "WarPOV", "普通士兵的视角，从恐惧到麻木");
        War.DirectorRead.PowerShift = NSLOCTEXT("Cinematic", "WarPS", "开始：平民掌控自己的生活；结束：战争掌控所有人的命运");
        War.DirectorRead.HiddenWant = NSLOCTEXT("Cinematic", "WarHW", "每个人只想活下去，回家");
        War.DirectorRead.ObstacleTactic = NSLOCTEXT("Cinematic", "WarOT", "障碍：敌军的进攻和混乱；策略：本能的求生和战斗");
        War.DirectorRead.Subtext = NSLOCTEXT("Cinematic", "WarST", "表面是两军交战，底层是个体在历史洪流中的无力");
        War.DirectorRead.SuppressedBehavior = NSLOCTEXT("Cinematic", "WarSB", "年轻士兵的手在颤抖，他紧紧握住长矛不让别人看见");
        War.DirectorRead.NonTransferableDetail = NSLOCTEXT("Cinematic", "WarNTD", "士兵腰间系着的半块平安符，是临行前母亲塞给他的");
        War.DirectorRead.StockRefused = NSLOCTEXT("Cinematic", "WarSR", "拒绝：无脑冲锋+慢动作鲜血飞溅的战争俗套；替代：以个体细节和环境声音建立战争的真实重量");
        FShotContract WShot1;
        WShot1.ShotID = TEXT("War_01_BattlefieldAerial");
        WShot1.Duration = 4.5f;
        WShot1.ShotSize = EShotSize::ExtremeWide;
        WShot1.Angle = ECameraAngle::Overhead;
        WShot1.Support = ECameraSupport::Drone;
        WShot1.Movement = ECameraMovement::Crane;
        WShot1.Ending = EEndingProfile::ExtensionAnchor;
        WShot1.TransitionOut = EShanHeTransitionType::WhipPan;
        WShot1.FocalLength = 24.0f;
        WShot1.Aperture = 8.0f;
        WShot1.LightQuality = ELightQuality::HardLight;
        WShot1.ColorGrade = EColorGrade::ColdBlue;
        WShot1.MusicTrack = TEXT("WarTheme");
        WShot1.EffectID = TEXT("Smoke_Battle");
        WShot1.EffectTriggerTime = 0.5f;
        War.Shots.Add(WShot1);
        FShotContract WShot2;
        WShot2.ShotID = TEXT("War_02_ChargeHandheld");
        WShot2.Duration = 3.0f;
        WShot2.ShotSize = EShotSize::Medium;
        WShot2.Angle = ECameraAngle::EyeLevel;
        WShot2.Support = ECameraSupport::Handheld;
        WShot2.Movement = ECameraMovement::LateralTrack;
        WShot2.Ending = EEndingProfile::ExtensionAnchor;
        WShot2.TransitionOut = EShanHeTransitionType::HardCut;
        WShot2.FocalLength = 35.0f;
        WShot2.Aperture = 4.0f;
        WShot2.MovementSpeed = 2.0f;
        WShot2.LightQuality = ELightQuality::HardLight;
        WShot2.ColorGrade = EColorGrade::Desaturated;
        WShot2.bCameraShake = true;
        WShot2.ShakeIntensity = 0.5f;
        War.Shots.Add(WShot2);
        FShotContract WShot3;
        WShot3.ShotID = TEXT("War_03_TremblingHand");
        WShot3.Duration = 3.5f;
        WShot3.ShotSize = EShotSize::CloseUp;
        WShot3.Angle = ECameraAngle::HighAngle;
        WShot3.Support = ECameraSupport::LockedOff;
        WShot3.Movement = ECameraMovement::Breathing;
        WShot3.Ending = EEndingProfile::HeroHold;
        WShot3.TransitionOut = EShanHeTransitionType::Dissolve;
        WShot3.FocalLength = 85.0f;
        WShot3.Aperture = 1.8f;
        WShot3.LightQuality = ELightQuality::SoftLight;
        WShot3.ColorGrade = EColorGrade::Desaturated;
        WShot3.SlowMotionFactor = 0.3f;
        WShot3.FragileAnchors = { TEXT("半块平安符"), TEXT("颤抖的手") };
        War.Shots.Add(WShot3);
        FShotContract WShot4;
        WShot4.ShotID = TEXT("War_04_BeaconPullBack");
        WShot4.Duration = 4.0f;
        WShot4.ShotSize = EShotSize::Wide;
        WShot4.Angle = ECameraAngle::LowAngle;
        WShot4.Support = ECameraSupport::Crane;
        WShot4.Movement = ECameraMovement::PullBack;
        WShot4.Ending = EEndingProfile::Resolve;
        WShot4.TransitionOut = EShanHeTransitionType::FadeBlack;
        WShot4.FocalLength = 35.0f;
        WShot4.Aperture = 5.6f;
        WShot4.MovementSpeed = 0.6f;
        WShot4.LightQuality = ELightQuality::RimLight;
        WShot4.ColorGrade = EColorGrade::ColdBlue;
        WShot4.EffectID = TEXT("Fire_Large");
        WShot4.EffectTriggerTime = 0.0f;
        War.Shots.Add(WShot4);
        War.TotalDuration = 15.0f;
        RegisterSequence(War);
    }
    // 预设3：九州一统
    {
        FCinematicSequence Unity;
        Unity.SequenceID = TEXT("CINE_Reunification");
        Unity.Title = NSLOCTEXT("Cinematic", "Unity", "九州一统");
        Unity.Description = NSLOCTEXT("Cinematic", "UnityDesc", "天下归一，四海升平");
        Unity.DirectorRead.DramaticFunction = NSLOCTEXT("Cinematic", "UnityDF", "收束：漫长战争的终点，和平的开始");
        Unity.DirectorRead.Turn = NSLOCTEXT("Cinematic", "UnityTurn", "从兵戈相向到放下武器，从敌对到共融");
        Unity.DirectorRead.POV = NSLOCTEXT("Cinematic", "UnityPOV", "双方士兵的双重视角，从对立到和解");
        Unity.DirectorRead.PowerShift = NSLOCTEXT("Cinematic", "UnityPS", "开始：胜者与败者的对立；结束：所有人共同面对和平");
        Unity.DirectorRead.HiddenWant = NSLOCTEXT("Cinematic", "UnityHW", "双方都渴望结束战争，但无人愿意先示弱");
        Unity.DirectorRead.ObstacleTactic = NSLOCTEXT("Cinematic", "UnityOT", "障碍：积怨和不信任；策略：一方先放下武器，另一方跟随");
        Unity.DirectorRead.Subtext = NSLOCTEXT("Cinematic", "UnityST", "表面是受降仪式，底层是人性中对和平的共同渴望");
        Unity.DirectorRead.SuppressedBehavior = NSLOCTEXT("Cinematic", "UnitySB", "败将低头时，肩膀微微松了一下——不是屈辱，是释然");
        Unity.DirectorRead.NonTransferableDetail = NSLOCTEXT("Cinematic", "UnityNTD", "受降台上那面残破的旧旗，旗角还留着箭孔");
        Unity.DirectorRead.StockRefused = NSLOCTEXT("Cinematic", "UnitySR", "拒绝：胜者高举双手+阳光普照的俗套胜利；替代：以双方沉默和武器落地的声音建立真正的和解感");
        FShotContract UShot1;
        UShot1.ShotID = TEXT("Unity_01_Standoff");
        UShot1.Duration = 4.0f;
        UShot1.ShotSize = EShotSize::Wide;
        UShot1.Angle = ECameraAngle::EyeLevel;
        UShot1.Support = ECameraSupport::LockedOff;
        UShot1.Movement = ECameraMovement::LockedOff;
        UShot1.Ending = EEndingProfile::ExtensionAnchor;
        UShot1.TransitionOut = EShanHeTransitionType::HardCut;
        UShot1.FocalLength = 35.0f;
        UShot1.Aperture = 5.6f;
        UShot1.LightQuality = ELightQuality::SoftLight;
        UShot1.ColorGrade = EColorGrade::Desaturated;
        UShot1.MusicTrack = TEXT("TriumphantTheme");
        Unity.Shots.Add(UShot1);
        FShotContract UShot2;
        UShot2.ShotID = TEXT("Unity_02_DropWeapon");
        UShot2.Duration = 3.5f;
        UShot2.ShotSize = EShotSize::Medium;
        UShot2.Angle = ECameraAngle::ThreeQuarter;
        UShot2.Support = ECameraSupport::Dolly;
        UShot2.Movement = ECameraMovement::PushIn;
        UShot2.Ending = EEndingProfile::ExtensionAnchor;
        UShot2.TransitionOut = EShanHeTransitionType::MatchCut;
        UShot2.FocalLength = 50.0f;
        UShot2.Aperture = 2.8f;
        UShot2.MovementSpeed = 0.8f;
        UShot2.LightQuality = ELightQuality::SoftLight;
        UShot2.ColorGrade = EColorGrade::WarmGolden;
        UShot2.SFXID = TEXT("Weapon_Drop");
        UShot2.EffectTriggerTime = 1.5f;
        Unity.Shots.Add(UShot2);
        FShotContract UShot3;
        UShot3.ShotID = TEXT("Unity_03_Release");
        UShot3.Duration = 3.0f;
        UShot3.ShotSize = EShotSize::CloseUp;
        UShot3.Angle = ECameraAngle::Profile;
        UShot3.Support = ECameraSupport::LockedOff;
        UShot3.Movement = ECameraMovement::Breathing;
        UShot3.Ending = EEndingProfile::HeroHold;
        UShot3.TransitionOut = EShanHeTransitionType::Dissolve;
        UShot3.FocalLength = 85.0f;
        UShot3.Aperture = 2.0f;
        UShot3.LightQuality = ELightQuality::RimLight;
        UShot3.ColorGrade = EColorGrade::WarmGolden;
        Unity.Shots.Add(UShot3);
        FShotContract UShot4;
        UShot4.ShotID = TEXT("Unity_04_MapReveal");
        UShot4.Duration = 5.0f;
        UShot4.ShotSize = EShotSize::ExtremeWide;
        UShot4.Angle = ECameraAngle::Overhead;
        UShot4.Support = ECameraSupport::Crane;
        UShot4.Movement = ECameraMovement::Pedestal;
        UShot4.Ending = EEndingProfile::Resolve;
        UShot4.TransitionOut = EShanHeTransitionType::FadeBlack;
        UShot4.FocalLength = 24.0f;
        UShot4.Aperture = 8.0f;
        UShot4.MovementSpeed = 0.4f;
        UShot4.LightQuality = ELightQuality::HighKey;
        UShot4.ColorGrade = EColorGrade::WarmGolden;
        UShot4.Subtitle = NSLOCTEXT("Cinematic", "UnityS4", "九州一统，四海升平");
        UShot4.SubtitleStartTime = 2.0f;
        UShot4.SubtitleDuration = 3.0f;
        Unity.Shots.Add(UShot4);
        Unity.TotalDuration = 15.5f;
        RegisterSequence(Unity);
    }
    // 预设4：宝岛归心
    {
        FCinematicSequence Taiwan;
        Taiwan.SequenceID = TEXT("CINE_TaiwanReturn");
        Taiwan.Title = NSLOCTEXT("Cinematic", "Taiwan", "宝岛归心");
        Taiwan.Description = NSLOCTEXT("Cinematic", "TaiwanDesc", "海峡两岸，血脉相连，台湾回归华夏版图");
        Taiwan.DirectorRead.DramaticFunction = NSLOCTEXT("Cinematic", "TaiwanDF", "收束：分离后的重聚，血脉的确认");
        Taiwan.DirectorRead.Turn = NSLOCTEXT("Cinematic", "TaiwanTurn", "从隔海相望到同舟共济，从分离到团圆");
        Taiwan.DirectorRead.POV = NSLOCTEXT("Cinematic", "TaiwanPOV", "海峡两岸人民的双重视角，从遥望到相拥");
        Taiwan.DirectorRead.PowerShift = NSLOCTEXT("Cinematic", "TaiwanPS", "开始：海峡分隔两方；结束：血脉超越地理");
        Taiwan.DirectorRead.HiddenWant = NSLOCTEXT("Cinematic", "TaiwanHW", "两岸人民都渴望团聚，但被时间和距离阻隔");
        Taiwan.DirectorRead.ObstacleTactic = NSLOCTEXT("Cinematic", "TaiwanOT", "障碍：海峡的阻隔和岁月的流逝；策略：以文化和血脉为桥");
        Taiwan.DirectorRead.Subtext = NSLOCTEXT("Cinematic", "TaiwanST", "表面是领土回归，底层是文化认同和血脉亲情");
        Taiwan.DirectorRead.SuppressedBehavior = NSLOCTEXT("Cinematic", "TaiwanSB", "老者手捧族谱，指尖在某个名字上停留了很久，然后轻轻翻过");
        Taiwan.DirectorRead.NonTransferableDetail = NSLOCTEXT("Cinematic", "TaiwanNTD", "那本泛黄的族谱，纸页间夹着一片来自对岸的落叶");
        Taiwan.DirectorRead.StockRefused = NSLOCTEXT("Cinematic", "TaiwanSR", "拒绝：海浪+海鸥+阳光的俗套团圆；替代：以族谱细节和静默的老者建立文化的重量");
        FShotContract TShot1;
        TShot1.ShotID = TEXT("Taiwan_01_StraitAerial");
        TShot1.Duration = 5.0f;
        TShot1.ShotSize = EShotSize::ExtremeWide;
        TShot1.Angle = ECameraAngle::HighAngle;
        TShot1.Support = ECameraSupport::Drone;
        TShot1.Movement = ECameraMovement::LateralTrack;
        TShot1.Ending = EEndingProfile::ExtensionAnchor;
        TShot1.TransitionOut = EShanHeTransitionType::Dissolve;
        TShot1.FocalLength = 24.0f;
        TShot1.Aperture = 8.0f;
        TShot1.MovementSpeed = 0.5f;
        TShot1.LightQuality = ELightQuality::RimLight;
        TShot1.ColorGrade = EColorGrade::TealAndOrange;
        TShot1.MusicTrack = TEXT("EpicTheme");
        TShot1.EffectID = TEXT("Water_River");
        TShot1.EffectTriggerTime = 0.0f;
        Taiwan.Shots.Add(TShot1);
        FShotContract TShot2;
        TShot2.ShotID = TEXT("Taiwan_02_Genealogy");
        TShot2.Duration = 4.5f;
        TShot2.ShotSize = EShotSize::Macro;
        TShot2.Angle = ECameraAngle::Overhead;
        TShot2.Support = ECameraSupport::Slider;
        TShot2.Movement = ECameraMovement::PushIn;
        TShot2.Ending = EEndingProfile::HeroHold;
        TShot2.TransitionOut = EShanHeTransitionType::MatchCut;
        TShot2.FocalLength = 100.0f;
        TShot2.Aperture = 2.8f;
        TShot2.MovementSpeed = 0.3f;
        TShot2.LightQuality = ELightQuality::CandleLight;
        TShot2.ColorGrade = EColorGrade::Sepia;
        TShot2.FragileAnchors = { TEXT("泛黄族谱"), TEXT("对岸落叶") };
        Taiwan.Shots.Add(TShot2);
        FShotContract TShot3;
        TShot3.ShotID = TEXT("Taiwan_03_ElderFace");
        TShot3.Duration = 4.0f;
        TShot3.ShotSize = EShotSize::CloseUp;
        TShot3.Angle = ECameraAngle::EyeLevel;
        TShot3.Support = ECameraSupport::LockedOff;
        TShot3.Movement = ECameraMovement::Breathing;
        TShot3.Ending = EEndingProfile::ExtensionAnchor;
        TShot3.TransitionOut = EShanHeTransitionType::Dissolve;
        TShot3.FocalLength = 50.0f;
        TShot3.Aperture = 2.0f;
        TShot3.LightQuality = ELightQuality::Rembrandt;
        TShot3.ColorGrade = EColorGrade::WarmGolden;
        Taiwan.Shots.Add(TShot3);
        FShotContract TShot4;
        TShot4.ShotID = TEXT("Taiwan_04_Reunion");
        TShot4.Duration = 5.0f;
        TShot4.ShotSize = EShotSize::MediumWide;
        TShot4.Angle = ECameraAngle::EyeLevel;
        TShot4.Support = ECameraSupport::Dolly;
        TShot4.Movement = ECameraMovement::PullBack;
        TShot4.Ending = EEndingProfile::Resolve;
        TShot4.TransitionOut = EShanHeTransitionType::FadeBlack;
        TShot4.FocalLength = 35.0f;
        TShot4.Aperture = 4.0f;
        TShot4.MovementSpeed = 0.5f;
        TShot4.LightQuality = ELightQuality::HighKey;
        TShot4.ColorGrade = EColorGrade::WarmGolden;
        TShot4.Subtitle = NSLOCTEXT("Cinematic", "TaiwanS4", "血脉相连，宝岛归心");
        TShot4.SubtitleStartTime = 2.0f;
        TShot4.SubtitleDuration = 3.0f;
        Taiwan.Shots.Add(TShot4);
        Taiwan.TotalDuration = 18.5f;
        RegisterSequence(Taiwan);
    }
}
