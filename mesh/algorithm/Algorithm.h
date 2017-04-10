#pragma once

#ifndef __mesh_ALGORITHM_EXPORT
#ifdef __BUILDING_mesh_ALGORITHM
#define __mesh_ALGORITHM_EXPORT __declspec(dllexport)
#else
#define __mesh_ALGORITHM_EXPORT __declspec(dllimport)
#endif
#endif