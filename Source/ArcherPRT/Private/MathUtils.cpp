// Archer Prototype. All rights reserved


#include "MathUtils.h"

DEFINE_LOG_CATEGORY_STATIC(LogMathUtils, Warning, All);

float UMathUtils::FindAngleBetweenForwardVectorAndTarget(FVector Center, FVector Forward, FVector Target)
{
	
	const auto OneVector = Forward;
	const auto TwoVector = Target - Center;
	const auto AngleBetween = FMath::Acos(FVector::DotProduct(OneVector.GetSafeNormal(), TwoVector.GetSafeNormal()));
	const auto CrossProduct = FVector::CrossProduct(OneVector, TwoVector);
	const auto Degrees = FMath::RadiansToDegrees(AngleBetween);
	const auto Angle = FMath::RadiansToDegrees(AngleBetween) * FMath::Sign(CrossProduct.Z);

	FString AngleStr = "Angle = " + FString::SanitizeFloat(Angle);
	UE_LOG(LogMathUtils, Display, TEXT("%s"), *AngleStr);

	return Angle;
}