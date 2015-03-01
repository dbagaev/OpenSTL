#pragma once

#ifdef _MSC_VER

#pragma warning (disable : 4251)

#ifndef __OPENSTL_DATA_EXPORT
#ifdef __BUILDING_OPENSTL_DATA
#define __OPENSTL_DATA_EXPORT __declspec(dllexport)
#else
#define __OPENSTL_DATA_EXPORT __declspec(dllimport)
#endif
#endif

#else

#define __OPENSTL_DATA_EXPORT

#endif  // #ifdef MSVC
