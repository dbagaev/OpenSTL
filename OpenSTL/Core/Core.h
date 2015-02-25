#pragma once

#ifndef __OPENSTL_CORE_EXPORT
#ifdef __BUILDING_OPENSTL_CORE
#define __OPENSTL_CORE_EXPORT __declspec(dllexport)
#else
#define __OPENSTL_CORE_EXPORT __declspec(dllimport)
#endif
#endif