#pragma once

#ifndef __OPENSTL_ALGORITHM_EXPORT
#ifdef __BUILDING_OPENSTL_ALGORITHM
#define __OPENSTL_ALGORITHM_EXPORT __declspec(dllexport)
#else
#define __OPENSTL_ALGORITHM_EXPORT __declspec(dllimport)
#endif
#endif