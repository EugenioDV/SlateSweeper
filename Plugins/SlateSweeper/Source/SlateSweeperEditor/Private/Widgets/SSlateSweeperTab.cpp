// This is a technical test from Eugenio Del Vecchio for Geotech, please do not share.

#include "SSlateSweeperTab.h"

#include "SlateSweeperEditor.h"
#include "SlateSweeperGameController.h"
#include "SlateSweeperSettings.h"
#include "Widgets/Input/SSpinBox.h"
#include "SSlateSweeperMinefield.h"
#include "Widgets/Layout/SScaleBox.h"


#define LOCTEXT_NAMESPACE "FSlateSweeperEditorModule"


//TODO style
/*
 *Access editor row width
 *text size
 *highlight color
 *default background color
 *padding values (won't be easy since they use indentation widgets)
 *perhaps a spliter or a drop down thing for the menu so that there is more space for playing when collapsed
 * check if they do the separation thing in ndisplay widgets aka SNew( SBox ) instead of SNew(SBox)
 *
 *
 *
 */

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

void SSlateSweeperTab::Construct(const FArguments& InArgs)
{
	SlateSweeperModule = InArgs._Module;
	USlateSweeperGameSettings* GameSettings = GetMutableDefault<USlateSweeperGameSettings>();
	const USlateSweeperSettings* GeneralSettings = GetDefault<USlateSweeperSettings>();

	if (!IsValid(GameSettings) || !IsValid(GeneralSettings) || !SlateSweeperModule)
	{
		//todo throw error
		return;
	}

	SDockTab::Construct(
		SDockTab::FArguments()
		.TabRole(ETabRole::NomadTab) //todo not sure if tab role should be hardcoded here since stuff is quite scattered
		[
			SNew(SVerticalBox) 
			+SVerticalBox::Slot() //todo this is repeated = bad
			.VAlign(VAlign_Top)
			.AutoHeight()
			[
				ConstructTabRow
				(
					SNew(STextBlock)
					.Text(LOCTEXT("Dimensions", "Dimensions")),
					SNew(SHorizontalBox)
					+SHorizontalBox::Slot()
					.HAlign(HAlign_Left) //todo verify these or just refactor it all into a decent ConstructTabRow
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
							GameSettings->GridHeight = NewValue;
							
							//todo should we bind an actual function here? idk 
							// Since the number of bombs can't be greater than
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
							
							//todo should we bind an actual function here? idk 
							// Since the number of bombs can't be greater than
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
					//uint16 is nice but since we are enforcing min max it can only get in the way
			SNew(SSpinBox<int32>) // Unfortunately this one needs to be very reactive as we can change the grid size after changing the number of mines
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
			.AutoHeight()
			[
				SNew(SButton)
				.Text(LOCTEXT("Start New Session", "Start New Session"))
				.OnPressed_Lambda([this]
				{
					OnStartGamePressed();
				})
				//.OnPressed() //todo this is just test code
			]
			+SVerticalBox::Slot()
			.VAlign(VAlign_Top)
			.HAlign(HAlign_Left)
			[
				SNew(SScaleBox)
				.Stretch(EStretch::ScaleToFit)
				[
					SAssignNew(MinefieldContainer, SBox)
				]
			]
		]);

	// Attempt to recover the previous game in case of a closed-reopened tab
	if (SlateSweeperModule->GetCurrentGameController().IsValid())
	{
		CurrentGame = SlateSweeperModule->GetCurrentGameController();
		MinefieldContainer->SetContent(CurrentGame.Pin()->GetSlateSweeperMinefield());
	}
}

void SSlateSweeperTab::OnStartGamePressed()
{
	USlateSweeperGameSettings* GameSettings = GetMutableDefault<USlateSweeperGameSettings>();
	const USlateSweeperSettings* GeneralSettings = GetDefault<USlateSweeperSettings>();

	if (!IsValid(GameSettings) || !IsValid(GeneralSettings) || !SlateSweeperModule || !MinefieldContainer.IsValid())
	{
		//todo throw error
		return;
	}
	
	CurrentGame = SlateSweeperModule->StartNewGame(GameSettings->GridWidth, GameSettings->GridHeight, GameSettings->TotalMines);
	if (!CurrentGame.IsValid())
	{
		//todo throw error
		return;
	}
	
	MinefieldContainer->SetContent(CurrentGame.Pin()->GetSlateSweeperMinefield());
}


#undef  LOCTEXT_NAMESPACE
