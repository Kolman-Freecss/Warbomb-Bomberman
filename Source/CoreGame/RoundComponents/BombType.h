#pragma once

UENUM(BlueprintType)
enum class BombType : uint8
{
	WEAK UMETA(DisplayName = "Weak"),
	STRONG UMETA(DisplayName = "Strong"),
	MAX UMETA(Hidden)
};
