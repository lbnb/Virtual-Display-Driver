#pragma once

#include <stdint.h>

// #define ENABLE_GRTW_TRACE

#ifdef __cplusplus
extern "C" {
#endif

void GRTWTrace_beginSection(const char *tag);
void GRTWTrace_endSection();
void GRTWTrace_setCounter(const char* tag, uint64_t val);
void GRTWTrace_setThreadName(int32_t threadId, const char* threadName);

struct ScopedTrace {
	ScopedTrace(const char* name) {
		GRTWTrace_beginSection(name);
	}

	~ScopedTrace() {
		GRTWTrace_endSection();
	}
};

#define PASTE_HELPER_HELPER(a, b) a ## b
#define PASTE_HELPER(a, b) PASTE_HELPER_HELPER(a, b)

#ifdef ENABLE_GRTW_TRACE
#define TRACE_CALL(name) ScopedTrace PASTE_HELPER(scopedTrace, __LINE__)(name)
#define AUTO_TRACE_CALL() ScopedTrace PASTE_HELPER(scopedTrace, __LINE__)(__FUNCTION__)
#else
#define TRACE_CALL(name)
#define AUTO_TRACE_CALL()
#endif

#ifdef __cplusplus
}
#endif