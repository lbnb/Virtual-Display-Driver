#define _CRT_SECURE_NO_WARNINGS
#define GR_DEBUG
#include "GTrace.h"
#include <io.h>
#include <direct.h>
#include <string>
#include <sys/types.h>
#include <iostream>
#include <sstream>
#include <iomanip>


//blow is a simple trace content
//TRACE:
//JGDemoRoom - 7045  (6960)[006] ....  4903.777234: tracing_mark_write: B | 6960 | HIDL::IQtiMapperExtensions::getSize::passthrough
//JGDemoRoom - 7045  (6960)[006] ....  4903.777235 : tracing_mark_write : E | 6960

static constexpr const char* tracing_mark_write_b = ": tracing_mark_write: B|";
static constexpr const char* tracing_mark_write_e = ": tracing_mark_write: E|";
static std::atomic<bool> enableTrace = true;


Trace::Trace() {
	traceInit();
}

Trace::~Trace() {
	traceRelease();
}

void Trace::enableTraceFunc(bool enable) {
	enableTrace = enable;
}

Trace& Trace::getInstance() {
	static Trace instance;
	return instance;
};

//static int64_t GetTimeInNanoSeconds()
//{
//	LARGE_INTEGER frequency, counter;
//	if (!QueryPerformanceFrequency(&frequency))
//	{
//		printf("Error: QueryPerformanceFrequency failed.");
//		return -1;
//	}
//
//	if (!QueryPerformanceCounter(&counter))
//	{
//		printf("Error: QueryPerformanceCounter failed.");
//		return -1;
//	}
//	// Calculate the time in nanoseconds
//	int64_t timeInNanoSeconds = (counter.QuadPart * 1000000000) / frequency.QuadPart;
//	return timeInNanoSeconds;
//}

static LONGLONG GetTimePerformanceCounter() {
    LARGE_INTEGER counter;
    if (!QueryPerformanceCounter(&counter))
    {
        printf("Error: QueryPerformanceCounter failed.");
        return -1;
    }
    return counter.QuadPart;
}

void Trace::traceInit() {
    if (!QueryPerformanceFrequency(&frequency))
    {
        printf("Error: QueryPerformanceFrequency failed.");
    }
	mProcessId = GetCurrentProcessId();
	mProcessIdTag = std::to_string(mProcessId);
	mProcessTag = std::to_string(mProcessId) + "|";
	GetLocalTime(&t_TraceStart);
	nanoTraceStartTs = GetTimePerformanceCounter();
	createTraceFile();
	writeTraceHead();
	mTraceThread = new std::thread(traceLoop, this);
}

uint32_t Trace::getTraceCompareTime(SYSTEMTIME& tv) {
	return tv.wHour * 60 * 60 * 1000 + tv.wMinute * 60 * 1000 + tv.wSecond * 1000 + tv.wMilliseconds;
}

void Trace::traceRelease() {
	isThreadStop = true;
	if (mTraceThread->joinable())
	{
		mTraceThread->join();
	}

	if (mTraceFile != nullptr)
	{
		fflush(mTraceFile);
		fclose(mTraceFile);
	}
}

void Trace::createTraceFile() {
	if (mTraceFile == nullptr) {
		if (_access("C:\\VirtualDisplayDriver\\Trace", 0) == -1) {
			_mkdir("C:\\VirtualDisplayDriver\\Trace");
		}
		time_t now = time(NULL);
		struct tm *t = localtime(&now);
		char filename[128];
		sprintf(filename, "C:\\VirtualDisplayDriver\\Trace\\Trace_%02d%02d_%02d%02d%02d.html", t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);
		fopen_s(&mTraceFile, filename, "wb");
	}
}


void Trace::traceLoop(void* ptr) {
	Trace* pThis = (Trace*)ptr;
	std::string content;
	char microsecond[20];
	TraceEntity tmpEntity;
	content.reserve(512);
	while (true) {
		if (!pThis->isThreadStop.load())
		{
			pThis->traceWrite(tmpEntity, microsecond, content);
		}
		else
		{
			break;
		}
	}
}

void Trace::writeTraceHead() {
	//todo write head
	std::string headInfo("TRACE:\n");
	fwrite(headInfo.c_str(), 1, headInfo.length(), mTraceFile);
}

