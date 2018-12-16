#include "Timer.h"

#include <sstream>

using namespace osomi;
using namespace std;

Timer::Timer(double _gtPSec, double startTime): gtPSec(_gtPSec), gt(startTime){}
void Timer::update(double delta) {
	gt += delta * gtPSec;
}
string Timer::getTimestamp() {
	stringstream ss;
	ss << getUGameHours() << ":" << getUGameMinutes() << ":" << getUGameSeconds() << " " << (getUGameDays()+1) << " " << (getUGameMonths()+1) << ";" << (getUGameWeeks()+1) << " " << (2017 + getUGameYears());
	return ss.str();
}
int Timer::interval(unsigned int max, double firstInterval) {
	if (max == 0)max = 1;
	float f = getUGameHours() + getUGameMinutes() / 60.f + getUGameSeconds() / 60.f / 60.f;
	float pI = 24.f / max;
	f = Math::mod(abs(f / pI - firstInterval / pI), max);

	if ((int)f == 1 && max == 2)return 0;

	return (int)f;
}
float Timer::value(unsigned int max, double firstInterval){
	if (max == 0)max = 1;
	float f = getUGameHours() + getUGameMinutes() / 60.f + getUGameSeconds() / 60.f / 60.f;
	if (f == 0)f = 0.0000001f;
	float pI = 24.f / max;
	f = Math::mod(abs(f / pI - firstInterval / pI), max);

	return f - (int)f;
}
float Timer::sunIntensity() {
	return (sin((getFGameHours() - 6) * M_PI / 12) + 1) / 2;
}