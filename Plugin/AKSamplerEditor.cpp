//
//  AKSamplerEditor.cpp
//  JUCE-AKSampler
//
//  Created by Shane Dunne, revision history on Github.
//  Copyright © 2018 AudioKit. All rights reserved.
//
#include "AKSamplerProcessor.h"
#include "AKSamplerEditor.h"
#include "JuceHeader.h"

AKSamplerEditor::AKSamplerEditor(AKSamplerProcessor& p)
    : AudioProcessorEditor(&p)
    , processor(p)
    , grpMaster("grpMaster", TRANS("Master"))
    , lblMasterVol("lblMasterVol", TRANS("Level"))
    , lblSemitoneOffset("lblSemitoneOffset", TRANS("Pitch"))
    , lblFineTune("lblFineTune", TRANS("Tune"))
    , grpAmpEnvelope("grpAmpEnvelope", TRANS("Amplitude Envelope"))
    , lblAmpAttack("lblAmpAttack", TRANS("Attack"))
    , lblAmpDecay("lblAmpDecay", TRANS("Decay"))
    , lblAmpSustain("lblAmpSustain", TRANS("Sustain"))
    , lblAmpRelease("lblAmpRelease", TRANS("Release"))
    , lblAmpVelSens("lblAmpVelSens", TRANS("Vel Sens"))
#if !JUCE_IOS
    , btnSfzFolderSelect(TRANS("Patch"))
