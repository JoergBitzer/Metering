
#include "SimpleMeter.h"

SimpleMeter::SimpleMeter()
:m_fs(44100.0),m_blockSize(1024),m_tauAttRMS_ms(10.0),m_tauRelRMS_ms(300.0),m_holdtime_ms(3000.0)
{

    m_rms.resize(m_maxnrofchannels);
    m_peak.resize(m_maxnrofchannels);
    m_peakholdcounter.resize(m_maxnrofchannels);
    reset();
    computeTimeConstants();
}
void SimpleMeter::prepareToPlay (float samplerate, int SamplesPerBlock)
{
    m_fs = samplerate;
    m_rms.resize(m_maxnrofchannels);
    std::fill(m_rms.begin(), m_rms.end(), 0.0);
    m_peak.resize(m_maxnrofchannels);
    std::fill(m_peak.begin(), m_peak.end(), 0.0);
    m_peakholdcounter.resize(m_maxnrofchannels);
    std::fill(m_peakholdcounter.begin(), m_peakholdcounter.end(), 0.0);
    reset();
    computeTimeConstants();
}
void SimpleMeter::analyseData (juce::AudioBuffer<float>& data)
{
    size_t totalNrChannels = data.getNumChannels();
	for (size_t channel = 0; channel < totalNrChannels; ++channel)
	{
        auto* channelData = data.getWritePointer (channel);

        for (size_t idx = 0; idx < data.getNumSamples(); ++idx)
        {
            double in = fabs(channelData[idx]);
            // RMS first
            float alpha;

            if (in > m_rms[channel])
                alpha = m_alphaAttRMS;
            else
                alpha = m_alphaRelRMS;

            m_rms[channel] *= alpha;
            m_rms[channel] += (1.0-alpha)*in*in;

            // peak
            if (in > m_peak[channel])
            {
                m_peak[channel] = in;
                m_peakholdcounter[channel] = m_holdtime_samples;
            }
            else
            {
                m_peakholdcounter[channel]--;
                if (m_peakholdcounter[channel] < 0)
                    m_peak[channel] *= m_alphaRelRMS;

            }
            
        }
	}


}
void SimpleMeter::computeTimeConstants()
{
    m_alphaAttRMS = exp(-1.0/(m_tauAttRMS_ms*0.001*m_fs));
    m_alphaRelRMS = exp(-1.0/(m_tauRelRMS_ms*0.001*m_fs));

    m_holdtime_samples = static_cast<int>(m_holdtime_ms*0.001*m_fs);

}
void SimpleMeter::reset()
{
    std::fill(m_rms.begin(), m_rms.end(), 0.0);
    std::fill(m_peak.begin(), m_peak.end(), 0.0);
    std::fill(m_peakholdcounter.begin(), m_peakholdcounter.end(), 0.0);
}

int SimpleMeter::getAnalyserData(std::vector<float>& rms, std::vector<float>& peak)
{
    size_t nrofchannels = m_rms.size();
    rms.resize(nrofchannels);
    peak.resize(nrofchannels);
    for (size_t kk = 0 ; kk < nrofchannels; ++kk)
    {
        rms[kk] = m_rms[kk];
        peak[kk] = m_peak[kk];
    }
    return nrofchannels;
}


SimpleMeterComponent::SimpleMeterComponent(SimpleMeter &meter)
:m_scaleFactor(1.f),m_meter(meter)
{
    m_rms.resize(8);
    m_peak.resize(8);
    std::fill(m_rms.begin(), m_rms.end(), 0.0);
    std::fill(m_peak.begin(), m_peak.end(), 0.0);

    startTimer (20);
}
void SimpleMeterComponent::paint(Graphics& g)
{
   g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::red);
    g.setFont (m_scaleFactor*15.0f);
    g.drawFittedText (String(10*log10(m_rms[0] + 0.00000000001)) + " db", getLocalBounds(), juce::Justification::centred, 1);
}    

void SimpleMeterComponent::resized()
{
    
}


