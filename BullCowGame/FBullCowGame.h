/* The game logic (no view code or direct user interaction)
The game is a simple guess the word game based on Mastermind
*/

#pragma once // unsures that anything imported is only imported once
#include <string>
#include <map>
#define TMap std::map

// to make syntax Unreal friendly
using FString = std::string;
using int32 = int;

struct FBullCowCount
{
	int32 Bulls = 0;
	int32 Cows = 0;
};

enum class EGuessStatus
{
	OK,
	Invalid_Status,
	Not_Isogram,
	Wrong_Length,
	Not_Lowercase
};

class FBullCowGame {
public:

	FBullCowGame(); // constructor

	int32 GetMaxTries() const;
	int32 GetMinWordLength() const;
	int32 GetMaxWordLength() const;
	int32 GetCurrentTry() const;
	int32 GetHiddenWordLength() const;

	bool IsGameWon() const;
	EGuessStatus CheckGuessValidity(FString) const;

	void Reset();
	void SetHiddenWord(int32);
	FBullCowCount SubmitValidGuess(FString);

private:

	// see constructor for initialization
	int32 MyCurrentTry;
	int32 MinWordLength;
	int32 MaxWordLength;
	bool bGameWon;
	TMap<int32, FString> WordLengthToHiddenWord;

	FString HiddenWord;

	// check if guess is isogram
	bool IsIsogram(FString) const;
	bool IsLower(FString) const;

};