#endif
    , grpFilter("grpFilter", TRANS("Per-Voice Filter"))
    , lblFltCutoff("lblFltCutoff", TRANS("Cutoff"))
    , lblFltResonance("lblFltResonance", TRANS("Res"))
    , lblFltEgStrength("lblFltEgStrength", TRANS("Env Amt"))
    , lblFltAttack("lblFltAttack", TRANS("Attack"))
    , lblFltDecay("lblFltDecay", TRANS("Decay"))
    , lblFltSustain("lblFltSustain", TRANS("Sustain"))
    , lblFltRelease("lblFltRelease", TRANS("Release"))
    , lblFltVelSens("lblFltVelSens", TRANS("Vel Sens"))
    , keyboardComponent(p.keyboardState, juce::MidiKeyboardComponent::horizontalKeyboard)//might remove
{
    setLookAndFeel(&myLookAndFeel);

    //addAndMakeVisible(grpMaster);
     addAndMakeVisible(grpAmpEnvelope);
    //addAndMakeVisible(grpFilter);

    auto initLabel = [this](Label& label)
    {
       // addAndMakeVisible(label);
        label.setJustificationType(Justification::horizontallyCentred);
        label.setEditable(false, false, false);
    };
    initLabel(lblMasterVol);
    initLabel(lblSemitoneOffset);
    initLabel(lblFineTune);

    initLabel(lblAmpAttack);
    initLabel(lblAmpDecay);
    initLabel(lblAmpSustain);
    initLabel(lblAmpRelease);
    initLabel(lblAmpVelSens);

    initLabel(lblFltCutoff);
    initLabel(lblFltResonance);
    initLabel(lblFltEgStrength);
    initLabel(lblFltAttack);
    initLabel(lblFltDecay);
    initLabel(lblFltSustain);
    initLabel(lblFltRelease);
    initLabel(lblFltVelSens);

    auto initSlider = [this](Slider& slider)
    {
        addAndMakeVisible(slider);
        slider.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
        slider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
        slider.setPopupDisplayEnabled(true, true, 0);
        slider.onValueChange = [this, &slider] { sliderValueChanged(&slider); };
    };
    auto initSecondsSlider = [this](Slider& slider)
    {
        addAndMakeVisible(slider);
        slider.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
        slider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
        slider.setPopupDisplayEnabled(true, true, 0);
        slider.setRange(0, 12, 0);
        slider.setSkewFactor(0.23);
        slider.onValueChange = [this, &slider] { sliderValueChanged(&slider); };
    };

    initSlider(slMasterVol);
    slMasterVol.setRange(0, 1);
    slMasterVol.setPointer(&processor.patchParams.sampler.main.masterLevel);
    slMasterVol.setDoubleClickReturnValue(true, 0.7);

    initSlider(slSemitoneOffset);
    slSemitoneOffset.setRange(-24, +24, 1);
    slSemitoneOffset.setPointer(&processor.patchParams.sampler.osc1.pitchOffsetSemitones);
    slSemitoneOffset.setDoubleClickReturnValue(true, 0);

    initSlider(slFineTune);
    slFineTune.setRange(-24, +24, -12);
    slFineTune.setPointer(&processor.patchParams.sampler.osc1.detuneOffsetCents);
    slFineTune.setDoubleClickReturnValue(true, -12);

    initSecondsSlider(slAmpAttack);
    slAmpAttack.setPointer(&processor.patchParams.sampler.ampEG.attackTimeSeconds);
    slAmpAttack.setDoubleClickReturnValue(true, 0);

    initSecondsSlider(slAmpDecay);
    slAmpDecay.setPointer(&processor.patchParams.sampler.ampEG.decayTimeSeconds);
    slAmpDecay.setDoubleClickReturnValue(true, 0);

    initSlider(slAmpSustain);
    slAmpSustain.setRange(0, 1);
    slAmpSustain.setPointer(&processor.patchParams.sampler.ampEG.sustainLevel);
    slAmpSustain.setDoubleClickReturnValue(true, 1);

    initSecondsSlider(slAmpRelease);
    slAmpRelease.setPointer(&processor.patchParams.sampler.ampEG.releaseTimeSeconds);
    slAmpRelease.setDoubleClickReturnValue(true, 0);

    initSlider(slAmpVelSens);
    slAmpVelSens.setRange(0, 1);
    slAmpVelSens.setSkewFactor(5.0);
    slAmpVelSens.setPointer(&processor.patchParams.sampler.main.ampVelocitySensitivity);
    slAmpVelSens.setDoubleClickReturnValue(true, 1);

#if JUCE_IOS
    addAndMakeVisible(cbBankSelect);
    populateBankComboBox();
    cbBankSelect.setEditableText(false);
    cbBankSelect.setTextWhenNothingSelected("");
    cbBankSelect.onChange = [this] { bankSelect(); };
#else
    addAndMakeVisible(btnSfzFolderSelect);
    btnSfzFolderSelect.onClick = [this] { sfzButtonClicked(); };
#endif

    addAndMakeVisible(cbSfzSelect);
    cbSfzSelect.setEditableText(false);
    cbSfzSelect.setJustificationType(Justification::centredLeft);
    cbSfzSelect.setTextWhenNothingSelected("Nothing Selected");
    cbSfzSelect.setTextWhenNoChoicesAvailable(TRANS("(select a Folder at Right)"));
    cbSfzSelect.onChange = [this] { sfzSelect(); };
    
    auto initFilterCombo = [this](FilterComboBox& combo)
    {
        addAndMakeVisible(combo);
        combo.setEditableText(false);
        combo.setJustificationType(Justification::centredLeft);
        combo.setTextWhenNothingSelected("");
        combo.setTextWhenNoChoicesAvailable(TRANS("(no choices)"));
        FilterSelector::setupComboBox(combo);
        combo.onChange = [this] { filterChanged(); };
    };
    initFilterCombo(cbFilterType);
    filterSelector.setIndex(processor.patchParams.sampler.filter.stages);
    cbFilterType.setPointer(&filterSelector);

    initSlider(slFltCutoff);
    slFltCutoff.setRange(0, 100);
    slFltCutoff.setPointer(&processor.patchParams.sampler.filter.cutoff);
    slFltCutoff.setDoubleClickReturnValue(true, 100);

    initSlider(slFltResonance);
    slFltResonance.setRange(0, +30);
    slFltResonance.setPointer(&processor.patchParams.sampler.filter.resonance);
    slFltResonance.setDoubleClickReturnValue(true, 0);

    initSlider(slFltEgStrength);
    slFltEgStrength.setRange(0, 100);
    slFltEgStrength.setPointer(&processor.patchParams.sampler.filter.envAmount);
    slFltEgStrength.setDoubleClickReturnValue(true, 0);

    initSecondsSlider(slFltAttack);
    slFltAttack.setPointer(&processor.patchParams.sampler.filterEG.attackTimeSeconds);
    slFltAttack.setDoubleClickReturnValue(true, 0);

    initSecondsSlider(slFltDecay);
    slFltDecay.setPointer(&processor.patchParams.sampler.filterEG.decayTimeSeconds);
    slFltDecay.setDoubleClickReturnValue(true, 0);

    initSlider(slFltSustain);
    slFltSustain.setRange(0, 1);
    slFltSustain.setPointer(&processor.patchParams.sampler.filterEG.sustainLevel);
    slFltSustain.setDoubleClickReturnValue(true, 1);

    initSecondsSlider(slFltRelease);
    slFltRelease.setPointer(&processor.patchParams.sampler.filterEG.releaseTimeSeconds);
    slFltRelease.setDoubleClickReturnValue(true, 0);

    initSlider(slFltVelSens);
    slFltVelSens.setRange(0, 1);
    slFltVelSens.setSkewFactor(4.0f);
    slFltVelSens.setPointer(&processor.patchParams.sampler.main.filterVelocitySensitivity);
    slFltVelSens.setDoubleClickReturnValue(true, 0);

    const int windowWidth = 900;
    const int windowHeight = 500;
    setSize(windowWidth, windowHeight);    // note this does nothing on mobile platforms
}

