# see-shoes-fw


Final script is 
`function_vibrate_distance.ino`:
- Obstacle detection only occurs if IMU is not angled downwards 30 degrees or more (this represents the toe-off segment of the human walking pattern where the foot faces the ground).
- Obstacle detection:
  - To mimic the distances caught by the cane, both sensors only vibrate their haptic motors when objects are found within 48.6 inches (range of a cane)
  - The front sensor vibrates at 100% if the object detected is within a step's distance (24 inches)
      - If the front obstacle detected is greater than a step distance but within cane distance, vibration is somewhere between 100-60% (100% = 24 in, 60% = 48.6 in)
  - The side sensor vibrates from 100% = 0 inches to 60% = 48.6 inches
