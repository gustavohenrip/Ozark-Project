#pragma once
#include "stdafx.hpp"
#include "security/security.hpp"

namespace Auth {
	bool Run();
	bool Heartbeat();
	void ProcessCachedMetrics();
	void ProcessMetric(Security::Metrics::Metric* Metric, std::string Info);
}