// This is a technical test from Eugenio Del Vecchio for Geotech, please do not share.

#include "SSlateSweeperMenu.h"
#include "SlateSweeperEditor.h"
#include "SlateSweeperSettings.h"
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
	const USlateSweeperSettings* GeneralSettings = GetDefault<USlateSweeperSettings>();

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
					.Value(GameSettings->GridWidth)
					.OnValueCommitted_Lambda([GameSettings, GeneralSettings](uint8 NewValue, ETextCommit::Type CommitType)
					{
						GameSettings->GridWidth = NewValue;
						GameSettings->TotalMines = FMath::Clamp
						(
							GameSettings->TotalMines,
							GeneralSettings->GetMinAllowedMines(GameSettings->GridWidth, GameSettings->GridHeight),
							GeneralSettings->GetMaxAllowedMines(GameSettings->GridWidth, GameSettings->GridHeight)
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
					.Value(GameSettings->GridHeight)
					.OnValueCommitted_Lambda([GameSettings, GeneralSettings](uint8 NewValue, ETextCommit::Type CommitType)
					{
						GameSettings->GridHeight = NewValue;
						GameSettings->TotalMines = FMath::Clamp
						(
							GameSettings->TotalMines,
							GeneralSettings->GetMinAllowedMines(GameSettings->GridWidth, GameSettings->GridHeight),
							GeneralSettings->GetMaxAllowedMines(GameSettings->GridWidth, GameSettings->GridHeight)
						);
						GameSettings->SaveConfig();
					})
				]
			)
		]

		+SVerticalBox::Slot()
		.VAlign(VAlign_Top)
		.AutoHeight()
		[
			ConstructTabRow
			(
				SNew(STextBlock)
				.Text(LOCTEXT("NumberOfMines", "Number of Mines")),

				SNew(SSpinBox<int32>)
				.MinValue_Lambda([GameSettings, GeneralSettings]
				{
					return GeneralSettings->GetMinAllowedMines(GameSettings->GridWidth, GameSettings->GridHeight);
				})
				.MaxValue_Lambda([GameSettings, GeneralSettings]
				{
					return GeneralSettings->GetMaxAllowedMines(GameSettings->GridWidth, GameSettings->GridHeight);
				})
				.Value_Lambda([GameSettings]
				{
					return GameSettings->TotalMines;
				})
				.OnValueChanged_Lambda([GameSettings](int32 NewValue)
				{
					GameSettings->TotalMines = NewValue;
				})
				.OnValueCommitted_Lambda([GameSettings](int32 NewValue, ETextCommit::Type CommitType)
				{
					GameSettings->TotalMines = NewValue;
					GameSettings->SaveConfig();
				})
			)
		]

		+SVerticalBox::Slot()
		.VAlign(VAlign_Top)
		.HAlign(HAlign_Left)
		.FillHeight(1.f)
		.Padding(20.f)
		[
			SNew(SButton)
			.Text(LOCTEXT("StartNewGame", "Start New Game"))
			.OnClicked_Lambda([this, GameSettings]
			{
				OnStartGameClicked.ExecuteIfBound(GameSettings->GridWidth, GameSettings->GridHeight, GameSettings->TotalMines);
				return FReply::Handled();
			})
		]
	];
}

#undef LOCTEXT_NAMESPACE
