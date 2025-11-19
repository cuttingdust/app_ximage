#ifndef XPLATFORM_GLOBAL_H
#define XPLATFORM_GLOBAL_H

#ifdef _WIN32

#ifdef xplatform_STATIC
#define XPLATFORM_EXPORT
#else
#ifdef xplatform_EXPORTS
#define XPLATFORM_EXPORT __declspec(dllexport)
#else
#define XPLATFORM_EXPORT __declspec(dllimport)
#endif
#endif
#else
#define XPLATFORM_EXPORT
#endif

#include <memory>

#endif // XPLATFORM_GLOBAL_H
