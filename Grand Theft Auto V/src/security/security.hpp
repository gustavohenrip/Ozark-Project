#pragma once
#include "stdafx.hpp"
#include "structs.hpp"

namespace Security {
	namespace Metrics {
        enum eMetricID : uint8_t {
            METRIC_BAD,
            METRIC_THREAD_CREATED_IN_UNMAPPED_MEMORY,	// i.e co-loading
            METRIC_DEBUGGING,
            METRIC_THREAD_BEING_DEBUGGED,
            METRIC_FUNCTION_INTEGRITY_FAILED,
            METRIC_BLACKLISTED_PROCESS,
            METRIC_MODULE_DIGEST_MISMATCH,
            METRIC_CRASHED,
        };

        class Metric {
        public:
            virtual eMetricID GetMetricID();
            virtual void OnEvent();
        };

        class MetricThreadCreatedInUnmappedMemory : public Metric {
        public:
            eMetricID GetMetricID();
        };

        class MetricDebugging : public Metric {
        public:
            eMetricID GetMetricID();
            void OnEvent();
        };

        class MetricThreadBeingDebugged : public Metric {
        public:
            eMetricID GetMetricID();
            void OnEvent();
        };

        class MetricFunctionIntegrityFailed : public Metric {
        public:
            eMetricID GetMetricID();
            void OnEvent();
        };

        class MetricBlacklistedProcess : public Metric {
        public:
            eMetricID GetMetricID();
        };

        class MetricModuleDigestMismatch : public Metric {
        public:
            eMetricID GetMetricID();
            void OnEvent();
        };

        class MetricCrashed : public Metric {
        public:
            eMetricID GetMetricID();
        };
	}

    bool Initialize();
    bool UpdateModuleDigest(HMODULE hModule, std::vector<uint8_t> PE);
    std::vector<Structs::ImageSection> EnumMemorySections(HMODULE hModule, std::vector<uint8_t> PE);
    void CloseProcess();
    void Bluescreen();
    void Unlink();
    void Relink();
    void FakePE();
}