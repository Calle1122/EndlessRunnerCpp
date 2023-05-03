#pragma once

#include "CoreMinimal.h"
#include "LeaderboardItem.generated.h"

USTRUCT(BlueprintType)
struct ENDLESSRUNNER_API FLeaderboardItem
{
public:
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Score;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Name1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Name2;

	FLeaderboardItem();
	~FLeaderboardItem();

	bool operator<(const FLeaderboardItem& B) const
	{
		return Score<B.Score;
	}

	/*bool score_sorter(FLeaderboardItem const& lhs, FLeaderboardItem const& rhs) {
		if (lhs.Score != rhs.Score)
		{
			return lhs.Score < rhs.Score;
		}
		return true;
	}*/
};