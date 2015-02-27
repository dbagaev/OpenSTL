#pragma once

#ifndef __OPENSTL_DATA_EXPORT
#ifdef __BUILDING_OPENSTL_DATA
#define __OPENSTL_DATA_EXPORT __declspec(dllexport)
#else
#define __OPENSTL_DATA_EXPORT __declspec(dllimport)
#endif
#endif

#ifdef _MSC_VER
#pragma warning (disable : 4251)
#endif  // #ifdef MSVC