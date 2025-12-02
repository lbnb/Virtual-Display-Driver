#include "GRTWTrace.h"
#include "GTrace.h"

void GRTWTrace_beginSection(const char* tag) {
	UNREFERENCED_PARAMETER(tag);
	Trace::getInstance().beginSection(tag);
}

void GRTWTrace_endSection() {
	Trace::getInstance().endSection();
}

void GRTWTrace_setCounter(const char* tag, uint64_t val) {
	UNREFERENCED_PARAMETER(tag);
	UNREFERENCED_PARAMETER(val);
	Trace::getInstance().setCounter(tag, val);
}

void GRTWTrace_setThreadName(int32_t threadId, const char* threadName) {
	UNREFERENCED_PARAMETER(threadId);
	UNREFERENCED_PARAMETER(threadName);
	Trace::getInstance().setThreadName(threadId, threadName);
}
