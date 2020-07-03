// Free to play. By SoNa Games.


#include "CountessEditorUtilityWidget.h"
#include "EditorLevelLibrary.h"

#pragma region OrganizeWorldOutliner

void UCountessEditorUtilityWidget::OrganizeWorldOutliner()
{
	TArray<AActor*> AllActors = UEditorLevelLibrary::GetAllLevelActors();
	for (AActor* Actor : AllActors)
	{
		if (ensure(Actor))
		{
			const FName* FolderName = FolerMap.Find(Actor->GetClass());
			if (ensure(FolderName))
			{
				Actor->SetFolderPath(*FolderName);
			}
		}
	}
}

#pragma endregion