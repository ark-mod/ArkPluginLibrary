#pragma once
#include "API/Ark/Ark.h"

namespace ArkExtensions
{
	#define ANY_PACKAGE ((UObject*)-1)

	struct Globals
	{
		static UObject* StaticFindObject(UClass *ObjectClass, UObject *InObjectPackage, const wchar_t *OrigInName, bool ExactClass)
		{
			return NativeCall<UObject*, UClass *, UObject *, const wchar_t *, bool>(
				nullptr, "Global.StaticFindObject", ObjectClass, InObjectPackage, OrigInName, ExactClass);
		}
	};
}