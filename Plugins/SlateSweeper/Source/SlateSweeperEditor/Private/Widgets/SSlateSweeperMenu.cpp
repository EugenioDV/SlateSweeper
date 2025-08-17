// This is a technical test from Eugenio Del Vecchio for Geotech, please do not share.

#include "SSlateSweeperMenu.h"
#include "SlateSweeperEditorSettings.h"
#include "Widgets/Input/SSpinBox.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Input/SButton.h"

#define LOCTEXT_NAMESPACE "FSlateSweeperEditor"



void SSlateSweeperMenu::Construct(const FArguments& InArgs)
{
	OnStartGameClicked = InArgs._OnStartGameClicked;

	USlateSweeperGameSettings* GameSettings = GetMutableDefault<USlateSweeperGameSettings>();
	const USlateSweeperEditorSettings* GeneralSettings = GetDefault<USlateSweeperEditorSettings>();
	check(IsValid(GameSettings) && IsValid(GeneralSettings))


	/*
	 * We add the various sections row by row, in a hardcoded fashion
	 * We could do much better with a builder, standards, splitters and so on
	 * But aesthetics not being a priority, we'd be out of scope for this project
	 */
	
	TSharedRef<SVerticalBox> MenuRowHost = SNew(SVerticalBox);

	static constexpr float HorizontalMargin = 12.f;
	static constexpr float VerticalMargin = 12.f;


	// Title row
	MenuRowHost->AddSlot()
	.Padding(HorizontalMargin, VerticalMargin, HorizontalMargin, 0.f)
	[
		SNew(STextBlock)
		.Text(LOCTEXT("SettingsMenuTitle", "Game Settings:"))
	];
	
	// Dimensions row
	MenuRowHost->AddSlot()
	.Padding(HorizontalMargin, VerticalMargin, 0.f, 0.f)
	[
		SNew(SHorizontalBox)
		+SHorizontalBox::Slot()
		.AutoWidth()
		.VAlign(VAlign_Center)
		[
			SNew(STextBlock)
			.Text(LOCTEXT("Width", "Width"))
		]
		+SHorizontalBox::Slot()
		.Padding(HorizontalMargin,0.f,0.f,0.f)
		.VAlign(VAlign_Center)
		[
			SNew(SSpinBox<uint8>)
			.MinValue(GeneralSettings->MinGridWidth)
			.MaxValue(GeneralSettings->MaxGridWidth)
			.Value(GameSettings->CurrentGameSettings.GridWidth)
			.OnValueCommitted_Lambda([GameSettings, GeneralSettings](uint8 NewValue, ETextCommit::Type)
			{
				GameSettings->CurrentGameSettings.GridWidth = NewValue;

				// We clamp the total mines as we edit the grid size as it might not be able to contain them
				GameSettings->CurrentGameSettings.TotalMines = FMath::Clamp(
					GameSettings->CurrentGameSettings.TotalMines,
					GeneralSettings->GetMinAllowedMines(GameSettings->CurrentGameSettings.GridWidth, GameSettings->CurrentGameSettings.GridHeight),
					GeneralSettings->GetMaxAllowedMines(GameSettings->CurrentGameSettings.GridWidth, GameSettings->CurrentGameSettings.GridHeight)
					);
					GameSettings->SaveConfig();
			})
		]
		+SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(HorizontalMargin,0.f,0.f,0.f)
		.VAlign(VAlign_Center)
		[
			SNew(STextBlock)
			.Text(LOCTEXT("Height", "Height"))
		]
		+SHorizontalBox::Slot()
		.Padding(HorizontalMargin,0.f,HorizontalMargin,0.f)
		.VAlign(VAlign_Center)
		[
			SNew(SSpinBox<uint8>)
			.MinValue(GeneralSettings->MinGridHeight)
			.MaxValue(GeneralSettings->MaxGridHeight)
			.Value(GameSettings->CurrentGameSettings.GridHeight)
			.OnValueCommitted_Lambda([GameSettings, GeneralSettings](uint8 NewValue, ETextCommit::Type)
			{
				GameSettings->CurrentGameSettings.GridHeight = NewValue;
				
				// We clamp the total mines as we edit the grid size as it might not be able to contain them
				GameSettings->CurrentGameSettings.TotalMines = FMath::Clamp(
					GameSettings->CurrentGameSettings.TotalMines,
					GeneralSettings->GetMinAllowedMines(GameSettings->CurrentGameSettings.GridWidth, GameSettings->CurrentGameSettings.GridHeight),
					GeneralSettings->GetMaxAllowedMines(GameSettings->CurrentGameSettings.GridWidth, GameSettings->CurrentGameSettings.GridHeight)
					);
				GameSettings->SaveConfig();
			})
		]
	];

	// Number of mines row
	MenuRowHost->AddSlot()
	.Padding(HorizontalMargin, VerticalMargin, HorizontalMargin, 0.f)
	[
		SNew(SHorizontalBox)
		+SHorizontalBox::Slot()
		.VAlign(VAlign_Center)
		.AutoWidth()
		.Padding(0.f, 0.f, HorizontalMargin, 0.f)
		[
			SNew(STextBlock)
			.Text(LOCTEXT("NumberOfMines", "Number of Mines"))
		]
		+SHorizontalBox::Slot()
		.VAlign(VAlign_Center)
		.FillWidth(1.f)
		[
			SNew(SSpinBox<int32>)
			.MinValue_Lambda([GameSettings, GeneralSettings]
			{
				return GeneralSettings->GetMinAllowedMines(GameSettings->CurrentGameSettings.GridWidth, GameSettings->CurrentGameSettings.GridHeight);
			})
			.MaxValue_Lambda([GameSettings, GeneralSettings]
			{
				return GeneralSettings->GetMaxAllowedMines(GameSettings->CurrentGameSettings.GridWidth, GameSettings->CurrentGameSettings.GridHeight);
			})
			.Value_Lambda([GameSettings]
			{
				return GameSettings->CurrentGameSettings.TotalMines;
			})
			.OnValueChanged_Lambda([GameSettings](int32 NewValue)
			{
				GameSettings->CurrentGameSettings.TotalMines = NewValue;
			})
			.OnValueCommitted_Lambda([GameSettings](int32 NewValue, ETextCommit::Type)
			{
				GameSettings->CurrentGameSettings.TotalMines = NewValue;
				GameSettings->SaveConfig();
			})
		]
	];

	// Start button row
	MenuRowHost->AddSlot()
	.Padding(HorizontalMargin, VerticalMargin, HorizontalMargin, VerticalMargin)
	.HAlign(HAlign_Left)
	[
		SNew(SButton)
		.Text(LOCTEXT("StartNewGame", "Start New Game"))
		.OnClicked_Lambda([this, GameSettings]
		{
			OnStartGameClicked.ExecuteIfBound(GameSettings->CurrentGameSettings);
			return FReply::Handled();
		})
	];

	ChildSlot
	[
		SNew(SBorder)
		[
			MenuRowHost
		]
	];
}

#undef LOCTEXT_NAMESPACE

/*
SNew(STextBlock).Text(LOCTEXT("Dimensions", "Dimensions")),
SNew(SHorizontalBox)
+SHorizontalBox::Slot()
.AutoWidth()
.VAlign(VAlign_Center)
[
	SNew(STextBlock)
	.Text(LOCTEXT("Width", "Width"))
]*/