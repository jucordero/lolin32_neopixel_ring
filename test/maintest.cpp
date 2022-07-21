// NeoPixelAnimation
// This example will randomly pick a new color for each pixel and animate
// the current color to the new color over a random small amount of time, using
// a randomly selected animation curve.
// It will repeat this process once all pixels have finished the animation
// 
// This will demonstrate the use of the NeoPixelAnimator extended time feature.
// This feature allows for different time scales to be used, allowing slow extended
// animations to be created.
// 
// This will demonstrate the use of the NeoEase animation ease methods; that provide
// simulated acceleration to the animations.
//
// It also includes platform specific code for Esp8266 that demonstrates easy
// animation state and function definition inline.  This is not available on AVR
// Arduinos; but the AVR compatible code is also included for comparison.
//
// The example includes some serial output that you can follow along with as it 
// does the animation.
//

#include <NeoPixelBus.h>
#include <NeoPixelAnimator.h>

const uint16_t PixelCount = 100; // make sure to set this to the number of pixels in your strip
const uint8_t PixelPin = 13;  // make sure to set this to the correct pin, ignored for Esp8266
const uint16_t levels = 5; // Number of stacked led rings

NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> strip(PixelCount, PixelPin);
// For Esp8266, the Pin is omitted and it uses GPIO3 due to DMA hardware use.  
// There are other Esp8266 alternative methods that provide more pin options, but also have
// other side effects.
// for details see wiki linked here https://github.com/Makuna/NeoPixelBus/wiki/ESP8266-NeoMethods 

// NeoPixel animation time management object
NeoPixelAnimator animations(levels, NEO_MILLISECONDS);

// create with enough animations to have one per pixel, depending on the animation
// effect, you may need more or less.
//
// since the normal animation time range is only about 65 seconds, by passing timescale value
// to the NeoPixelAnimator constructor we can increase the time range, but we also increase
// the time between the animation updates.   
// NEO_CENTISECONDS will update the animations every 100th of a second rather than the default
// of a 1000th of a second, but the time range will now extend from about 65 seconds to about
// 10.9 minutes.  But you must remember that the values passed to StartAnimations are now 
// in centiseconds.
//
// Possible values from 1 to 32768, and there some helpful constants defined as...
// NEO_MILLISECONDS        1    // ~65 seconds max duration, ms updates
// NEO_CENTISECONDS       10    // ~10.9 minutes max duration, centisecond updates
// NEO_DECISECONDS       100    // ~1.8 hours max duration, decisecond updates
// NEO_SECONDS          1000    // ~18.2 hours max duration, second updates
// NEO_DECASECONDS     10000    // ~7.5 days, 10 second updates
//



void setup()
{
    Serial.begin(9600);
    while (!Serial); // wait for serial attach

    strip.Begin();
    strip.Show();

    // just pick some colors
    for (uint16_t pixel = 0; pixel < PixelCount; pixel++)
    {
        RgbColor color = RgbColor(random(10), random(10), random(10));
        strip.SetPixelColor(pixel, color);
    }

    Serial.println();
    Serial.println("Running...");
}


void SetupAnimationSet(){

    // setup some animations
    for (uint16_t pixel = 0; pixel < PixelCount; pixel++){
        const uint8_t peak = 10;

        // pick a random duration of the animation for this pixel
        // since values are centiseconds, the range is 1 - 4 seconds
        uint16_t time = random(100, 400);

        // each animation starts with the color that was present
        RgbColor originalColor = strip.GetPixelColor(pixel);
        // and ends with a random color
        RgbColor targetColor = RgbColor(random(peak), random(peak), random(peak));
        // with the random ease function
        AnimEaseFunction easing;

        switch (random(3)){
            case 0:
                easing = NeoEase::CubicIn;
                break;
            case 1:
                easing = NeoEase::CubicOut;
                break;
            case 2:
                easing = NeoEase::QuadraticInOut;
                break;
        }

    // we must supply a function that will define the animation, in this example
    // we are using "lambda expression" to define the function inline, which gives
    // us an easy way to "capture" the originalColor and targetColor for the call back.
    //
    // this function will get called back when ever the animation needs to change
    // the state of the pixel, it will provide a animation progress value
    // from 0.0 (start of animation) to 1.0 (end of animation)
    //
    // we use this progress value to define how we want to animate in this case
    // we call RgbColor::LinearBlend which will return a color blended between
    // the values given, by the amount passed, hich is also a float value from 0.0-1.0.
    // then we set the color.
    //
    // There is no need for the MyAnimationState struct as the compiler takes care
    // of those details for us
    AnimUpdateCallback animUpdate = [=](const AnimationParam& param)
    {
        // progress will start at 0.0 and end at 1.0
        // we convert to the curve we want
        float progress = easing(param.progress);

        // use the curve value to apply to the animation
        RgbColor updatedColor = RgbColor::LinearBlend(originalColor, targetColor, progress);
        strip.SetPixelColor(pixel, updatedColor);
    };

    // now use the animation properties we just calculated and start the animation
    // which will continue to run and call the update function until it completes
    animations.StartAnimation(pixel, time, animUpdate);
    }
}

void loop()
{
    if (animations.IsAnimating())
    {
        // the normal loop just needs these two to run the active animations
        animations.UpdateAnimations();
        strip.Show();
    }
    else
    {
        Serial.println();
        Serial.println("Setup Next Set...");
        // example function that sets up some animations
        SetupAnimationSet();
    }
}