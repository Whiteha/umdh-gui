#ifndef UMDH_DATAMODEL_GLOBAL
#define UMDH_DATAMODEL_GLOBAL

#if defined(UMDH_DATAMODEL_LIBRARY)
#  define UMDH_DATAMODEL_EXPORT __declspec(dllexport)
#else
#  define UMDH_DATAMODEL_EXPORT __declspec(dllimport)
#endif

#endif // UMDH_DATAMODEL_GLOBAL
