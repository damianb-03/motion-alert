/*
 * cSound.h
 *
 *  Created on: 17 lis 2014
 */

#ifndef CSOUND_H_
#define CSOUND_H_

#include "libs.h"

#include "cSections.h"

#define MIN_CONF 5
//#define MIN_CONF 2
#define MAX_STANDARD_NOISE 250


typedef std::vector<double> samples;


class cSound {
public:
	cSound(bool s);
	cSound();
	virtual ~cSound();

	bool ProccessRecording(const sf::Int16* Samples, std::size_t SamplesCount, unsigned int SampleRate);
	const std::string currentDateTime();

	void sendXMPPNotificationAlarm(const std::string &mess);
	void alarm();

	const bool simulation_;


	struct alarmData {
		int from;
		int to;
		double sum;
		double avg;
		double max;
	};


private:
	const double minAlarm;
	static std::mutex mtx;
	static int n;
	bool wasAlarm;

	std::thread xmppScript;

	static std::stack <std::string> alarmsToSend;



	static void alarmHandler();


	double energy_;
	int confirmation;

	void createThreadForSendScript();
	void wait_for_key(); // used in plot function
	std::shared_ptr<alarmData> getSection(const samples &mag,  int from,  int to, unsigned int SampleRate, size_t N);
	int Interpret(const samples &mag, unsigned int SampleRate, size_t N); // hardcoded values for specific hardware
	bool IsInRange(double var, double from, double to);
	void convArrToDouble(double *toArr, const sf::Int16 *fromArr, const size_t &size);
	std::vector<double> calculateMagnitude(size_t fftw_size, const fftw_complex *out);
	std::vector<double> calculateFrequencies(unsigned int SampleRate, size_t fftw_size);
	void normalize(samples &mag, double maxMag, size_t fftw_size);
	void plotResults(const samples &x, const samples &y);
	bool detectAlarm(samples mag, unsigned int SampleRate, size_t fftw_size);
	bool hasMagHigh(samples mag);

	// plotResult();
	int autodetect(size_t fftw_size, const samples &freq, const samples &mag, unsigned int SampleRate); // old way to detect alarm, not used at now
};

#endif /* CSOUND_H_ */
