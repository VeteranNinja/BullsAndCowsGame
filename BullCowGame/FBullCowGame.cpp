#include "FBullCowGame.h"
#include <map>
#define TMap std::map

using int32 = int;


FBullCowGame::FBullCowGame() { 	Reset(); } //default constructor

int32 FBullCowGame::GetCurrentTry() const { return  MyCurrentTry; }
int32 FBullCowGame::GetHiddenWordLength() const { return MyHiddenWord.length(); }
bool FBullCowGame::IsGameWon() const {	return bGameIsWon; }

int32 FBullCowGame::GetMaxTries() const
{
	TMap<int32, int32> WordLengthToMaxTries{ { 3, 5 }, { 4, 6 }, { 5, 8 }, { 6, 10 } };
	return WordLengthToMaxTries[MyHiddenWord.length()];
}

void FBullCowGame::Reset()
{
	const FString HIDDEN_WORD = "planet"; //this must be an isogram
	MyHiddenWord = HIDDEN_WORD;
	
	MyCurrentTry = 1;
	bGameIsWon = false;
	return;
}

EGuessStatus FBullCowGame::CheckGuessValidity(FString Guess) const
{
	if(!IsIsogram(Guess)) // if the guess isn't an isogram
	{
		return EGuessStatus::Not_Isogram;	
	}
	else if (!IsLowerCase(Guess))	// if the guess isn't all lowercase 
	{
		return EGuessStatus::Not_Lowercase;

	}
	else if (Guess.length() != GetHiddenWordLength())	// if the guess length is wrong
	{
		return EGuessStatus::Wrong_Length;
	}
	else
	{
		return EGuessStatus::OK;
	}
}

//Receives a VALID guess, increments turn and returns count
FBullCowCount FBullCowGame::SubmitValidGuess(FString Guess)
{
	MyCurrentTry++;
	FBullCowCount BullCowCount;
	int32 WordLength = MyHiddenWord.length(); //assuming same length as guess

	// loop through all letters in the hidden word
	for (int32 MHWChar = 0; MHWChar < WordLength; MHWChar++) //assuming same length as guess
	{
		//loop through all letters against the guess
		for (int32 GChar = 0; GChar < WordLength; GChar++) 
		{
			// check to see if they match
			if (Guess[GChar] == MyHiddenWord[MHWChar]) 
			{
				if (MHWChar == GChar) 
				{ 
					BullCowCount.Bulls++;
				}
				else 
				{
					BullCowCount.Cows++; // must be a cows
				}
			}
		}
	}
	if (BullCowCount.Bulls == WordLength)
		{
			bGameIsWon = true;
		}
	else
	{
		bGameIsWon = false;
	}
	return BullCowCount;
}

bool FBullCowGame::IsIsogram(FString Word) const
{
	// treat 0 and 1 letter words as isogram
	if (Word.length() <= 1) { return true; }

	TMap<char, bool> LetterSeen; //setup our map
	for (auto Letter : Word)	 // for all letters of the word
	{
		Letter = tolower(Letter); //handle mixed letters
		if (LetterSeen[Letter])	// if the letter is in the map
		{
			return false;	// we do NOT have an isogram
		}
		else
		{
			LetterSeen[Letter] = true;	// add the letter to the map as seen
		}
	}
	return true; // for example in cases where /0 is entered
}

bool FBullCowGame::IsLowerCase(FString Word) const
{

		for (auto Letter : Word)
		{
			if (!islower(Letter)) //if not a lowercase letter
			return false;
		}
		
	return true;
}
