/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

#define TRANSPARENT 0.5f
#define BOLD 1.0f

void NewProjectAudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &m_input_gain)
    {
        audioProcessor.m_input_gain = m_input_gain.getValue();
    }
    else if (slider == &m_output_gain)
    {
        audioProcessor.m_output_gain = m_output_gain.getValue();
    }
    else if (slider == &m_delay_time)
    {
        audioProcessor.m_delay_time = m_delay_time.getValue();
    }
    else if (slider == &m_delay_feedback)
    {
        audioProcessor.m_delay_feedback = m_delay_feedback.getValue();
    }
    else
    {
        for (int i = 0; i < 16; i++)
        {
            if (slider == &m_volume_dials[i])
            {
                audioProcessor.m_volume_dials[i] = m_volume_dials[i].getValue();
            }
            if (slider == &m_pan_dials[i])
            {
                audioProcessor.m_pan_dials[i] = m_pan_dials[i].getValue();
            }
        }
    }
}

void NewProjectAudioProcessorEditor::buttonClicked(Button* button)
{
    DBG("clicked");
    for (int i = 0; i < 16; i++)
    {
        if (button == &m_on_off_buttons[i])
        {
            audioProcessor.m_on_off_button_array[i] = m_on_off_buttons[i].getToggleState();
            switch (m_on_off_buttons[i].getToggleState())
            {
            case true:
                audioProcessor.marked++;
                break;
            case false:
                audioProcessor.marked--;
                break;
            }
        }
        else if (button == &m_reverb_buttons[i])
        {
            audioProcessor.m_reverb_button_array[i] = m_reverb_buttons[i].getToggleState();
        }
    }
    reAlphaComponents();

}

void NewProjectAudioProcessorEditor::reAlphaComponents()
{
    for (int i = 0; i < 16; i++)
    {
        if (audioProcessor.marked == 0)
        {
            m_volume_dials[i].setAlpha(TRANSPARENT);
            m_pan_dials[i].setAlpha(TRANSPARENT);
            m_on_off_buttons[i].setAlpha(TRANSPARENT);
            m_reverb_buttons[i].setAlpha(TRANSPARENT);
        }
        else
        {
            m_volume_dials[i].setAlpha(BOLD);
            m_pan_dials[i].setAlpha(BOLD);
            m_on_off_buttons[i].setAlpha(BOLD);
            m_reverb_buttons[i].setAlpha(BOLD);
        }
    }
}

//==============================================================================
NewProjectAudioProcessorEditor::NewProjectAudioProcessorEditor(NewProjectAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p)
{
    setSize(1200, 800);
    //TODO: check if need to change to use valueTree
    //slider_attach = new juce::AudioProcessorValueTreeState::SliderAttachment(audioProcessor.parameters, INPUT_GAIN_ID, m_input_gain);
    //slider_attach = new juce::AudioProcessorValueTreeState::SliderAttachment(audioProcessor.parameters, DELAY_MIX_ID, m_delay_feedback);
    //slider_attach = new juce::AudioProcessorValueTreeState::SliderAttachment(audioProcessor.parameters, OUTPUT_GAIN_ID, m_output_gain);
    initiateComponents(p);

}

NewProjectAudioProcessorEditor::~NewProjectAudioProcessorEditor()
{
}


