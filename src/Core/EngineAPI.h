#pragma once
#pragma once

#ifdef ENGINE_EXPORTS
#define ENGINE_API __declspec(dllexport)	// exportando desde engine
#elif defined(CORE_BUILDING)
#define ENGINE_API							// exportando desde core (no hace nada) 
#else
#define ENGINE_API __declspec(dllimport)	// consumiendo desde fuera
#endif