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
    /*
    Knobs indexes:
        1. Drive
        2. Range
        3. Blend
        4. Volume
    */

    for (int i = 0; i < 4; i++) {
        std::string name;

        switch (i)
        {
        case 0: {name = "Drive";  break;}
        case 1: {name = "Range";  break;}
        case 2: {name = "Blend";  break;}
        case 3: {name = "Volume"; break;}
        default: {break;}
        }

        knob[i] = std::make_unique<juce::Slider>(name);
        addAndMakeVisible(*knob[i]);
        knob[i]->setSliderStyle(juce::Slider::Rotary);
        knob[i]->setTextBoxStyle(juce::Slider::NoTextBox, false, 100, 100);

        attachment[i] = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(p.getState(), name, *knob[i]);

    }

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

    for (int i = 0; i < 4; i++)
    {
        x = (i + 1) * (getWidth() / 5) - (100 / 2);
        knob[i]->setBounds(x, y, width, height);
    }
}
 