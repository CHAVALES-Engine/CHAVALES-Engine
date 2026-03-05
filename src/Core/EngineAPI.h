#pragma once


#ifdef ENGINE_EXPORTS
#define ENGINE_API __declspec(dllexport)	// exportando desde engine
#else
#define ENGINE_API __declspec(dllimport)	// consumiendo desde fuera
#endif