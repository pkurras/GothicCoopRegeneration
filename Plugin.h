
// This file added in headers queue
// File: "Headers.h"

#include <chrono>

namespace GOTHIC_ENGINE
{
	class RegenerationTimer
	{
		public: RegenerationTimer(void)
			: StartTimepoint(std::chrono::high_resolution_clock::now())
		{ }
		public: void Reset(void)
		{
			StartTimepoint = std::chrono::high_resolution_clock::now();
		}
		public: double Elapsed(void) const
		{
			std::chrono::duration<double> elapsedTime = std::chrono::high_resolution_clock::now() - StartTimepoint;
			return elapsedTime.count();
		}
		private: std::chrono::time_point<std::chrono::high_resolution_clock> StartTimepoint;
	};
}
