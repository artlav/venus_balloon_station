Shukra venus balloon station 080813 for Orbiter 2006
Made in 2008 by Artlav
Full add-on files (cfg, mesh, textures, etc) are at https://www.orbithangar.com/searchid.php?ID=3488
Requires UMMU SDK from http://orbiter.dansteph.com
Some assembly required, code provided as is.
Needs porting to newer Orbiter and Visual Studio

## About
Shukra Venus Balloon Station is a manned facility in the Earth region of the Venus atmosphere, maintained up there by aerostatic means.

## Installation
Unpack into Orbiter directory.
Shukra is UMMU compatible, installing UMMU addon is highly recommended. See http://orbiter.dansteph.com

Collision physics specs and known bugs
The landing happens if relative velocity is less than 0.5 m/s.
The station's location is fixed once the time accel goes above 10x, other time it is in free-float.
Landing on a station with time accel above or equal 10 is certain to create bugs.

## Keyboard controls

#### Landed vessel
Period key engages wheel arrest

#### UMMU
To move, use ROT or OFF mode
Motion as in ground mode, secondary - WASD keys
Airlock B is on
Action button is E

#### Platform
Alt+C  Stow/Retract landing strip
Alt+V  Stow/Retract probe
Alt+B  Cycle dropper
Numpad +  Raise station
Numpad -  Lower station

#### VC Camera navigation
Alt+K move forward
Alt+I move backwards
Alt+J turn left
Alt+L turn right
Alt+U Return to default location

## Changes
#### 080813
- Fixed some UMMU bugs
- Added runway lights
- Added ADWS as UMMU controls
- Added more animations
- Added UMMU-interactable components

#### 080811
- Multiple fixes
- Tuned collision handling (multiple vessels, hanging in weird attitudes, fall-thru without a cause)
- Fixed the collision edges (tri-shape with hole and runway7 instead of circle)
- Added full UMMU support (walking, jumping, entering)
- Added wheel arrest (. key in landing vessel)
