#pragma once

#include <vector>

#include "JuceHeader.h"

class SimpleMeter
{
public:
    SimpleMeter();
    void prepareToPlay (float samplerate, int SamplesPerBlock);
    void analyseData (juce::AudioBuffer<float>& data);

    int getAnalyserData(std::vector<float>& rms, std::vector<float>& peak);

private:
    float m_fs;
    int m_blockSize;
    const size_t m_maxnrofchannels = 8;

    double m_tauAttRMS_ms;
    double m_alphaAttRMS;
    double m_tauRelRMS_ms;
    double m_alphaRelRMS;

    int m_holdtime_ms;
    int m_holdtime_samples;

    std::vector<float> m_rms;
    std::vector<float> m_peak;
    std::vector<float> m_peakholdcounter;

    void computeTimeConstants();
    void reset();
};

class SimpleMeterComponent : public Component, public Timer
{
public:
    SimpleMeterComponent(SimpleMeter &meter);
    ~SimpleMeterComponent(){stopTimer();};
	void paint(Graphics& g) override;
	void resized() override;
    void setScaleFactor(float newscale){m_scaleFactor = newscale;};
    void timerCallback() override
    {
        m_displaychannels = m_meter.getAnalyserData(m_rms,m_peak);
        repaint();
    }
private:
    SimpleMeter& m_meter;
    float m_scaleFactor;
    std::vector<float> m_rms;
    std::vector<float> m_peak;
    int m_displaychannels; 

};