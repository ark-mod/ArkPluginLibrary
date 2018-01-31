#pragma once
#include "API/Ark/Ark.h"

inline ClassCastFlags operator|(ClassCastFlags first, ClassCastFlags second)
{
	return static_cast<ClassCastFlags>(static_cast<std::underlying_type_t<ClassCastFlags>>(first) | static_cast<std::underlying_type_t<ClassCastFlags>>(second));
}

inline ClassCastFlags operator|(ClassCastFlags first, unsigned long long second)
{
	return static_cast<ClassCastFlags>(static_cast<std::underlying_type_t<ClassCastFlags>>(first) | second);
}

inline ClassCastFlags operator&(ClassCastFlags first, ClassCastFlags second)
{
	return static_cast<ClassCastFlags>(static_cast<std::underlying_type_t<ClassCastFlags>>(first) & static_cast<std::underlying_type_t<ClassCastFlags>>(second));
}

inline ClassCastFlags operator&(ClassCastFlags first, unsigned long long second)
{
	return static_cast<ClassCastFlags>(static_cast<std::underlying_type_t<ClassCastFlags>>(first) & second);
}

inline bool operator==(ClassCastFlags first, unsigned long long second)
{
	return static_cast<std::underlying_type_t<ClassCastFlags>>(first) == second;
}

inline bool operator!=(ClassCastFlags first, unsigned long long second)
{
	return static_cast<std::underlying_type_t<ClassCastFlags>>(first) != second;
}