//wrtie trace on same thread to ensure high resolution clock accuracy
void Trace::traceWrite(TraceEntity& tmpEntity, char* microsecond, std::string& content) {
	content.clear();
	size_t queueSize = 0;
	{
		std::unique_lock<std::mutex> lck(mEntityLock);
		queueSize = mEntityQueue.size();
		if (mEntityQueue.size() > 0)
		{
			tmpEntity = mEntityQueue.front();
			mEntityQueue.pop();
		}
	}
	if (queueSize == 0)
	{
		//wait to be notify
		//fflush(mTraceFile);
		std::unique_lock<std::mutex> lck(mEntityLock);
		mEnityCondition.wait(lck);
		return;
	}

	if (tmpEntity.type == TYPE_BEGIN)
	{
        auto f_tmp = (float)(tmpEntity.ts - nanoTraceStartTs) / frequency.QuadPart;
		sprintf(microsecond, "%.6f", f_tmp);
		content.append(getThreadName(tmpEntity.threadId))
			.append(microsecond)
			.append(tracing_mark_write_b)
			.append(mProcessTag)
			.append(tmpEntity.tag)
			.append("\n");
	}
	else if (tmpEntity.type == TYPE_END)
	{
        auto f_tmp = (float)(tmpEntity.ts - nanoTraceStartTs) / frequency.QuadPart;
		sprintf(microsecond, "%.6f", f_tmp);
		content.append(getThreadName(tmpEntity.threadId))
			.append(microsecond)
			.append(tracing_mark_write_e)
			.append(mProcessIdTag)
			.append("\n");
	}
	else if (tmpEntity.type == TYPE_COUNT)
	{
        auto f_tmp = (float)(tmpEntity.ts - nanoTraceStartTs) / frequency.QuadPart;
		sprintf(microsecond, "%.6f", f_tmp);
		content.append(getThreadName(tmpEntity.threadId))
			.append(microsecond)
			.append(": tracing_mark_write: C|")
			.append(mProcessTag)
			.append(tmpEntity.tag)
			.append("|")
			.append(std::to_string(tmpEntity.count))
			.append("\n");
	}
	fwrite(content.c_str(), 1, content.length(), mTraceFile);
}

void Trace::beginSection(const char* name) {
	std::unique_lock<std::mutex> lck(mEntityLock);
	TraceEntity entity{ TYPE_BEGIN, "", GetCurrentThreadId(),0, GetTimePerformanceCounter() };
	strncpy(entity.tag, name, MAX_TAG_SIZE);
	mEntityQueue.push(entity);
	//reduce lock time
	//mEnityCondition.notify_one();
}

void Trace::endSection() {
	std::unique_lock<std::mutex> lck(mEntityLock);
	TraceEntity entity{ TYPE_END, "" , GetCurrentThreadId(), 0, GetTimePerformanceCounter() };
	mEntityQueue.push(entity);
	mEnityCondition.notify_one();
}

void Trace::setCounter(const char* name, uint64_t val) {
	std::unique_lock<std::mutex> lck(mEntityLock);
	TraceEntity entity{ TYPE_COUNT, "" , GetCurrentThreadId() , val, GetTimePerformanceCounter() };
	strncpy(entity.tag, name, MAX_TAG_SIZE);
	mEntityQueue.push(entity);
	mEnityCondition.notify_one();
}

void Trace::setThreadName(int threadId, std::string threadName) {
	std::lock_guard<std::mutex> lck(mThreadNameLock);
    if (threadId == 0) {
        threadId = (int)GetCurrentThreadId();
    }
	//Avoid unnecessary string concatenation
	mThreadNameMap[threadId] = threadName + "-" + std::to_string(threadId).append("  ( ")
		.append(std::to_string(mProcessId))
		.append(") [006] ....  ");//cpu;
}

std::string& Trace::getThreadName(int threadId) {
	std::lock_guard<std::mutex> lck(mThreadNameLock);
	if (mThreadNameMap.find(threadId) != mThreadNameMap.end())
	{
		return mThreadNameMap[threadId];
	}
	mThreadNameMap[threadId] = "GRTW-" + std::to_string(threadId).append("  ( ")
		.append(std::to_string(mProcessId))
		.append(") [006] ....  ");//cpu;;
	return mThreadNameMap[threadId];
}