AKSamplerEditor::~AKSamplerEditor()
{
    setLookAndFeel(nullptr);

}

void AKSamplerEditor::paint (Graphics& g)
{
    //g.fillAll(getLookAndFeel().findColour(ResizableWindow::backgroundColourId));
    Image imagebg;
    imagebg = ImageCache::getFromMemory(BinaryData::finalbg_png, BinaryData::finalbg_pngSize);
    g.setColour(Colours::black);
    g.drawImageAt(imagebg,0,0);
    g.fillRect(0, 0, 900, 56);
    //g.fillAll(Colours::black);
    Image logo;
    logo = ImageCache::getFromMemory(BinaryData::finlogo121_png, BinaryData::finlogo121_pngSize);
    g.drawImageAt(logo, 15, 0);
    g.setColour(Colours::white);
    g.drawHorizontalLine(56,0,900);
}

void AKSamplerEditor::resized()
{
    const int gutter = 8;
    const int nudge = 2;
    const int topOffset = -120 + gutter;//originally 10
    const int masterGroupWidth = 200;
    const int ampGroupWidth = 340;
    const int rowHeight = 120;
    const int knobWidth = 60;
    const int cbWidth = 50;
    const int cbHeight = 18;
    const int sfzSelectHeight = 40;
#if !JUCE_IOS
    const int sfzButtonWidth = 80;
#endif
    const int sfzSelectSqueeze = 4;

    auto area = getLocalBounds();
    area.reduce(gutter, gutter);
    auto rowArea = area.removeFromBottom(rowHeight);

    
    keyboardComponent.setSize(900, 120);
    auto groupArea1 = rowArea.removeFromBottom(rowHeight);
    keyboardComponent.setBounds(groupArea1);
    addAndMakeVisible(keyboardComponent);
    
   



    getLookAndFeel().setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    getLookAndFeel().setColour(juce::ComboBox::backgroundColourId, juce::Colours::black);
    getLookAndFeel().setColour(juce::ComboBox::outlineColourId, juce::Colours::black);
    getLookAndFeel().setColour(juce::PopupMenu::backgroundColourId, juce::Colours::black);
    getLookAndFeel().setColour(juce::Slider::thumbColourId, juce::Colours::white);
    getLookAndFeel().setColour(juce::Slider::backgroundColourId, juce::Colours::whitesmoke);
    getLookAndFeel().setColour(juce::Slider::trackColourId, juce::Colours::white);

   

    auto groupArea = rowArea.removeFromLeft(masterGroupWidth);
    grpMaster.setBounds(groupArea);
    groupArea.removeFromTop(topOffset);
    groupArea.reduce(gutter, gutter);
    auto knobArea = groupArea.removeFromLeft(knobWidth);
   // slMasterVol.setBounds(900-gutter-40,gutter,45,45);//master vol setting the position
   // lblMasterVol.setBounds(knobArea);
    knobArea = groupArea.removeFromLeft(knobWidth);
    //slSemitoneOffset.setBounds(900 - gutter - 40 - 40 - gutter, gutter, 45, 45);
    //lblSemitoneOffset.setBounds(knobArea);
    knobArea = groupArea.removeFromLeft(knobWidth);
    //slFineTune.setBounds(900 - gutter - 40 - 40 - gutter - 40 - gutter, gutter, 45, 45);
    //lblFineTune.setBounds(900 - gutter - 40 - 40 - gutter - gutter, 0, 0, 45);//fine tune knob lbl

    mainVOL.setBounds(900 - gutter -100, 12, 100, 20);
    mainVOL.setTextBoxStyle(Slider::NoTextBox, false, 0, 0); 
    addAndMakeVisible(mainVOL);

    panning.setBounds(900 - gutter - 100, 30, 100, 20);
    panning.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    addAndMakeVisible(panning);

   // addAndMakeVisible(lblFineTune);
    groupArea = rowArea.removeFromRight(ampGroupWidth);
    grpAmpEnvelope.setBounds(groupArea);
    groupArea.removeFromTop(topOffset);
    groupArea.reduce(gutter, gutter);

    knobArea = groupArea.removeFromRight(knobWidth);
    slAmpVelSens.setBounds(10 + gutter + 54 * 4, 500 - rowHeight - gutter - 70, 52, 52);//hard
    lblAmpVelSens.setBounds(knobArea);
    groupArea.removeFromRight(2 * gutter);

    knobArea = groupArea.removeFromRight(knobWidth);
    slAmpRelease.setBounds(10 + gutter + 54 * 3, 500 - rowHeight - gutter - 70, 52, 52);//hard
    lblAmpRelease.setBounds(knobArea);
    knobArea = groupArea.removeFromRight(knobWidth);
    slAmpSustain.setBounds(10 + gutter + 54*2, 500 - rowHeight - gutter - 70, 52, 52);//hard
    lblAmpSustain.setBounds(knobArea);
    knobArea = groupArea.removeFromRight(knobWidth);
    slAmpDecay.setBounds(10+gutter + 54 *1, 500 - rowHeight - gutter - 70, 52, 52);//hard
    lblAmpDecay.setBounds(knobArea);
 
    slAmpAttack.setBounds(10+gutter, 500 - rowHeight - gutter-70,52,52);//hard
    //lblAmpAttack.setBounds(10, 500 - rowHeight - gutter-50 + 30,50,50);//hard

    groupArea = area.removeFromTop(sfzSelectHeight);
    groupArea.reduce(gutter,gutter);
#if JUCE_IOS
    cbBankSelect.setBounds(groupArea.removeFromLeft((groupArea.getWidth() - gutter) / 2));
    groupArea.removeFromLeft(gutter / 2);
#else
    btnSfzFolderSelect.setBounds(650,0,56,56);
    
#endif
    cbSfzSelect.setBounds(250,0,390,56);
    
    //////////////////////////////////////////////////////////////////////////////////////////////////////

    groupArea = area.removeFromBottom(rowHeight);
    grpFilter.setBounds(groupArea);
    auto cbArea = groupArea;
    cbArea.setY(cbArea.getY() - nudge);
    cbArea.removeFromLeft(0);
    cbArea = cbArea.removeFromLeft(cbWidth + 2 * gutter);
    cbFilterType.setBounds(10 + gutter + 54 * 6, 500 - rowHeight - gutter - 70 + 12, 100, 30);//hard cmbo box filter);
    groupArea.removeFromTop(topOffset);
    groupArea.reduce(gutter, gutter);

    knobArea = groupArea.removeFromLeft(knobWidth);
    slFltCutoff.setBounds(10 + gutter + 54 * 8, 500 - rowHeight - gutter - 70, 52, 52);//hard););
    lblFltCutoff.setBounds(knobArea);
    knobArea = groupArea.removeFromLeft(knobWidth);
    slFltResonance.setBounds(10 + gutter + 54 * 9, 500 - rowHeight - gutter - 70, 52, 52);//hard););
    lblFltResonance.setBounds(knobArea);
    knobArea = groupArea.removeFromLeft(knobWidth);
    slFltEgStrength.setBounds(10 + gutter + 54 * 10, 500 - rowHeight - gutter - 70, 52, 52);//hard););
    lblFltEgStrength.setBounds(knobArea);

    knobArea = groupArea.removeFromRight(knobWidth);
    slFltVelSens.setBounds(10 + gutter + 54 * 11, 500 - rowHeight - gutter - 70, 52, 52);//hard););
    lblFltVelSens.setBounds(knobArea);
    groupArea.removeFromRight(2 * gutter);

    knobArea = groupArea.removeFromRight(knobWidth);
    slFltRelease.setBounds(10 + gutter + 54 * 15, 500 - rowHeight - gutter - 70, 52, 52);//hard););
    lblFltRelease.setBounds(knobArea);
    knobArea = groupArea.removeFromRight(knobWidth);
    slFltSustain.setBounds(10 + gutter + 54 * 14, 500 - rowHeight - gutter - 70, 52, 52);//hard););
    lblFltSustain.setBounds(knobArea);
    knobArea = groupArea.removeFromRight(knobWidth);
    slFltDecay.setBounds(10 + gutter + 54 * 13, 500 - rowHeight - gutter - 70, 52, 52);//hard););
    lblFltDecay.setBounds(knobArea);
    knobArea = groupArea.removeFromRight(knobWidth);
    slFltAttack.setBounds(10 + gutter + 54 * 12, 500 - rowHeight - gutter - 70, 52, 52);//hard););
    lblFltAttack.setBounds(knobArea);

    keyboardComponent.grabKeyboardFocus();//attempt to grab focus look later
}

