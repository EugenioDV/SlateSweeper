// This is a technical test from Eugenio Del Vecchio for Geotech, please do not share.

#include "SSlateSweeperMenu.h"
#include "SlateSweeperEditorSettings.h"
#include "Widgets/Input/SSpinBox.h"

#define LOCTEXT_NAMESPACE "FSlateSweeperEditor"

/* Minimalistic helper for clean and consistent-looking rows */
TSharedRef<SHorizontalBox> ConstructTabRow(const TSharedRef<SWidget>& LeftWidget, const TSharedRef<SWidget>& RightWidget)
{
	return SNew(SHorizontalBox)
		+SHorizontalBox::Slot()
		[
			SNew(SSplitter)
			+SSplitter::Slot()
			[
				LeftWidget
			]
			+SSplitter::Slot()
			[
				RightWidget
			]
		];
}

void SSlateSweeperMenu::Construct(const FArguments& InArgs)
{
	OnStartGameClicked = InArgs._OnStartGameClicked;
	USlateSweeperGameSettings* GameSettings = GetMutableDefault<USlateSweeperGameSettings>();
	const USlateSweeperEditorSettings* GeneralSettings = GetDefault<USlateSweeperEditorSettings>();

	check(IsValid(GameSettings) && IsValid(GeneralSettings))

	ChildSlot
	[
		SNew(SVerticalBox)
		+SVerticalBox::Slot()
		.VAlign(VAlign_Top)
		.AutoHeight()
		[
			ConstructTabRow
			(
				SNew(STextBlock)
				.Text(LOCTEXT("Dimensions", "Dimensions")),

				SNew(SHorizontalBox)
				+SHorizontalBox::Slot()
				.HAlign(HAlign_Left)
				.AutoWidth()
				[
					SNew(STextBlock)
					.Text(LOCTEXT("Width", "Width"))
				]
				+SHorizontalBox::Slot()
				.HAlign(HAlign_Left)
				[
					SNew(SSpinBox<uint8>)
					.MinValue(GeneralSettings->MinGridWidth)
					.MaxValue(GeneralSettings->MaxGridWidth)
					.Value(GameSettings->CurrentGameSettings.GridWidth)
					.OnValueCommitted_Lambda([GameSettings, GeneralSettings](uint8 NewValue, ETextCommit::Type CommitType)
					{
						GameSettings->CurrentGameSettings.GridWidth = NewValue;
						GameSettings->CurrentGameSettings.TotalMines = FMath::Clamp
						(
							GameSettings->CurrentGameSettings.TotalMines,
							GeneralSettings->GetMinAllowedMines(GameSettings->CurrentGameSettings.GridWidth, GameSettings->CurrentGameSettings.GridHeight),
							GeneralSettings->GetMaxAllowedMines(GameSettings->CurrentGameSettings.GridWidth, GameSettings->CurrentGameSettings.GridHeight)
						);
						GameSettings->SaveConfig();
					})
				]
				+SHorizontalBox::Slot()
				.HAlign(HAlign_Left)
				.AutoWidth()
				[
					SNew(STextBlock)
					.Text(LOCTEXT("Height", "Height"))
				]
				+SHorizontalBox::Slot()
				.HAlign(HAlign_Left)
				[
					SNew(SSpinBox<uint8>)
					.MinValue(GeneralSettings->MinGridHeight)
					.MaxValue(GeneralSettings->MaxGridHeight)
					.Value(GameSettings->CurrentGameSettings.GridHeight)
					.OnValueCommitted_Lambda([GameSettings, GeneralSettings](uint8 NewValue, ETextCommit::Type CommitType)
					{
						GameSettings->CurrentGameSettings.GridHeight = NewValue;
						GameSettings->CurrentGameSettings.TotalMines = FMath::Clamp
						(
							GameSettings->CurrentGameSettings.TotalMines,
							GeneralSettings->GetMinAllowedMines(GameSettings->CurrentGameSettings.GridWidth, GameSettings->CurrentGameSettings.GridHeight),
							GeneralSettings->GetMaxAllowedMines(GameSettings->CurrentGameSettings.GridWidth, GameSettings->CurrentGameSettings.GridHeight)
						);
						GameSettings->SaveConfig();
					})
				]
			)
		]
		+SVerticalBox::Slot()
		.VAlign(VAlign_Top)
		.HAlign(HAlign_Left)
		[
			ConstructTabRow
			(
				SNew(STextBlock)
				.Text(LOCTEXT("NumberOfMines", "Number of Mines")),

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
				.OnValueCommitted_Lambda([GameSettings](int32 NewValue, ETextCommit::Type CommitType)
				{
					GameSettings->CurrentGameSettings.TotalMines = NewValue;
					GameSettings->SaveConfig();
				})
			)
		]
		+SVerticalBox::Slot()
		.VAlign(VAlign_Top)
		.HAlign(HAlign_Left)
		[
			SNew(SButton)
			.Text(LOCTEXT("StartNewGame", "Start New Game"))
			.OnClicked_Lambda([this, GameSettings]
			{
				OnStartGameClicked.ExecuteIfBound(GameSettings->CurrentGameSettings);
				return FReply::Handled();
			})
		]
	];
}

#undef LOCTEXT_NAMESPACE
