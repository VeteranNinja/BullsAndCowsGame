/* This is the console executable that makes use of the BullCow class.
This acts as the view in a MVC pattern, and is responsible for all
user interaction. For game logic see the FBullCowGame class.
*/

#pragma once
#include <iostream>
#include <string>
#include <limits>
#include "FBullCowGame.h"

// to make syntax Unreal friendly
using FText = std::string;
using int32 = int;

// function prototypes as outside a class
void PrintIntro();
int32 GetHiddenWordLength();
void SetHiddenWord();
void ClearInputStream();
void PlayGame();
FText GetValidGuess();
void PrintBullCowCount(FBullCowCount);
void PrintGameSummary();
bool AskToPlayAgain();

FBullCowGame BCGame; // instantiate a new game, which we re-use across plays

// the entry point for our application
int main()
{
	do
	{
		PrintIntro();
		SetHiddenWord();
		PlayGame();

	} while (AskToPlayAgain());

	return 0; // exit the application
}

void PrintIntro()
{
	std::cout << "\n ############################################################################# \n";
	std::cout << "\n Welcome to Bulls and Cows, a fun word game.\n";
	std::cout << std::endl;
	std::cout << "          }   {         ___ " << std::endl;
	std::cout << "          (o o)        (o o) " << std::endl;
	std::cout << "   /-------\\ /          \\ /-------\\ " << std::endl;
	std::cout << "  / | BULL |O            O| COW  | \\ " << std::endl;
	std::cout << " *  |-,--- |              |------|  * " << std::endl;
	std::cout << "    ^      ^              ^      ^ " << std::endl;
	std::cout << " Can you guess the isogram I'm thinking of?\n";
	std::cout << "\n Game Rules:\n\n";
	std::cout << " Type in a word that is an isogram (A word with no repeating letters.) \n";
	std::cout << " You have a limited number of  turns to figure out the word!  \n";
	std::cout << " A Bull shows how many letters you have correct ";
	std::cout << " and  in the correct position. \n";
	std::cout << " A Cow shows how many letters you have correct";
	std::cout << " but are in the wrong position. \n";
	std::cout << "\n ############################################################################# \n";
	std::cout << std::endl;
	return;
}

// select hidden word based on user-selected word length
void SetHiddenWord()
{
	int32 WordLength = GetHiddenWordLength();

	// set hidden word based on selected word length
	BCGame.SetHiddenWord(WordLength);

	// display message
	std::cout << "Can you guess the " << BCGame.GetHiddenWordLength() << " letter isogram I'm thinking of?\n";
	std::cout << std::endl;

	return;
}

// get word length from user
int32 GetHiddenWordLength()
{
	int32 WordLength;

	std::cout << "Please select length of isogram (" << BCGame.GetMinWordLength();
	std::cout << "-" << BCGame.GetMaxWordLength() << "): ";

	// get input from user and handle invalid inputs
	while (!(std::cin >> WordLength) || WordLength < BCGame.GetMinWordLength() || WordLength > BCGame.GetMaxWordLength()) {
		ClearInputStream(); // clear input steam
		std::cout << "Invalid input. Try again: ";
	}

	std::cout << std::endl;
	ClearInputStream(); // clear input stream

	return WordLength;
}

void ClearInputStream()
{
	std::cin.clear();
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

// plays single game until completion
void PlayGame()
{
	BCGame.Reset();
	int32 MaxTries = BCGame.GetMaxTries();
	FText Guess = "";

	// loop asking for guesses while the game is NOT won and there are still tries remaining.
	do
	{
		// get a valid guess from the user
		Guess = GetValidGuess();

		// submit valid guess to the game
		FBullCowCount BullCowCount = BCGame.SubmitValidGuess(Guess);

		// print latest result of valid guess
		PrintBullCowCount(BullCowCount);

	} while (!BCGame.IsGameWon() && BCGame.GetCurrentTry() <= MaxTries);

	PrintGameSummary();

	return;
}

// loop continually until the user gives a valid guess
FText GetValidGuess()
{
	EGuessStatus Status = EGuessStatus::Invalid_Status;
	FText Guess = "";

	do {
		// get a guess from the player
		std::cout << "Try " << BCGame.GetCurrentTry() << " of " << BCGame.GetMaxTries();
		std::cout << ". Enter your guess: ";
		std::getline(std::cin, Guess);

		Status = BCGame.CheckGuessValidity(Guess);

		switch (Status)
		{
		case EGuessStatus::Wrong_Length:
			std::cout << "Please enter a " << BCGame.GetHiddenWordLength() << " letter word.\n\n";
			break;
		case EGuessStatus::Not_Isogram:
			std::cout << "Please enter a word without repeating letters.\n\n";
			break;
		case EGuessStatus::Not_Lowercase:
			std::cout << "Please enter all lowercase letters.\n\n";
			break;
		default:
			// assume the guess is valid
			break;
		}
		//std::cout << std::endl;
	} while (Status != EGuessStatus::OK);

	return Guess;

}

// print new guess
void PrintBullCowCount(FBullCowCount BullCowCount)
{
	std::cout << "Bulls = " << BullCowCount.Bulls;
	std::cout << ". Cows = " << BullCowCount.Cows << ".\n\n";

	return;
}

void PrintGameSummary()
{

	if (BCGame.IsGameWon())
	{
		std::cout << "WELL DONE - YOU WIN!\n";
	}
	else
	{
		std::cout << "Out of tries - better luck next time!\n";
	}

	return;

}

bool AskToPlayAgain()
{
	std::cout << "Do you want to play again (y/n)? ";
	FText Response = "";
	std::getline(std::cin, Response);
	std::cout << std::endl;

	return (Response[0] == 'y') || (Response[0] == 'Y');

}