void AKSamplerEditor::changeListenerCallback(ChangeBroadcaster*)
{
    slMasterVol.notify();
    slSemitoneOffset.notify();
    slFineTune.notify();

    slAmpAttack.notify();
    slAmpDecay.notify();
    slAmpSustain.notify();
    slAmpRelease.notify();
    slAmpVelSens.notify();

    for (int i = 0; i < cbSfzSelect.getNumItems(); i++)
    {
        if (cbSfzSelect.getItemText(i) == processor.patchParams.sampler.osc1.sfzName)
        {
            cbSfzSelect.setSelectedItemIndex(i);
            break;
        }
    }

    cbFilterType.notify();
    slFltCutoff.notify();
    slFltResonance.notify();
    slFltEgStrength.notify();
    slFltAttack.notify();
    slFltDecay.notify();
    slFltSustain.notify();
    slFltRelease.notify();
    slFltVelSens.notify();
}

void AKSamplerEditor::sliderValueChanged(Slider*)
{
    processor.parameterChanged();
}

void AKSamplerEditor::filterChanged()
{
    int stages = filterSelector.getIndex();
    bool on = stages > 0;
    processor.patchParams.sampler.filter.stages = stages;

    slFltCutoff.setEnabled(on);
    slFltResonance.setEnabled(on);
    slFltEgStrength.setEnabled(on);
    slFltAttack.setEnabled(on);
    slFltDecay.setEnabled(on);
    slFltSustain.setEnabled(on);
    slFltRelease.setEnabled(on);
    slFltVelSens.setEnabled(on);

    processor.parameterChanged();
}

