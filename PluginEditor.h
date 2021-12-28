/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "MyDelay.h"

using namespace juce;

//==============================================================================
/**
*/
class NewProjectAudioProcessorEditor : public juce::AudioProcessorEditor,
    public juce::Slider::Listener,
    public juce::Button::Listener
{
public:
    NewProjectAudioProcessorEditor(NewProjectAudioProcessor&);
    ~NewProjectAudioProcessorEditor() override;
    void juce::Slider::Listener::sliderValueChanged(Slider* slider) override;
    void juce::Button::Listener::buttonClicked(Button*) override;
    void initiateComponents(NewProjectAudioProcessor&);
    void printComponents();
    ScopedPointer <AudioProcessorValueTreeState::SliderAttachment> slider_attach;
    void paint(juce::Graphics&) override;
    void resized() override;


private:

    juce::Slider m_input_gain;
    juce::Label m_input_gain_label;
    juce::Slider m_output_gain;
    juce::Label m_output_gain_label;

    Slider m_volume_dials[NUM_OF_INSTENCES];
    Slider m_pan_dials[NUM_OF_INSTENCES];
    ToggleButton m_on_off_buttons[NUM_OF_INSTENCES];
    ToggleButton m_reverb_buttons[NUM_OF_INSTENCES];

    juce::Label m_volume_dials_label;
    juce::Label m_pan_dials_label;
    juce::Label m_on_off_buttons_label;
    juce::Label m_reverb_buttons_label;

    Slider m_delay_time;
    juce::Label m_delay_time_label;
    Slider m_delay_feedback;
    juce::Label m_delay_feedback_label;

    ImageComponent m_background;

    void reAlphaComponents();

    NewProjectAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(NewProjectAudioProcessorEditor)
};