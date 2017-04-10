#pragma once

#ifndef __mesh_CORE_EXPORT
#ifdef __BUILDING_mesh_CORE
#define __mesh_CORE_EXPORT __declspec(dllexport)
#else
#define __mesh_CORE_EXPORT __declspec(dllimport)
#endif
#endif