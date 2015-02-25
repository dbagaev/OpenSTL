#pragma once

#ifndef __OPENSTL_DATA_EXPORT
#ifdef __BUILDING_OPENSTL_DATA
#define __OPENSTL_DATA_EXPORT __declspec(dllexport)
#else
#define __OPENSTL_DATA_EXPORT __declspec(dllimport)
#endif
#endif