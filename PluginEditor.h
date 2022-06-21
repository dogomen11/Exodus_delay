/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

using namespace juce;
using namespace std;
//==============================================================================
/**
*/
class ExodusAudioProcessorEditor :  public juce::AudioProcessorEditor,
                                    public juce::Button::Listener,
                                    public juce::Timer
                                    
{
public:
    ExodusAudioProcessorEditor (ExodusAudioProcessor&);
    ~ExodusAudioProcessorEditor() override;

    void juce::Button::Listener::buttonClicked(Button*) override;
    void reAlphaComponents();
    void initiateComponents(AudioProcessor&);
    void printComponents();
    void paint (juce::Graphics&) override;
    void resized() override;

private:

    class timer_indicator : public Component
    {
        public:
            timer_indicator() = default;
            ~timer_indicator() = default;
            void paint(Graphics& g) override {
                Rectangle<float> timer_area(84, 650, 20, 20);
                g.setColour(Colours::blueviolet);
                g.drawEllipse(timer_area, 2.0);
            }
            void resized() override { }
        private:
            JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(timer_indicator)
    };

    timer_indicator ti_instence;
    void timerCallback() override;

    double winSizeRatio;

    ToggleButton indicator_debug;

    Slider m_input_gain;
    Label m_input_gain_label;
    Slider m_output_gain;
    Label m_output_gain_label;

    Slider m_volume_dials[NUM_OF_INSTENCES];
    Slider m_pan_dials[NUM_OF_INSTENCES];
    ToggleButton m_on_off_buttons[NUM_OF_INSTENCES];
    ToggleButton m_reverb_buttons[NUM_OF_INSTENCES];

    Label m_volume_dials_label;
    Label m_pan_dials_label;
    Label m_on_off_buttons_label;
    Label m_reverb_buttons_label;

    Slider m_delay_time;
    Label m_delay_time_label;
    Slider m_delay_feedback;
    Label m_delay_feedback_label;
    Slider m_delay_mix;
    Label m_delay_mix_label;

    ImageComponent m_background;

    Slider m_filter_freq;
    Label m_filter_freq_label;
    Slider m_filter_res;
    Label m_filter_res_label;
    Slider m_filter_drive;
    Label m_filter_drive_label;

public:

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> m_input_gain_attach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> m_output_gain_attach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> m_delay_time_attach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> m_delay_feedback_attach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> m_delay_mix_attach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> m_filter_freq_attach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> m_filter_res_attach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> m_filter_drive_attach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> m_volume_dials_attach[NUM_OF_INSTENCES];
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> m_pan_dials_attach[NUM_OF_INSTENCES];
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> m_on_off_buttons_attach[NUM_OF_INSTENCES];
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> m_reverb_buttons_attach[NUM_OF_INSTENCES];

    ExodusAudioProcessor& audioProcessor;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ExodusAudioProcessorEditor)
};
