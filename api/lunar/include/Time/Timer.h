#pragma once

#include <string>
#include "../Math/Math.h"

namespace osomi {
	class Timer {
	private:
		double gtPSec;
		double gt;
	public:
		double getTime() { return gt / gtPSec; }
		double getGameTime() { return gt; }
		double getGameMinutes() { return gt / 60; }
		double getGameHours() { return getGameMinutes() / 60; }
		double getGameDays() { return getGameHours() / 24; }
		double getGameWeeks() { return getGameDays() / 7; }
		double getGameYears() { return getGameDays() / 365.25; }
		double getGameMonths() { return getGameYears() * 12; }

		unsigned int getUGameSeconds() { return (unsigned int)(Math::mod(gt, 60)); }
		unsigned int getUGameMinutes() { return (unsigned int)(Math::mod(floor(gt / 60), 60)); }
		unsigned int getUGameHours() { return (unsigned int)(Math::mod(floor(floor(gt/60)/60), 24));}
		unsigned int getUGameDays() { return (unsigned int)(Math::mod(floor(floor(floor(gt / 60) / 60) / 24), 7)); }
		unsigned int getUGameWeeks() { return (unsigned int)(Math::mod(floor(floor(floor(floor(gt / 60) / 60) / 24) / 7), 365.25/12/7)); }
		unsigned int getUGameMonths() { return (unsigned int)(Math::mod(floor(floor(floor(floor(floor(gt / 60) / 60) / 24) / 7) / (365.25 / 12 / 7)), 365.25)); }
		unsigned int getUGameYears() { return (unsigned int)(floor(floor(floor(floor(floor(floor(gt / 60) / 60) / 24) / 7) / (365.25 / 12 / 7)) / 365.25)); }
		
		float getFGameSeconds() { return Math::mod(gt, 60); }
		float getFGameMinutes() { return getUGameMinutes() + getFGameSeconds() / 60; }
		float getFGameHours() { return getUGameHours() + getFGameMinutes() / 60; }
		float getFGameDays() { return getUGameDays() + getFGameHours() / 24; }
		float getFGameWeeks() { return getUGameWeeks() + getFGameDays() / 7; }

		double getGTPS() { return gtPSec; }

		Timer(double gtPSec, double startTime);
		void update(double delta);
		std::string getTimestamp();
		int interval(unsigned int max, double firstInterval);
		float value(unsigned int max, double firstInterval);
		float sunIntensity();
	};
}