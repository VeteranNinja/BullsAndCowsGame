#pragma once
#include "FBullCowGame.h"
#define TMap std::map

// to make syntax Unreal friendly
using FString = std::string;
using int32 = int;

// getter functions
int32 FBullCowGame::GetCurrentTry() const { return MyCurrentTry; }
int32 FBullCowGame::GetHiddenWordLength() const { return HiddenWord.length(); }
int32 FBullCowGame::GetMinWordLength() const { return MinWordLength; }
int32 FBullCowGame::GetMaxWordLength() const { return MaxWordLength; }
bool FBullCowGame::IsGameWon() const { return bGameWon; }

// default constructor
FBullCowGame::FBullCowGame() { Reset(); }

void FBullCowGame::Reset()
{
	constexpr int32 MIN_WORD_LENGTH = 3;
	constexpr int32 MAX_WORD_LENGTH = 7;
	MinWordLength = MIN_WORD_LENGTH;
	MaxWordLength = MAX_WORD_LENGTH;

	TMap<int32, FString> MY_HIDDEN_WORDS = { { 3,"pen" },{ 4,"rate" },{ 5,"paint" },{ 6,"planet" },{ 7,"subject" } }; // all words MUST be isograms
	WordLengthToHiddenWord = MY_HIDDEN_WORDS;

	MyCurrentTry = 1;
	bGameWon = false;
	return;
}

void FBullCowGame::SetHiddenWord(int32 WordLength)
{
	HiddenWord = WordLengthToHiddenWord[WordLength];
}

int32 FBullCowGame::GetMaxTries() const
{
	TMap<int32, int32> WordLengthToMaxTries{ {3,4}, {4,7}, {5,10}, {6,15}, {7,20} };
	return WordLengthToMaxTries[HiddenWord.length()];
}

bool FBullCowGame::IsIsogram(FString Guess) const
{
	TMap<char, bool> LetterSeen;

	if (Guess.length() > 1) //treat 0 and 1 letter words as isograms
	{
		// loop through guess
		for (auto Letter : Guess)
		{
			Letter = tolower(Letter); // handle mixed case

			if (LetterSeen[Letter])  // check if letter already exists in alphabet map
			{
				return false;
			}
			else // otherwise populate map with new letter
			{
				LetterSeen[Letter] = true;
			}
		}
	}

	// return true if no repeated letters found
	return true;
}

bool FBullCowGame::IsLower(FString Guess) const
{

	if (Guess.length() > 1)
	{
		// check each letter in Guess
		for (auto Letter : Guess)
		{
			if (!islower(Letter))
			{
				return false; // return false if uppercase letter
			}
		}
	}

	return true; // return true if all lowercase
}

EGuessStatus FBullCowGame::CheckGuessValidity(FString Guess) const
{
	if (!IsLower(Guess)) // if the guess is not lowercase 
	{
		return EGuessStatus::Not_Lowercase;
	}
	else if (!IsIsogram(Guess)) // if the guess is not an isogram 
	{
		return EGuessStatus::Not_Isogram;
	}
	else if (Guess.length() != GetHiddenWordLength()) // if the guess is wrong length
	{
		return EGuessStatus::Wrong_Length;
	}
	else // otherwise return OK
	{
		return EGuessStatus::OK;
	}

	return EGuessStatus::OK;
}

// receives a VALID guess, increments turn and returns count of Bulls and Cows
FBullCowCount FBullCowGame::SubmitValidGuess(FString Guess)
{
	MyCurrentTry++;
	FBullCowCount BullCowCount;

	int32 WordLength = HiddenWord.length(); // assuming the same length as guess

	// loop through all letters in the hidden word
	for (int32 MHWChar = 0; MHWChar < WordLength; MHWChar++)
	{
		// compare letters against the guess
		for (int32 GChar = 0; GChar < WordLength; GChar++)
		{
			if ((Guess[GChar] == HiddenWord[MHWChar]) && (GChar == MHWChar))
			{
				BullCowCount.Bulls++; // increment bulls if they're in the same place
			}
			else if ((Guess[GChar] == HiddenWord[MHWChar]) && (GChar != MHWChar))
			{
				BullCowCount.Cows++; // increment cows if not
			}
		}
	}

	if (BullCowCount.Bulls == WordLength)
	{
		bGameWon = true;
	}

	return BullCowCount;
}
