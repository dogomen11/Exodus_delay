/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

#define TRANSPARENT 0.5f
#define BOLD 1.0f

//==============================================================================
ExodusAudioProcessorEditor::ExodusAudioProcessorEditor (ExodusAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p), ti_instence()
{
    m_input_gain_attach = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.tree_state, "m_input_gain_id", m_input_gain);
    m_output_gain_attach = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.tree_state, "m_output_gain_id", m_output_gain);
    m_delay_time_attach = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.tree_state, "m_delay_time_id", m_delay_time);
    m_delay_feedback_attach = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.tree_state, "m_delay_feedback_id", m_delay_feedback);
    m_delay_mix_attach = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.tree_state, "m_delay_mix_id", m_delay_mix);
    m_filter_freq_attach = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.tree_state, "m_filter_freq_id", m_filter_freq);
    m_filter_res_attach = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.tree_state, "m_filter_res_id", m_filter_res);
    m_filter_drive_attach = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.tree_state, "m_filter_drive_id", m_filter_drive);
    for (int i = 0; i < NUM_OF_INSTENCES; i++)
    {
        std::string vol_dial_id = "m_vol_dial_id_";
        vol_dial_id.append(to_string(i));
        m_volume_dials_attach[i] = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.tree_state, vol_dial_id, m_volume_dials[i]);
        std::string pan_dial_id = "m_pan_dial_id_";
        pan_dial_id.append(to_string(i));
        m_pan_dials_attach[i] = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.tree_state, pan_dial_id, m_pan_dials[i]);
        std::string on_off_button_id = "m_on_off_button_id_";
        on_off_button_id.append(to_string(i));
        m_on_off_buttons_attach[i] = std::make_unique<AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.tree_state, on_off_button_id, m_on_off_buttons[i]);
        std::string reverb_button_id = "m_reverb_button_id_";
        reverb_button_id.append(to_string(i));
        m_reverb_buttons_attach[i] = std::make_unique<AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.tree_state, reverb_button_id, m_reverb_buttons[i]);
    }
    startTimerHz(*audioProcessor.tree_state.getRawParameterValue("m_delay_time_id"));
    winSizeRatio = 3.0 / 1.0;
    setResizable(true, true);
    //setResizeLimits(600, juce::roundToInt(600.0 / winSizeRatio), 1200, juce::roundToInt(1200.0 / winSizeRatio));
    //getConstrainer()->setFixedAspectRatio(winSizeRatio);
    //setResizeLimits(600, 400, 1200, 800);
    setSize (1200, 800);
    initiateComponents(p);
}


ExodusAudioProcessorEditor::~ExodusAudioProcessorEditor()
{
    stopTimer();
}

void ExodusAudioProcessorEditor::timerCallback()
{
    audioProcessor.promoteInstence();
    indicator_debug.setBounds((84 + 64 * audioProcessor.current_instence), 650, 20, 20);
    startTimer(*audioProcessor.tree_state.getRawParameterValue("m_delay_time_id"));
    // TODO: add mroe options
}

void ExodusAudioProcessorEditor::buttonClicked(Button* button)
{
    for (int i = 0; i < 16; i++)
    {
        if (button == &m_on_off_buttons[i])
        {
            audioProcessor.delay.d_on_off[i] = m_on_off_buttons[i].getToggleState();
            switch (m_on_off_buttons[i].getToggleState())
            {
            case true:
                audioProcessor.delay.addMark(i);
                break;
            case false:
                audioProcessor.delay.subMark(i);
                break;
            }
        }
        else if (button == &m_reverb_buttons[i])
        {
            audioProcessor.delay.d_reverb[i] = m_reverb_buttons[i].getToggleState();
        }
    }
    reAlphaComponents();

}

