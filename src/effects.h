#include <numbers.h>
#include <colors.h>

uint16_t pixDown(uint16_t column, uint16_t level){
  uint16_t pix = column + (level-1)*20;
  return pix;
}

uint16_t pixUp(uint16_t column, uint16_t level){
  uint16_t pix = column + (level+1)*20;
  return pix;
}

uint16_t pixLeft(uint16_t pos){
  uint16_t pix = pos - 1;
  if (pos % 20 == 0) pix = pix + 20;
  return pix;
}

uint16_t pixRight(uint16_t pos){
  uint16_t pix = pos + 1;
  if (pos % 20 == 19) pix = pix - 20;
  return pix;
}

uint16_t pixDownMinus(uint16_t column, uint16_t level){
  uint16_t pix = column + (level-1)*20 - 1;
  uint16_t rest = column % 20;
  if (rest == 0) pix = pix + 20;

  return pix;
}

uint16_t pixDownPlus(uint16_t column, uint16_t level){
  uint16_t pix = column + (level-1)*20 + 1;
  uint16_t rest = column % 20;
  if (rest == 19) pix = pix - 20;

  return pix;
}


void rain(){
  for (uint16_t level = 0; level < levels; level++){

    for (uint16_t column = 0; column < 20; column++){

      uint16_t pos = column + level*20;
      RgbColor oldColor = strip.GetPixelColor(pos);
      RgbColor newColor;

      if (level == levels-1)
        newColor = random(100) < prob ? white(peak) : black;

      else{
        RgbColor cUp = strip.GetPixelColor(pixUp(column, level));
        newColor = cUp.R > peak/2 ? white(peak) : black;
      }

      AnimUpdateCallback animUpdate = [=](const AnimationParam& param){
        // use the curve value to apply to the animation
        RgbColor updatedColor = RgbColor::LinearBlend(oldColor, newColor, param.progress);
        strip.SetPixelColor(pos, updatedColor);
      };
      
      animations.StartAnimation(pos, animTime/2, animUpdate);
    }
  }
}

void drawNumber(char number, uint8_t numPos){
  switch(number){
    case '1': for (uint8_t i=0; i < sizeof(pix1); i++) strip.SetPixelColor(pix1[i]+4*numPos, red(peak)); break;
    case '2': for (uint8_t i=0; i < sizeof(pix2); i++) strip.SetPixelColor(pix2[i]+4*numPos, red(peak)); break;
    case '3': for (uint8_t i=0; i < sizeof(pix3); i++) strip.SetPixelColor(pix3[i]+4*numPos, red(peak)); break;
    case '4': for (uint8_t i=0; i < sizeof(pix4); i++) strip.SetPixelColor(pix4[i]+4*numPos, red(peak)); break;
    case '5': for (uint8_t i=0; i < sizeof(pix5); i++) strip.SetPixelColor(pix5[i]+4*numPos, red(peak)); break;
    case '6': for (uint8_t i=0; i < sizeof(pix6); i++) strip.SetPixelColor(pix6[i]+4*numPos, red(peak)); break;
    case '7': for (uint8_t i=0; i < sizeof(pix7); i++) strip.SetPixelColor(pix7[i]+4*numPos, red(peak)); break;
    case '8': for (uint8_t i=0; i < sizeof(pix8); i++) strip.SetPixelColor(pix8[i]+4*numPos, red(peak)); break;
    case '9': for (uint8_t i=0; i < sizeof(pix9); i++) strip.SetPixelColor(pix9[i]+4*numPos, red(peak)); break;
    case '0': for (uint8_t i=0; i < sizeof(pix0); i++) strip.SetPixelColor(pix0[i]+4*numPos, red(peak)); break;
  }
}

void timer(){

  if (loopCounter == 0){
    struct tm timeinfo;
    getLocalTime(&timeinfo);

    char timeHour[3];
    char timeMinute[3];
    char timeSecond[3];
    strftime(timeHour,3, "%H", &timeinfo);
    strftime(timeMinute,3, "%M", &timeinfo);
    strftime(timeSecond,3, "%S", &timeinfo);

    for (uint16_t i = 0; i < PixelCount; i++){
      strip.SetPixelColor(i, black);
    }

    drawNumber(timeHour[0], 3);
    drawNumber(timeHour[1], 2);
    strip.SetPixelColor(67, white(peak));
    strip.SetPixelColor(127, white(peak));
    drawNumber(timeMinute[0], 1);
    drawNumber(timeMinute[1], 0);
  }

  loopCounter++;
  if(loopCounter == 20) loopCounter = 0;
  
  for (uint16_t i = 0; i < PixelCount; i++){

    RgbColor oldColor = strip.GetPixelColor(i);
    RgbColor newColor = strip.GetPixelColor(pixLeft(i));

    AnimUpdateCallback animUpdate = [=](const AnimationParam& param){
      strip.SetPixelColor(i, newColor);
    };
        
    animations.StartAnimation(i, animTime, animUpdate);
  }
}

void lamp(){
  for (uint16_t i = 0; i < PixelCount; i++){
    if (10 < (i+angle) % 20)
      strip.SetPixelColor(i, wwhite(peak));
    else
      strip.SetPixelColor(i, black);

  }
  strip.Show();
}

void off(){
  for (uint16_t i = 0; i < PixelCount; i++){
    strip.SetPixelColor(i, black);
  }
  strip.Show();
}

