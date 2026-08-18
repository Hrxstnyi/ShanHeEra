#include "ShanHeEra.h"
#include "Core/ShanHeLog.h"

DEFINE_LOG_CATEGORY(LogShanHe);

void FShanHeEraModule::StartupModule()
{
    UE_LOG(LogShanHe, Log, TEXT("山河纪元 ShanHeEra module started"));
}

void FShanHeEraModule::ShutdownModule()
{
    UE_LOG(LogShanHe, Log, TEXT("山河纪元 ShanHeEra module shutdown"));
}

IMPLEMENT_PRIMARY_GAME_MODULE(FShanHeEraModule, ShanHeEra, "ShanHeEra");
