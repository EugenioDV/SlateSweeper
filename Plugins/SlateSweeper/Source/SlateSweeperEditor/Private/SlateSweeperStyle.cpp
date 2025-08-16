// This is a technical test from Eugenio Del Vecchio for Geotech, please do not share.

#include "SlateSweeperStyle.h"
#include "SlateSweeperEditor.h"
#include "Framework/Application/SlateApplication.h"
#include "Styling/SlateStyleRegistry.h"
#include "Slate/SlateGameResources.h"
#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyleMacros.h"

#define RootToContentDir Style->RootToContentDir

TSharedPtr<FSlateStyleSet> FSlateSweeperStyle::StyleInstance = nullptr;

void FSlateSweeperStyle::Initialize()
{
	if (!StyleInstance.IsValid())
	{
		StyleInstance = Create();
		FSlateStyleRegistry::RegisterSlateStyle(*StyleInstance);
	}
}

void FSlateSweeperStyle::Shutdown()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(*StyleInstance);
	ensure(StyleInstance.IsUnique());
	StyleInstance.Reset();
}

FName FSlateSweeperStyle::GetStyleSetName()
{
	static FName StyleSetName(TEXT("SlateSweeperStyle"));
	return StyleSetName;
}


const FVector2D Icon16x16(16.0f, 16.0f);
const FVector2D Icon20x20(20.0f, 20.0f);

TSharedRef< FSlateStyleSet > FSlateSweeperStyle::Create()
{
	TSharedRef< FSlateStyleSet > Style = MakeShareable(new FSlateStyleSet("SlateSweeperStyle"));
	Style->SetContentRoot(IPluginManager::Get().FindPlugin("SlateSweeper")->GetBaseDir() / TEXT("Resources"));

	Style->Set("SlateSweeper.OpenSlateSweeperWindow", new IMAGE_BRUSH_SVG(TEXT("OpenPluginWindowIcon"), Icon20x20));
	return Style;
}

void FSlateSweeperStyle::ReloadTextures()
{
	if (FSlateApplication::IsInitialized())
	{
		FSlateApplication::Get().GetRenderer()->ReloadTextureResources();
	}
}

const ISlateStyle& FSlateSweeperStyle::Get()
{
	return *StyleInstance;
}
