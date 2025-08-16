// This is a technical test from Eugenio Del Vecchio for Geotech, please do not share.

#include "SSlateSweeperTab.h"

#include "SlateSweeperEditor.h"
#include "SlateSweeperGameController.h"
#include "SlateSweeperSettings.h"
#include "Widgets/Input/SSpinBox.h"
#include "SSlateSweeperMinefieldView.h"


#define LOCTEXT_NAMESPACE "FSlateSweeperEditorModule"


/*
 * TODO style start
 * Access editor row width
 * text size
 * highlight color
 * default background color
 * padding values (won't be easy since they use indentation widgets)
 * perhaps a spliter or a drop down thing for the menu so that there is more space for playing when collapsed
 * todo style end
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
		UE_LOG(LogSlateSweeper, Error, TEXT("Failed to construct SlateSweeperTab. Valid checks failed."));
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
							
							// Since the number of bombs can't be greater than the number of cells, adjust accordingly
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
							
							// Since the number of bombs can't be greater than the number of cells, adjust accordingly
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
					// uint16 would be nice here, but since we are already enforcing min/max, it can only get in the way
					// This one needs to be very reactive as we can change the grid size after changing the number of mines
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
				.OnPressed_Lambda([this]
				{
					OnStartNewGamePressed();
				})
			]
			+SVerticalBox::Slot()
			.AutoHeight()
			[
				SAssignNew(MinefieldContainer, SBox)
			]
		]);
	
	// Attempt to recover the previous game in case of a closed-reopened tab
	CurrentGame = SlateSweeperModule->GetCurrentGameController();
	if (CurrentGame.IsValid())
	{
		MinefieldContainer->SetContent(CurrentGame.Pin()->GetOrCreateGameView().Pin().ToSharedRef());
	}
}

void SSlateSweeperTab::OnStartNewGamePressed()
{
	USlateSweeperGameSettings* GameSettings = GetMutableDefault<USlateSweeperGameSettings>();
	const USlateSweeperSettings* GeneralSettings = GetDefault<USlateSweeperSettings>();

	if (!IsValid(GameSettings) || !IsValid(GeneralSettings) || !SlateSweeperModule || !MinefieldContainer.IsValid())
	{
		UE_LOG(LogSlateSweeper, Error, TEXT("Failed to start new game. Valid checks failed."));
		return;
	}
	
	CurrentGame = SlateSweeperModule->StartNewGame(GameSettings->GridWidth, GameSettings->GridHeight, GameSettings->TotalMines);
	if (!CurrentGame.IsValid())
	{
		UE_LOG(LogSlateSweeper, Error, TEXT("Failed to start new game. Valid checks failed."));
		return;
	}
	
	MinefieldContainer->SetContent(CurrentGame.Pin()->GetOrCreateGameView().Pin().ToSharedRef());
}


#undef  LOCTEXT_NAMESPACE