void NewProjectAudioProcessorEditor::initiateComponents(NewProjectAudioProcessor& p)
{

    addAndMakeVisible(m_volume_dials_label);
    m_volume_dials_label.setText("vol", juce::dontSendNotification);
    addAndMakeVisible(m_pan_dials_label);
    m_pan_dials_label.setText("pan", juce::dontSendNotification);
    addAndMakeVisible(m_on_off_buttons_label);
    m_on_off_buttons_label.setText("on/off", juce::dontSendNotification);
    addAndMakeVisible(m_reverb_buttons_label);
    m_reverb_buttons_label.setText("reverb", juce::dontSendNotification);
    addAndMakeVisible(this->audioProcessor.m_visualiser);

    addAndMakeVisible(m_background);
    //*****************************************************************************
    addAndMakeVisible(m_input_gain);
    m_input_gain.setRange(-60.0f, 6.0f, 0.01f);
    m_input_gain.setSkewFactorFromMidPoint(0);
    m_input_gain.setValue(0.0f);
    m_input_gain.setTextValueSuffix("db");
    m_input_gain.setTextBoxStyle(Slider::TextBoxBelow, true, 80, 20);
    m_input_gain.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    m_input_gain.addListener(this);
    addAndMakeVisible(m_input_gain_label);
    m_input_gain_label.setText("input", juce::dontSendNotification);

    addAndMakeVisible(m_output_gain);
    m_output_gain.setRange(-60.0f, 6.0f, 0.01f);
    m_output_gain.setSkewFactorFromMidPoint(0);
    m_output_gain.setValue(0.0f);
    m_output_gain.setTextValueSuffix("db");
    m_output_gain.setTextBoxStyle(Slider::TextBoxBelow, true, 80, 20);
    m_output_gain.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    m_output_gain.addListener(this);
    addAndMakeVisible(m_output_gain_label);
    m_output_gain_label.setText("output", juce::dontSendNotification);

    addAndMakeVisible(m_delay_time);
    m_delay_time.setRange(0.0f, 1500.0f, 1.0f);
    m_delay_time.setValue(500.0f);
    m_delay_time.setTextValueSuffix("ms");
    m_delay_time.setTextBoxStyle(Slider::TextBoxBelow, true, 50, 20);
    m_delay_time.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    m_delay_time.addListener(this);
    addAndMakeVisible(m_delay_time_label);
    m_delay_time_label.setText("time", juce::dontSendNotification);

    addAndMakeVisible(m_delay_feedback);
    m_delay_feedback.setRange(0.0f, 0.8f, 0.01f);
    m_delay_feedback.setValue(0.4f);
    m_delay_feedback.setTextValueSuffix("%");
    m_delay_feedback.setTextBoxStyle(Slider::TextBoxBelow, true, 50, 20);
    m_delay_feedback.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    m_delay_feedback.addListener(this);
    addAndMakeVisible(m_delay_feedback_label);
    m_delay_feedback_label.setText("feedback", juce::dontSendNotification);
    //*****************************************************************************

    for (int i = 0; i < 16; i++)
    {
        addAndMakeVisible(m_volume_dials[i]);
        m_volume_dials[i].setRange(0.0f, 1.0f, 0.01f);
        m_volume_dials[i].setValue(1.0f);
        m_volume_dials[i].setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
        m_volume_dials[i].setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
        m_volume_dials[i].addListener(this);
        m_volume_dials[i].setOpaque(false);
        if (audioProcessor.marked == 0) { m_volume_dials[i].setAlpha(TRANSPARENT); }

        addAndMakeVisible(m_pan_dials[i]);
        m_pan_dials[i].setRange(-1.0f, 1.0f, 0.01f);
        m_pan_dials[i].setValue(0.0f);
        m_pan_dials[i].setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
        m_pan_dials[i].setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
        m_pan_dials[i].addListener(this);
        m_pan_dials[i].setOpaque(false);
        if (audioProcessor.marked == 0) { m_pan_dials[i].setAlpha(TRANSPARENT); }

        addAndMakeVisible(m_on_off_buttons[i]);
        m_on_off_buttons[i].addListener(this);

        addAndMakeVisible(m_reverb_buttons[i]);
        m_reverb_buttons[i].addListener(this);
    }


}


void NewProjectAudioProcessorEditor::printComponents()
{
    int dials_horizontal_distance = 66;
    int dials_distance_from_edeg = 68;
    int size_of_dial = 62;


    m_input_gain.setBounds(13, 5, 80, 80);
    m_input_gain_label.setBounds(30, 70, 80, 50);
    m_output_gain.setBounds(1110, 5, 80, 80);
    m_output_gain_label.setBounds(1127, 70, 70, 50);
    m_pan_dials_label.setBounds(15, 510, 80, 50);
    m_volume_dials_label.setBounds(15, 580, 80, 50);
    m_on_off_buttons_label.setBounds(15, 650, 80, 50);
    m_reverb_buttons_label.setBounds(15, 700, 80, 50);
    m_delay_time.setBounds(200, 710, 90, 90);
    m_delay_time_label.setBounds(120, 735, 90, 30);
    m_delay_feedback.setBounds(400, 710, 90, 90);
    m_delay_feedback_label.setBounds(320, 735, 90, 30);
    this->audioProcessor.m_visualiser.setBounds(110, 60, 980, 400);
    for (int i = 0; i < 16; i++)
    {
        m_volume_dials[i].setBounds(dials_distance_from_edeg + (dials_horizontal_distance * i), 580, size_of_dial, size_of_dial);
        m_pan_dials[i].setBounds(dials_distance_from_edeg + (dials_horizontal_distance * i), 510, size_of_dial, size_of_dial);
        m_on_off_buttons[i].setBounds(16 + dials_distance_from_edeg + dials_horizontal_distance * i, 650, size_of_dial, size_of_dial);
        m_reverb_buttons[i].setBounds(16 + dials_distance_from_edeg + dials_horizontal_distance * i, 700, size_of_dial, size_of_dial);
    }

}



//==============================================================================
void NewProjectAudioProcessorEditor::paint(juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));

    g.setColour(juce::Colours::white);
    g.setFont(15.0f);
    g.setColour(Colours::maroon);
    Rectangle<float> area(5, 5, 1195, 795);
    g.drawRoundedRectangle(area, 20.0f, 3.0f);
    Rectangle<float> area_2(108, 58, 984, 404);
    g.setColour(Colours::black);
    g.drawRoundedRectangle(area_2, 15.0f, 10.0f);
    Image background_image = ImageCache::getFromMemory(BinaryData::background_2_png, BinaryData::background_2_pngSize);
    if (!background_image.isNull())
    {
        m_background.setImage(background_image, RectanglePlacement::stretchToFit);
    }
    else
    {
        jassert(!background_image.isNull());
    }
    g.drawImageAt(background_image, 0, 0);

}

void NewProjectAudioProcessorEditor::resized()
{

    printComponents();

}