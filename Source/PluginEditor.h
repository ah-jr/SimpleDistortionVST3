/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class SimpleDistortionVSTAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    SimpleDistortionVSTAudioProcessorEditor (SimpleDistortionVSTAudioProcessor&);
    ~SimpleDistortionVSTAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    std::unique_ptr<juce::Slider> knob[4];

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> attachment[4];

    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    SimpleDistortionVSTAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SimpleDistortionVSTAudioProcessorEditor)
};
