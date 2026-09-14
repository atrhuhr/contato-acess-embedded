# -*- coding: utf-8 -*-
'''
  @file demo.py
  @brief This demo showcases all the vibration effects of the TM6605.
  @copyright  Copyright (c) 2025 DFRobot Co.Ltd (http://www.dfrobot.com)
  @license The MIT License (MIT)
  @author [fary](feng.yang@dfrobot.com)
  @version V1.0
  @date 2025-04-22
  @url https://github.com/DFRobot/DFRobot_TM6605
'''
import sys
sys.path.append("../")
import time
from DFRobot_TM6605 import DFRobot_TM6605

tm = DFRobot_TM6605()
if tm.begin() != 0:
    print("Sensor initialization failed!")
    exit(1)

print("Sensor initialization succeeded!")

# Get all vibration effects.
effects = DFRobot_TM6605.Effect
tm.play()
# Traverse all effects
'''effects:
sharp_click instant_click light_tap double_click light_pulse
strong_alert medium_duration_alert sharp_click_2 medium_click
flash_strike double_high_click_short double_medium_click_short 
double_flash_strike_short double_instant_click_long double_medium_instant_click_long 
double_flash_strike_long alert toggle_click 
long_slow_fade_transition_1 long_slow_fade_transition_2 
medium_slow_fade_transition_1 medium_slow_fade_transition_2 
short_slow_fade_transition_1 short_slow_fade_transition_2 
long_fast_fade_transition_1 long_fast_fade_transition_2 
medium_fast_fade_transition_1 medium_fast_fade_transition_2 
short_fast_fade_transition_1 short_fast_fade_transition_2 
long_slow_boost_transition_1 long_slow_boost_transition_2 
medium_slow_boost_transition_1 medium_slow_boost_transition_2 
short_slow_boost_transition_1 short_slow_boost_transition_2 
long_fast_boost_transition_1 long_fast_boost_transition_2 
medium_fast_boost_transition_1 medium_fast_boost_transition_2 
short_fast_boost_transition_1 short_fast_boost_transition_2 
long_alert soft_noise sleep_command
'''
for effect in effects:
    if effect.name == DFRobot_TM6605.Effect.sleep_command.name:
        continue
    print("\nPlaying effect:{} (id:{})".format(effect.name,effect.value))
    # Set and trigger the effect.
    tm.select_effect(effect) 
    #tm.select_effect(effect.value) 
    time.sleep(0.5) 
    if effect.name == DFRobot_TM6605.Effect.long_alert.name:
        time.sleep( 8)
    time.sleep(2)
    
tm.stop()  
time.sleep(0.5)

print("\nAll vibration effects demonstration completed.")