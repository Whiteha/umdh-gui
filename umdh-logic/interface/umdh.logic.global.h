#ifndef UMDH_LOGIC_GLOBAL
#define UMDH_LOGIC_GLOBAL

#if defined(UMDH_LOGIC_LIBRARY)
#  define UMDH_LOGIC_EXPORT __declspec(dllexport)
#else
#  define UMDH_LOGIC_EXPORT __declspec(dllimport)
#endif

#endif // UMDH_LOGIC_GLOBAL
