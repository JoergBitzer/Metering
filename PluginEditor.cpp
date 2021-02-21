#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "PlugInGUISettings.h"

//==============================================================================
#if WITH_MIDIKEYBOARD   
SimpleMeterProcessorEditor::SimpleMeterProcessorEditor (SimpleMeterProcessor& p)
    : AudioProcessorEditor (&p), m_processorRef (p), m_presetGUI(p.m_presets),
    	m_keyboard(m_processorRef.m_keyboardState, MidiKeyboardComponent::Orientation::horizontalKeyboard)
#else
SimpleMeterProcessorEditor::SimpleMeterProcessorEditor (SimpleMeterProcessor& p)
    : AudioProcessorEditor (&p), m_processorRef (p), m_presetGUI(p.m_presets),
    m_metercomponent(p.m_meter)
#endif
{

    setResizeLimits (g_minGuiSize_x,g_minGuiSize_x*g_guiratio , g_maxGuiSize_x, g_maxGuiSize_x*g_guiratio);
    getConstrainer()->setFixedAspectRatio(1./g_guiratio);
    setSize (g_minGuiSize_x, g_minGuiSize_x*g_guiratio);

	addAndMakeVisible(m_presetGUI);
#if WITH_MIDIKEYBOARD      
	addAndMakeVisible(m_keyboard);
#endif

    addAndMakeVisible(m_metercomponent);

}

SimpleMeterProcessorEditor::~SimpleMeterProcessorEditor()
{
}

//==============================================================================
void SimpleMeterProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
 	g.fillAll((getLookAndFeel().findColour(ResizableWindow::backgroundColourId)).darker(0.1));
}
const int g_minPresetHandlerHeight(30);
const float g_midikeyboardratio(0.13);
void SimpleMeterProcessorEditor::resized()
{
    int height = getHeight();
    // necessary to change fontsize of comboboxes and PopUpmenus
    // 0.5 is a good compromisecould be slightly higher or lower
    m_jadeLAF.setFontSize(0.5*height*g_minPresetHandlerHeight/g_minGuiSize_y);
    // top presethandler
    m_presetGUI.setBounds(0, 0, getWidth(), height*g_minPresetHandlerHeight/g_minGuiSize_y);
    // bottom a small midkeyboard
#if WITH_MIDIKEYBOARD    
    m_keyboard.setBounds(0, height*(1-g_midikeyboardratio), getWidth(), height*g_midikeyboardratio);
#endif
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    int width = getWidth();
    float scaleFactor = float(width)/g_minGuiSize_x;
    m_metercomponent.setBounds(scaleFactor*10,scaleFactor*60,scaleFactor*120,scaleFactor*40);


}
