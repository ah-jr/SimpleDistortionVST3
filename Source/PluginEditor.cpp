/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SimpleDistortionVSTAudioProcessorEditor::SimpleDistortionVSTAudioProcessorEditor (SimpleDistortionVSTAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
 
    driveKnob = std::make_unique<juce::Slider>("Drive");
    addAndMakeVisible(*driveKnob);
    driveKnob->setSliderStyle(juce::Slider::Rotary);
    driveKnob->setTextBoxStyle(juce::Slider::NoTextBox, false, 100, 100);

    rangeKnob = std::make_unique<juce::Slider>("Drive");
    addAndMakeVisible(*rangeKnob);
    rangeKnob->setSliderStyle(juce::Slider::Rotary);
    rangeKnob->setTextBoxStyle(juce::Slider::NoTextBox, false, 100, 100);

    blendKnob = std::make_unique<juce::Slider>("Drive");
    addAndMakeVisible(*blendKnob);
    blendKnob->setSliderStyle(juce::Slider::Rotary);
    blendKnob->setTextBoxStyle(juce::Slider::NoTextBox, false, 100, 100);

    volumeKnob = std::make_unique<juce::Slider>("Drive");
    addAndMakeVisible(*volumeKnob);
    volumeKnob->setSliderStyle(juce::Slider::Rotary);
    volumeKnob->setTextBoxStyle(juce::Slider::NoTextBox, false, 100, 100);

    driveAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(p.getState(), 
        "drive", *driveKnob);

    rangeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(p.getState(),
        "range", *rangeKnob);

    blendAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(p.getState(),
        "blend", *blendKnob);

    volumeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(p.getState(),
        "volume", *volumeKnob);


    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (500, 200);
}

SimpleDistortionVSTAudioProcessorEditor::~SimpleDistortionVSTAudioProcessorEditor()
{
}

//==============================================================================
void SimpleDistortionVSTAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    //g.setColour (juce::Colours::white);
    //g.setFont (15.0f);
    //g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
}

void SimpleDistortionVSTAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..

    int x;
    int y = (getHeight() / 2) - (100 / 2);
    int width = 100;
    int height = 100;

    x = 1 * (getWidth() / 5) - (100 / 2);
    driveKnob->setBounds(x, y, width, height);

    x = 2 * (getWidth() / 5) - (100 / 2);
    rangeKnob->setBounds(x, y, width, height);

    x = 3 * (getWidth() / 5) - (100 / 2);
    blendKnob->setBounds(x, y, width, height);

    x = 4 * (getWidth() / 5) - (100 / 2);
    volumeKnob->setBounds(x, y, width, height);
}
 