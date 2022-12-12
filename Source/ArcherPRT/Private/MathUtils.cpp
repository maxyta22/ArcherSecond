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

FName UMathUtils::GetNameSocketByType(ETypeSocketForBugs TypeSocketForBugs)
{
	switch (TypeSocketForBugs)
	{
	case ETypeSocketForBugs::HeadSocket:
		return "Head_Socket";
		break;
	case ETypeSocketForBugs::SpineSocket:
		return "Spine_Socket";
		break;
	case ETypeSocketForBugs::LeftShoulderSocket:
		return "Left_Shoulder_Socket";
		break;
	case ETypeSocketForBugs::LeftArmSocket:
		return "Left_Arm_Socket";
		break;
	case ETypeSocketForBugs::RightShoulderSocket:
		return "Right_Shoulder_Socket";
		break;
	case ETypeSocketForBugs::RightArmSocket:
		return "Right_Arm_Socket";
		break;
	case ETypeSocketForBugs::LeftLegSocket:
		return "Left_Leg_Socket";
		break;
	case ETypeSocketForBugs::RightLegSocket:
		return "Right_Leg_Sockett";
		break;
	case ETypeSocketForBugs::RightKneeSocket:
		return "Right_Knee_Socket";
		break;
	case ETypeSocketForBugs::LeftKneeSocket:
		return "Left_Knee_Socket";
		break;
	}
	return FName();
}
