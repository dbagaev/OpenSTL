#pragma once

#ifdef _MSC_VER

#pragma warning (disable : 4251)

#ifndef __mesh_DATA_EXPORT
#ifdef __BUILDING_mesh_DATA
#define __mesh_DATA_EXPORT __declspec(dllexport)
#else
#define __mesh_DATA_EXPORT __declspec(dllimport)
#endif
#endif

#else

#define __mesh_DATA_EXPORT

#endif  // #ifdef MSVC