void ExodusAudioProcessorEditor::reAlphaComponents()
{
    for (int i = 0; i < 16; i++)
    {
        if (audioProcessor.delay.getMarked() == 0)
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
void ExodusAudioProcessorEditor::paint (juce::Graphics& g)
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


void ExodusAudioProcessorEditor::initiateComponents(AudioProcessor& p)
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
    addAndMakeVisible(this->audioProcessor.m_visualiser_2);

    addAndMakeVisible(m_background);
    //*****************************************************************************
    addAndMakeVisible(m_input_gain);
    m_input_gain.setRange(-60.0f, 6.0f, 0.01f);
    m_input_gain.setSkewFactorFromMidPoint(0);
    m_input_gain.setValue(0.0f);
    m_input_gain.setTextValueSuffix("db");
    m_input_gain.setTextBoxStyle(Slider::TextBoxBelow, true, 80, 20);
    m_input_gain.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    //m_input_gain.addListener(this);
    addAndMakeVisible(m_input_gain_label);
    m_input_gain_label.setText("input", juce::dontSendNotification);

    addAndMakeVisible(m_output_gain);
    m_output_gain.setRange(-60.0f, 6.0f, 1.0f);
    m_output_gain.setSkewFactorFromMidPoint(0);
    m_output_gain.setValue(0.0f);
    m_output_gain.setTextValueSuffix("db");
    m_output_gain.setTextBoxStyle(Slider::TextBoxBelow, true, 80, 20);
    m_output_gain.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    //m_output_gain.addListener(this);
    addAndMakeVisible(m_output_gain_label);
    m_output_gain_label.setText("output", juce::dontSendNotification);

    addAndMakeVisible(m_delay_time);
    m_delay_time.setRange(0.0f, 1500.0f, 1.0f);
    m_delay_time.setValue(500.0f);
    m_delay_time.setTextValueSuffix("ms");
    m_delay_time.setTextBoxStyle(Slider::TextBoxBelow, true, 50, 20);
    m_delay_time.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    //m_delay_time.addListener(this);
    addAndMakeVisible(m_delay_time_label);
    m_delay_time_label.setText("time", juce::dontSendNotification);

    addAndMakeVisible(m_delay_feedback);
    m_delay_feedback.setRange(0.0f, 0.84f, 0.01f);
    m_delay_feedback.setValue(0.42f);
    m_delay_feedback.setTextValueSuffix("%");
    m_delay_feedback.setTextBoxStyle(Slider::TextBoxBelow, true, 50, 20);
    m_delay_feedback.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    //m_delay_feedback.addListener(this);
    addAndMakeVisible(m_delay_feedback_label);
    m_delay_feedback_label.setText("feedback", juce::dontSendNotification);

    addAndMakeVisible(m_delay_mix);
    m_delay_mix.setRange(0.0f, 100.0f,1.0f);
    m_delay_mix.setValue(40.0f);
    m_delay_mix.setTextValueSuffix("%");
    m_delay_mix.setTextBoxStyle(Slider::TextBoxBelow, true, 50, 20);
    m_delay_mix.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    //m_delay_mix.addListener(this);
    addAndMakeVisible(m_delay_mix_label);
    m_delay_mix_label.setText("mix", juce::dontSendNotification);

    addAndMakeVisible(m_filter_freq);
    m_filter_freq.setRange(20.0f, 12000.0f, 10.0f);
    m_filter_freq.setValue(2000.0f);
    m_filter_freq.setTextValueSuffix("Hz");
    m_filter_freq.setTextBoxStyle(Slider::TextBoxBelow, true, 50, 20);
    m_filter_freq.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    //m_filter_freq.addListener(this);
    addAndMakeVisible(m_filter_freq_label);
    m_filter_freq_label.setText("freq", juce::dontSendNotification);

    addAndMakeVisible(m_filter_res);
    m_filter_res.setRange(0.0f, 5.0f, 0.01f);
    m_filter_res.setValue(1.0f);
    m_filter_res.setTextBoxStyle(Slider::TextBoxBelow, true, 50, 20);
    m_filter_res.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    //m_filter_res.addListener(this);
    addAndMakeVisible(m_filter_res_label);
    m_filter_res_label.setText("res", juce::dontSendNotification);

    addAndMakeVisible(m_filter_drive);
    m_filter_drive.setRange(0.1f, 10.0f, 0.01f);
    m_filter_drive.setValue(1.0f);
    m_filter_drive.setTextValueSuffix("%");
    m_filter_drive.setTextBoxStyle(Slider::TextBoxBelow, true, 50, 20);
    m_filter_drive.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    //m_filter_drive.addListener(this);
    addAndMakeVisible(m_filter_drive_label);
    m_filter_drive_label.setText("drive", juce::dontSendNotification);
    //*****************************************************************************

    for (int i = 0; i < 16; i++)
    {
        addAndMakeVisible(m_volume_dials[i]);
        m_volume_dials[i].setRange(0.0f, 1.5f, 0.01f);
        m_volume_dials[i].setValue(1.0f);
        m_volume_dials[i].setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
        m_volume_dials[i].setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
        //m_volume_dials[i].addListener(this);
        m_volume_dials[i].setOpaque(false);
        if (audioProcessor.delay.getMarked() == 0) { m_volume_dials[i].setAlpha(TRANSPARENT); }

        addAndMakeVisible(m_pan_dials[i]);
        m_pan_dials[i].setRange(-1.0f, 1.0f, 0.01f);
        m_pan_dials[i].setValue(0.0f);
        m_pan_dials[i].setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
        m_pan_dials[i].setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
        //m_pan_dials[i].addListener(this);
        m_pan_dials[i].setOpaque(false);
        if (audioProcessor.delay.getMarked() == 0) { m_pan_dials[i].setAlpha(TRANSPARENT); }

        addAndMakeVisible(m_on_off_buttons[i]);
        m_on_off_buttons[i].addListener(this);
        if (audioProcessor.delay.getMarked() == 0) { m_on_off_buttons[i].setAlpha(TRANSPARENT); }

        addAndMakeVisible(m_reverb_buttons[i]);
        m_reverb_buttons[i].addListener(this);
        if (audioProcessor.delay.getMarked() == 0) { m_reverb_buttons[i].setAlpha(TRANSPARENT); }
    }
    addAndMakeVisible(ti_instence);
    addAndMakeVisible(indicator_debug);
    

}


void ExodusAudioProcessorEditor::printComponents()
{
    int dials_horizontal_distance = 66;
    int dials_distance_from_edeg = 68;
    int size_of_dial = 62;


    m_input_gain.setBounds(13, 5, 80, 80);
    m_input_gain_label.setBounds(30, 70, 80, 50);
    m_output_gain.setBounds(1110, 5, 80, 80);
    m_output_gain_label.setBounds(1127, 70, 70, 50);
    m_pan_dials_label.setBounds(15, 430, 80, 50);
    m_volume_dials_label.setBounds(15, 500, 80, 50);
    m_on_off_buttons_label.setBounds(15, 570, 80, 50);
    m_reverb_buttons_label.setBounds(15, 620, 80, 50);

    m_delay_time.setBounds(90, 710, 90, 90);
    m_delay_time_label.setBounds(60, 835, 90, 30);
    m_delay_feedback.setBounds(170, 710, 90, 90);
    m_delay_feedback_label.setBounds(140, 835, 90, 30);
    m_delay_mix.setBounds(310, 710, 90, 90);
    m_delay_mix_label.setBounds(340, 835, 90, 30);

    m_filter_freq.setBounds(600, 710, 70, 70);
    m_filter_freq_label.setBounds(600, 770, 90, 30);
    m_filter_res.setBounds(680, 710, 70, 70);
    m_filter_res_label.setBounds(680, 770, 90, 30);
    m_filter_drive.setBounds(760, 710, 70, 70);
    m_filter_drive_label.setBounds(760, 770, 90, 30);
    audioProcessor.m_visualiser.setBounds(25, 100, 600, 280);
    audioProcessor.m_visualiser_2.setBounds(635, 100, 600, 280);
    for (int i = 0; i < 16; i++)
    {
        m_pan_dials[i].setBounds(dials_distance_from_edeg + (dials_horizontal_distance * i), 430, size_of_dial, size_of_dial);
        m_volume_dials[i].setBounds(dials_distance_from_edeg + (dials_horizontal_distance * i), 500, size_of_dial, size_of_dial);
        m_on_off_buttons[i].setBounds(16 + dials_distance_from_edeg + dials_horizontal_distance * i, 570, size_of_dial, size_of_dial);
        m_reverb_buttons[i].setBounds(16 + dials_distance_from_edeg + dials_horizontal_distance * i, 620, size_of_dial, size_of_dial);
    }
    ti_instence.setBounds((84 + 64 * audioProcessor.current_instence), 650, 20, 20);
    indicator_debug.setBounds((84 + 64 * audioProcessor.current_instence), 650, 20, 20);
}

void ExodusAudioProcessorEditor::resized()
{
    auto bounds = getLocalBounds().toFloat() .removeFromBottom(getHeight() * 0.9f) .reduced(getWidth() * 0.06f, getHeight() * 0.25f);
    printComponents();
}
