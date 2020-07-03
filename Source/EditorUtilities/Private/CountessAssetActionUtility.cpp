// Free to play. By SoNa Games.


#include "CountessAssetActionUtility.h"
#include "EditorUtilityLibrary.h"
#include "EditorAssetLibrary.h"
#include "Engine/Texture.h"

#pragma region RenameSelectedAssets

void UCountessAssetActionUtility::RenameSelectedAssets(FString SearchPattern, FString ReplacePattern, ESearchCase::Type SearchCase)
{
	if (SearchPattern.IsEmpty() || SearchPattern.Equals(ReplacePattern, SearchCase))
		return;

	TArray<UObject*> SelectedObjects = UEditorUtilityLibrary::GetSelectedAssets();

	uint32 Counter = 0;

	for (UObject* SelectedObject : SelectedObjects)
	{
		if (ensure(SelectedObject))
		{
			FString AssetName = SelectedObject->GetFName().ToString();
			if (AssetName.Contains(SearchPattern, SearchCase))
			{
				FString NewName = AssetName.Replace(*SearchPattern, *ReplacePattern, SearchCase);
				UEditorUtilityLibrary::RenameAsset(SelectedObject, NewName);
				Counter++;
			}
		}
	}
	GiveFeedback(TEXT("Renamed"), Counter);
}

#pragma endregion

#pragma region CheckPowerOfTwo

void UCountessAssetActionUtility::CheckPowerOfTwo()
{
	TArray<UObject*> SelectedObjects = UEditorUtilityLibrary::GetSelectedAssets();

	uint32 Counter = 0;

	for (UObject* SelectedObject : SelectedObjects)
	{
		if (ensure(SelectedObject))
		{
			UTexture* Texture = dynamic_cast<UTexture*>(SelectedObject);
			if (ensure(Texture))
			{
				int32 Width = static_cast<int32>(Texture->GetSurfaceWidth());
				int32 Height = static_cast<int32>(Texture->GetSurfaceHeight());
				if (!IsPowerOfTwo(Height) || !IsPowerOfTwo(Width))
				{
					PrintToScreen(SelectedObject->GetPathName() + " is not a power of 2 Texture", FColor::Red);
				}
				else
				{
					Counter++;
				}
			}
			else
			{
				PrintToScreen(SelectedObject->GetPathName() + " is not a Texture", FColor::Red);
			}
		}
	}
	GiveFeedback(TEXT("Power of 2"), Counter);
}

#pragma endregion

#pragma region AddPrefixes

void UCountessAssetActionUtility::AddPreFixes()
{
	TArray<UObject*> SelectedObjects = UEditorUtilityLibrary::GetSelectedAssets();

	uint32 Counter = 0;

	for (UObject* SelectedObject : SelectedObjects)
	{
		if (ensure(SelectedObject))
		{
			const FString* Prefix = PrefixMap.Find(SelectedObject->GetClass());
			if (ensure(Prefix) && !Prefix->Equals(""))
			{
				FString OldName = SelectedObject->GetFName().ToString();
				if (!OldName.StartsWith(*Prefix))
				{
					FString NewName = *Prefix + OldName;
					UEditorUtilityLibrary::RenameAsset(SelectedObject, NewName);
					Counter++;
				}
			}
			else
			{
				PrintToScreen(TEXT("Couldn't find Prefix for class ") + SelectedObject->GetClass()->GetFName().ToString(), FColor::Red);
			}
		}
	}
	GiveFeedback(TEXT("Added Prefix to"), Counter);
}

#pragma endregion

#pragma region CleanUpFolder

void UCountessAssetActionUtility::CleanUpFolder(FString ParentFolder)
{
	// Check if parent folder is in content folder
	if (!ParentFolder.StartsWith(TEXT("/Game")))
	{
		ParentFolder = FPaths::Combine(TEXT("/Game"), ParentFolder);
	}

	TArray<UObject*> SelectedObjects = UEditorUtilityLibrary::GetSelectedAssets();

	uint32 Counter = 0;

	for (UObject* SelectedObject : SelectedObjects)
	{
		if (ensure(SelectedObject))
		{
			FString NewPath = FPaths::Combine(ParentFolder, SelectedObject->GetClass()->GetFName().ToString(), SelectedObject->GetFName().ToString());
			if (UEditorAssetLibrary::RenameLoadedAsset(SelectedObject, NewPath))
			{
				Counter++;
			}
			else
			{
				PrintToScreen(TEXT("Couldn't move ") + SelectedObject->GetPathName(), FColor::Red);
			}
		}
	}
	GiveFeedback(TEXT("Moved"), Counter);
}

#pragma endregion

#pragma region RemoveUnusedAssets

void UCountessAssetActionUtility::RemoveUnusedAssets(bool bDeleteImmediately)
{
	TArray<UObject*> SelectedObjects = UEditorUtilityLibrary::GetSelectedAssets();
	TArray<UObject*> UnusedObjects = TArray<UObject*>();

	for (UObject* SelectedObject : SelectedObjects)
	{
		if (ensure(SelectedObject))
		{
			if (UEditorAssetLibrary::FindPackageReferencersForAsset(SelectedObject->GetPathName(), true).Num() <= 0)
			{
				UnusedObjects.Add(SelectedObject);
			}
		}
	}

	uint32 Counter = 0;

	for (UObject* UnusedObject : UnusedObjects)
	{
		if (bDeleteImmediately)
		{
			if (UEditorAssetLibrary::DeleteLoadedAsset(UnusedObject))
			{
				Counter++;
			}
			else
			{
				PrintToScreen(TEXT("Error deleting ") + UnusedObject->GetPathName(), FColor::Red);
			}
		}
		else
		{
			FString NewPath = FPaths::Combine(TEXT("/Game"), TEXT("Trash"), UnusedObject->GetFName().ToString());
			if (UEditorAssetLibrary::RenameLoadedAsset(UnusedObject, NewPath))
			{
				Counter++;
			}
			else
			{
				PrintToScreen(TEXT("Error Moving ") + UnusedObject->GetPathName(), FColor::Red);
			}
		}
	}

	GiveFeedback(bDeleteImmediately ? TEXT("Deleted") : TEXT("Moved to Trash"), Counter);
}

#pragma endregion

#pragma region Helper

void UCountessAssetActionUtility::PrintToScreen(FString Message, FColor Color)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.5f, Color, Message);
	}
}

void UCountessAssetActionUtility::GiveFeedback(FString Method, uint32 Counter)
{
	FString Message = FString("No Matching Files Found!");
	FColor Color = Counter > 0 ? FColor::Green : FColor::Red;
	if (Counter > 0)
	{
		Message = Method.AppendChar(' ');
		Message.AppendInt(Counter);
		Message.Append(Counter == 1 ? TEXT(" file") : TEXT(" files"));
	}
	PrintToScreen(Message, Color);
}

bool UCountessAssetActionUtility::IsPowerOfTwo(int32 NumberToCheck)
{
	if (NumberToCheck == 0)
		return false;

	return (NumberToCheck & (NumberToCheck - 1)) == 0;
}

#pragma endregion