void random_lights(){

    AnimEaseFunction easing = NeoEase::CubicCenter;

    for (uint16_t pos = 0; pos < PixelCount; pos++){

      RgbColor oldColor = strip.GetPixelColor(pos);
      RgbColor newColor;


      if (random(100) < prob){
        if (oldColor == black) newColor = white(peak);
        else newColor = black;
      }
      else newColor = oldColor;

      AnimUpdateCallback animUpdate = [=](const AnimationParam& param){

        float progress = easing(param.progress);

        // use the curve value to apply to the animation
        RgbColor updatedColor = RgbColor::LinearBlend(oldColor, newColor, progress);
        strip.SetPixelColor(pos, updatedColor);
      };
      
      animations.StartAnimation(pos, animTime/2, animUpdate);
  }
}

void rings(){

  if (loopCounter == 0){
    for (uint16_t pos = 0; pos < 20; pos++)
      strip.SetPixelColor(pos, cyan(peak));
    for (uint16_t pos = 20; pos < PixelCount; pos++)
      strip.SetPixelColor(pos, black);
  }

  loopCounter++;
  if (loopCounter == 19) loopCounter = 1;

  if (loopCounter < 10){
    for (uint16_t level = 0; level < levels; level++){
      for (uint16_t column = 0; column < 20; column++){
        uint16_t pos = column + level*20;
        RgbColor oldColor = strip.GetPixelColor(pos);
        RgbColor newColor;
        if(level == 0)
          newColor = black;
        else
          if (strip.GetPixelColor(pixDown(column, level)) == black)
            newColor = black;
          else
            newColor = cyan(peak);

        AnimUpdateCallback animUpdate = [=](const AnimationParam& param){
          RgbColor updatedColor = RgbColor::LinearBlend(oldColor, newColor, param.progress);
          strip.SetPixelColor(pos, updatedColor);
        };          
        animations.StartAnimation(pos, animTime, animUpdate);
      }
    }
  }

  else{
    for (uint16_t level = 0; level < levels; level++){
      for (uint16_t column = 0; column < 20; column++){
        uint16_t pos = column + level*20;
        RgbColor oldColor = strip.GetPixelColor(pos);
        RgbColor newColor;
        if(level == levels-1)
          newColor = black;
        else
          if (strip.GetPixelColor(pixUp(column, level)) == black)
            newColor = black;
          else
            newColor = cyan(peak);

        AnimUpdateCallback animUpdate = [=](const AnimationParam& param){
          RgbColor updatedColor = RgbColor::LinearBlend(oldColor, newColor, param.progress);
          strip.SetPixelColor(pos, updatedColor);
        };          
        animations.StartAnimation(pos, animTime, animUpdate);
      }
    }
  }
}

void fire(){

  for (int16_t level = levels-1; level >= 0; level--){
    for (uint16_t column = 0; column < 20; column++){        

      uint16_t pos = column + level*20;
      RgbColor oldColor = strip.GetPixelColor(pos);
      RgbColor newColor;
      uint16_t n;
      if (level == 0)
        grid[level][column] = random(6, 10);

      else{
        n = grid[level-1][column];
        if (n == 0)
          grid[level][column] = 0;
        else
          grid[level][column] = n-1;
      }
      newColor = HtmlColor(fire_colors[grid[level][column]]);
    
      AnimUpdateCallback animUpdate = [=](const AnimationParam& param){
        // use the curve value to apply to the animation
        RgbColor updatedColor = RgbColor::LinearBlend(oldColor, newColor, param.progress);
        strip.SetPixelColor(pos, updatedColor);
      };

      animations.StartAnimation(pos, animTime, animUpdate);
    }
  }
}

void water(){
  Serial.println(loopCounter);
  for (int16_t level = levels-1; level >= 0; level--){
    for (uint16_t column = 0; column < 20; column++){        

      uint16_t pos = column + level*20;
      RgbColor oldColor = strip.GetPixelColor(pos);
      RgbColor newColor;
      uint16_t n;
      if (level == 0)
        grid[level][column] = loopCounter;

      else{
        n = grid[level-1][column];
        if (n == 1)
          grid[level][column] = 1;
        else
          grid[level][column] = n;
      }
      newColor = HtmlColor(water_colors[grid[level][column]]);
    
      AnimUpdateCallback animUpdate = [=](const AnimationParam& param){
        // use the curve value to apply to the animation
        RgbColor updatedColor = RgbColor::LinearBlend(oldColor, newColor, param.progress);
        strip.SetPixelColor(pos, updatedColor);
      };

      animations.StartAnimation(pos, animTime, animUpdate);
      }
  }
  loopCounter = loopCounter + direction * random(1,4);
  if (loopCounter >= 10){
    loopCounter = 10;
    direction = -direction;
  }

  else if(loopCounter <= 1){
    loopCounter = 1;
    direction = -direction;
  }
}

void matrix(){
  for (uint16_t level = 0; level < levels; level++){
    for (uint16_t column = 0; column < 20; column++){

      uint16_t pos = column + level*20;
      RgbColor oldColor = strip.GetPixelColor(pos);
      RgbColor newColor;

      if (level == levels-1)
        grid[level][column] = random(100) < prob ? 6 : (0 < grid[level][column] ? grid[level][column] -1 : 0);
      else
        grid[level][column] = grid[level+1][column];

      newColor = HtmlColor(matrix_colors[grid[level][column]]);

      AnimUpdateCallback animUpdate = [=](const AnimationParam& param){
        // use the curve value to apply to the animation
        RgbColor updatedColor = RgbColor::LinearBlend(oldColor, newColor, param.progress);
        strip.SetPixelColor(pos, updatedColor);
      };
      
      animations.StartAnimation(pos, animTime, animUpdate);
    }
  }
}

// It would be great to be able to generate more things 
