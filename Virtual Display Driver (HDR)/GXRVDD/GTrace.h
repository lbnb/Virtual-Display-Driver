#pragma once
#include <memory>
#include <windows.h>
#include <queue>
#include <mutex>
#include <thread>
#include <unordered_map>

#define MAX_TAG_SIZE 50

enum TYPE_TRACE {
	TYPE_BEGIN,
	TYPE_END,
	TYPE_COUNT,
};

typedef struct _TraceEnity {
    enum TYPE_TRACE type;
	char tag[MAX_TAG_SIZE];
	DWORD threadId;
    uint64_t count;
    LONGLONG ts;
}TraceEntity;

class Trace {

public:

	Trace();
	~Trace();
	Trace(const Trace&) = delete;
	Trace& operator=(const Trace&) = delete;

	void beginSection(const char* name);
	void endSection();
	void setCounter(const char* tag, uint64_t val);
	void setThreadName(int threadId, std::string threadName);
	//enable resource monitor will lost some performance
	//so disable default

private:
	void traceInit();
	void traceRelease();
	void createTraceFile();
	void writeTraceHead();
	uint32_t getTraceCompareTime(SYSTEMTIME& tv);
	FILE* mTraceFile = nullptr;
	SYSTEMTIME t_TraceStart;
    LONGLONG nanoTraceStartTs;
    LARGE_INTEGER frequency;

	static void traceLoop(void* ptr);
	void traceWrite(TraceEntity& tmpEntity, char* microsecond, std::string& content);
	std::string& getThreadName(int threadId);

	std::mutex mEntityLock;
	std::condition_variable mEnityCondition;
	std::queue<TraceEntity> mEntityQueue;
	std::thread* mTraceThread;
	DWORD mProcessId;
	//reduce string convert
	std::string mProcessIdTag;
	std::string mProcessTag;
	std::mutex mThreadNameLock;
	std::unordered_map<int, std::string> mThreadNameMap;
	std::unordered_map<int, std::string> mBlackThreadNameMap;
	std::atomic<bool> isThreadStop = false;

public:
	static Trace& getInstance();
	static void enableTraceFunc(bool enable);
};
