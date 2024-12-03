# see-shoes-fw


Final script is 
`function_vibrate_distance.ino`:
- Obstacle detection only occurs if IMU is not angled downwards 30 degrees or more (this represents the toe-off segment of the human walking pattern where the foot faces the ground).
- Obstacle detection:
  - To mimic the distances caught by the cane, both sensors only vibrate their haptic motors when objects are found within 46.8 inches (range of a cane)
  - The front sensor vibrates at 100% if the object detected is within a step's distance (24 inches)
      - If the front obstacle detected is greater than a step distance but within cane distance, vibration is somewhere between 100-60% (100% = 24 in, 60% = 46.8 in)
  - The side sensor vibrates from 100% = 0 inches to 60% = 46.8 inches

Final Script for the RP2040 is `rp2040_shoe` & it should do the same as mentioned above:
- Differences from the first script:
  - analog motor pins are different (A0 and A3) and have to be set using #define
  - digital pins are different for the distance sensors
  - IMU is oriented differently (seems to be flipped compared to the 33 BLE Rev2), but the code should account for this already
  
