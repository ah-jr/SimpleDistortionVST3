/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SimpleDistortionVSTAudioProcessor::SimpleDistortionVSTAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    state = std::make_unique<juce::AudioProcessorValueTreeState>(*this, nullptr, "Parameters", createParameterLayout());
    //state = std::make_unique<juce::AudioProcessorValueTreeState>(*this, nullptr);

    //state->createAndAddParameter("drive", "Drive", "Drive", 
    //                             juce::NormalisableRange<float>(0.f, 1.f, 0.001), 
    //                             1.f, nullptr, nullptr);

    //state->createAndAddParameter("range", "Range", "Range", 
    //                             juce::NormalisableRange<float>(0.f, 3000.f, 0.001), 
    //                             1.f, nullptr, nullptr);

    //state->createAndAddParameter("blend", "Blend", "Blend",
    //                             juce::NormalisableRange<float>(0.f, 1.f, 0.001),
    //                             1.f, nullptr, nullptr);

    //state->createAndAddParameter("volume", "Volume", "Volume",
    //                             juce::NormalisableRange<float>(0.f, 3.f, 0.001),
    //                             1.f, nullptr, nullptr);

    //state->state = juce::ValueTree("drive");
    //state->state = juce::ValueTree("range");
    //state->state = juce::ValueTree("blend");
    //state->state = juce::ValueTree("volume");

}

juce::AudioProcessorValueTreeState::ParameterLayout SimpleDistortionVSTAudioProcessor::createParameterLayout() 
{
    std::vector <std::unique_ptr<juce::RangedAudioParameter>> params;

    auto drive = std::make_unique <juce::AudioParameterFloat>("drive", "Drive", 0.f, 1.0f, 1.0f);
    auto range = std::make_unique <juce::AudioParameterFloat>("range", "Range", 0.f, 3000.0f, 1.0f);
    auto blend = std::make_unique <juce::AudioParameterFloat>("blend", "Blend", 0.f, 1.0f, 1.0f);
    auto volume = std::make_unique <juce::AudioParameterFloat>("volume", "Volume", 0.f, 3.0f, 1.0f);

    params.push_back(std::move(drive));
    params.push_back(std::move(range));
    params.push_back(std::move(blend));
    params.push_back(std::move(volume));

    return { params.begin(), params.end() };
}

SimpleDistortionVSTAudioProcessor::~SimpleDistortionVSTAudioProcessor()
{
}

//==============================================================================
const juce::String SimpleDistortionVSTAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool SimpleDistortionVSTAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool SimpleDistortionVSTAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool SimpleDistortionVSTAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double SimpleDistortionVSTAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int SimpleDistortionVSTAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int SimpleDistortionVSTAudioProcessor::getCurrentProgram()
{
    return 0;
}

void SimpleDistortionVSTAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String SimpleDistortionVSTAudioProcessor::getProgramName (int index)
{
    return {};
}

void SimpleDistortionVSTAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void SimpleDistortionVSTAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void SimpleDistortionVSTAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SimpleDistortionVSTAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void SimpleDistortionVSTAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    float drive  = *state->getRawParameterValue("drive");
    float range  = *state->getRawParameterValue("range");
    float blend  = *state->getRawParameterValue("blend");
    float volume = *state->getRawParameterValue("volume");


    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);
        float sampleProc, sampleClean, sampleOutput;

        for (int sample = 0; sample < buffer.getNumSamples(); sample++)
        {
            sampleProc  = channelData[sample];
            sampleClean = channelData[sample];

            sampleProc = sampleProc * drive * range;
            sampleProc = (2.f / M_PI) * atan(sampleProc);
            sampleOutput = blend * sampleProc + (1 - blend) * sampleClean;

            channelData[sample] = volume * sampleOutput;
        }
    }
}

//==============================================================================
juce::AudioProcessorValueTreeState& SimpleDistortionVSTAudioProcessor::getState()
{
    return *state;
}


//==============================================================================
bool SimpleDistortionVSTAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* SimpleDistortionVSTAudioProcessor::createEditor()
{
    return new SimpleDistortionVSTAudioProcessorEditor (*this);
}

//==============================================================================
void SimpleDistortionVSTAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.

    juce::MemoryOutputStream stream(destData, false);
    state->state.writeToStream(stream);
}

void SimpleDistortionVSTAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.

    juce::ValueTree tree = juce::ValueTree::readFromData(data, sizeInBytes);

    if (tree.isValid()) {
        state->state = tree;
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SimpleDistortionVSTAudioProcessor();
}
