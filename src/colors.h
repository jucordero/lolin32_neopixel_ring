RgbColor orange(uint8_t peak){
  RgbColor color(peak, peak/4, 0);
  return color;
}

RgbColor red(uint8_t peak){
  RgbColor color(peak, 0, 0);
  return color;
}

RgbColor green(uint8_t peak){
  RgbColor color(0, peak, 0);
  return color;
}

RgbColor black(0);

RgbColor white(uint8_t peak){
  RgbColor color(peak);
  return color;
}

RgbColor wwhite(uint8_t peak){
  RgbColor color(peak, peak * 180 / 255, peak * 120 / 255);
  return color;
}

RgbColor cyan(uint8_t peak){
  RgbColor color(0, peak, peak);
  return color;
}

const uint32_t fire_colors[] = {
  0x000000,
  0x100000,
  0x300000,
  0x600000,
  0x800000,
  0xA00000,
  0xC02000,
  0xC04000,
  0xC06000,
  0xC08000
};

const RgbColor water_colors[] = {  
  RgbColor::LinearBlend(RgbColor(0, 80, 80), RgbColor(0, 0, 50), 0.0),
  RgbColor::LinearBlend(RgbColor(0, 80, 80), RgbColor(0, 0, 50), 0.1),
  RgbColor::LinearBlend(RgbColor(0, 80, 80), RgbColor(0, 0, 50), 0.2),
  RgbColor::LinearBlend(RgbColor(0, 80, 80), RgbColor(0, 0, 50), 0.3),
  RgbColor::LinearBlend(RgbColor(0, 80, 80), RgbColor(0, 0, 50), 0.4),
  RgbColor::LinearBlend(RgbColor(0, 80, 80), RgbColor(0, 0, 50), 0.5),
  RgbColor::LinearBlend(RgbColor(0, 80, 80), RgbColor(0, 0, 50), 0.6),
  RgbColor::LinearBlend(RgbColor(0, 80, 80), RgbColor(0, 0, 50), 0.7),
  RgbColor::LinearBlend(RgbColor(0, 80, 80), RgbColor(0, 0, 50), 0.8),
  RgbColor::LinearBlend(RgbColor(0, 80, 80), RgbColor(0, 0, 50), 0.9),
  RgbColor::LinearBlend(RgbColor(0, 80, 80), RgbColor(0, 0, 50), 1.0)
};

const RgbColor matrix_colors[] = {  
  RgbColor::LinearBlend(RgbColor(0), RgbColor(0, 80, 0), 0.0),
  RgbColor::LinearBlend(RgbColor(0), RgbColor(0, 80, 0), 0.2),
  RgbColor::LinearBlend(RgbColor(0), RgbColor(0, 80, 0), 0.4),
  RgbColor::LinearBlend(RgbColor(0), RgbColor(0, 80, 0), 0.6),
  RgbColor::LinearBlend(RgbColor(0), RgbColor(0, 80, 0), 0.8),
  RgbColor::LinearBlend(RgbColor(0), RgbColor(0, 80, 0), 1.0),
  RgbColor(80)
};