void AKSamplerEditor::sfzSelect()
{
    processor.loadSfz(dirUrl.getLocalFile().getFullPathName(), cbSfzSelect.getText());
}

#if JUCE_IOS
void AKSamplerEditor::populateBankComboBox()
{
    auto docsDir = getSharedResourceFolder().getChildFile("Sounds");
    cbBankSelect.clear();

    StringArray items;
    Array<File> subdirs;
    docsDir.findChildFiles(subdirs, File::findDirectories, false);
    for (int i = 0; i < subdirs.size(); i++)
    {
        File dir = subdirs.getReference(i);
        items.add(dir.getFileName());
    }

    items.sort(true);

    int count = 0;
    for (auto item : items) cbBankSelect.addItem(item, ++count);
}

void AKSamplerEditor::bankSelect()
{
    auto docsDir = getSharedResourceFolder().getChildFile("Sounds");
    dirUrl = URL(docsDir.getChildFile(cbBankSelect.getText()));
    populateSfzComboBox(dirUrl);
}
#else
void AKSamplerEditor::sfzButtonClicked()
{
    fc.reset(new FileChooser("Choose a directory of the root patch location", File::getCurrentWorkingDirectory(), "*.mspreset", true));//was just star
    fc->launchAsync(FileBrowserComponent::openMode | FileBrowserComponent::canSelectDirectories,
        [this](const FileChooser& chooser)
    {
        auto result = chooser.getURLResult();
        if (result.isLocalFile())
        {
            this->populateSfzComboBox(result);
        }
    });
}
#endif

void AKSamplerEditor::populateSfzComboBox(URL dir)
{
    dirUrl = dir;
    cbSfzSelect.clear();

    StringArray items;

    DirectoryIterator iter(dir.getLocalFile(), true);
    while (iter.next())
    {
        if (iter.getFile().hasFileExtension("mspreset"))
            items.add(iter.getFile().getFileName());
    }

    items.sort(true);

    int count = 0;
    for (auto item : items)
        cbSfzSelect.addItem(item, ++count);
}
