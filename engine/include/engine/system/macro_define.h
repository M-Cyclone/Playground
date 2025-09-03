#pragma once

#ifndef ENGINE_DEBUG
#define ENGINE_DEBUG 0
#endif

static constexpr bool bIsEngineDebug = (ENGINE_DEBUG != 0);

#ifndef ENGINE_FORCE_INLINE
#define ENGINE_FORCE_INLINE inline
#endif
