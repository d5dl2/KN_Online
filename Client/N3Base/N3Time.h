#pragma once
#include <chrono>

class CN3Time
{
public:
	static constexpr long long TimeGetMs()
	{
		auto duration = std::chrono::high_resolution_clock::now().time_since_epoch();
		auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(duration);

		return milliseconds.count();
	}
};