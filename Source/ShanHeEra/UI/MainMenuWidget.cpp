#include "UI/MainMenuWidget.h"
#include "Components/SizeBox.h"
#include "Components/Overlay.h"
#include "Components/Border.h"
#include "Components/Slider.h"
#include "Components/ScrollBox.h"
#include "Components/HorizontalBox.h"
#include "Components/VerticalBox.h"
#include "Components/CanvasPanel.h"
#include "Components/GridPanel.h"
#include "Components/ProgressBar.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/ComboBoxString.h"
#include "Core/ShanHeLog.h"

bool UMainMenuWidget::Initialize()
{
    if (!Super::Initialize()) return false;
    if (NewGameButton) NewGameButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnNewGame);
    if (ContinueButton) ContinueButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnContinue);
    if (SettingsButton) SettingsButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnSettings);
    if (ExitButton) ExitButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnExit);
    if (LanguageSelect)
    {
        LanguageSelect->AddOption(TEXT("中文"));
        LanguageSelect->AddOption(TEXT("English"));
        LanguageSelect->AddOption(TEXT("日本語"));
        LanguageSelect->OnSelectionChanged.AddDynamic(this, &UMainMenuWidget::OnLanguageChanged);
    }
    return true;
}

void UMainMenuWidget::OnNewGame() { UE_LOG(LogShanHe, Log, TEXT("新游戏 - 玩家只是出生在华夏大地上的一个生命")); }
void UMainMenuWidget::OnContinue() {}
void UMainMenuWidget::OnSettings() {}
void UMainMenuWidget::OnExit() {}
void UMainMenuWidget::OnLanguageChanged(FString SelectedItem, ESelectInfo::Type SelectionType) {}
void UMainMenuWidget::SetLanguage(ELanguage NewLang) {}
