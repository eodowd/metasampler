//
//  GuiComponentUtils.cpp
//  JUCE-AKSampler
//
//  Created by Shane Dunne, revision history on Github.
//  Copyright © 2018 AudioKit. All rights reserved.
//
#include "GuiComponentUtils.h"



void MyLookAndFeel::drawRotarySlider(Graphics& g, int x, int y, int width, int height, float sliderPos,
    const float rotaryStartAngle, const float rotaryEndAngle,
    Slider& slider)
{
    ignoreUnused(slider);

    const float radius = jmin(width / 2, height / 2) - 10.0f;
    const float centreX = x + width * 0.5f;
    const float centreY = y + height * 0.5f;
    const float rx = centreX - radius;
    const float ry = centreY - radius;
    const float rw = radius * 2.0f;
    const float angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);

    // fill
    Image imageKnob;
    imageKnob = ImageCache::getFromMemory(BinaryData::mainknob_png, BinaryData::mainknob_pngSize);
    const double rotation = (slider.getValue()
        - slider.getMinimum())
        / (slider.getMaximum()
            - slider.getMinimum());
    const int frames = imageKnob.getHeight() / imageKnob.getWidth();
    const int frameId = (int)ceil(rotation * ((double)frames - 1.0));
    g.drawImage(imageKnob, (int)rx, (int)ry, 2 * (int)radius, 2 * (int)radius, 0, frameId * imageKnob.getWidth(), imageKnob.getWidth(), imageKnob.getWidth());

}

// See https://forum.juce.com/t/app-group-folder-access/23102
File getSharedResourceFolder()
{
    File bundle = File::getSpecialLocation(File::invokedExecutableFile).getParentDirectory();

    // macOS uses Contents/MacOS structure, iOS bundle structure is flat
#if JUCE_MAC
    bundle = bundle.getParentDirectory().getParentDirectory();
#endif

    // appex is in a PlugIns folder inside the parent bundle
    if (SystemStats::isRunningInAppExtensionSandbox())
        bundle = bundle.getParentDirectory().getParentDirectory();

#if JUCE_MAC
    bundle = bundle.getChildFile("Resources");
#endif

    return bundle;
}
