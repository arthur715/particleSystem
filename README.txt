For mac:
  gcc -DMACOSX -framework OpenGL -framework GLUT -Wno-deprecated-declarations ex1.c -o ex1
For school PC:
  cogl ex1.c

GUI Control:

  Motions:
    ,/.         : left / right
    -/+         : down / up
    up/down     : go front / back
    left/right  : turn left / right

  World functions:
    p : pause
    / : enable axis
    ? : enable ground
    space       : re-spawn the particles
    n/N         : decrease / increase number of particles in the system
    g/G         : gravity
    q/Q, w/W    : range of energy Loss
    m/M : mass (should have no change because gravity acts the same and the velocity is not depend on the mass of particles)

  Spawn Angle:
    1/!, 2/@    : range of initial speed
    3/#, 4/$    : range of angle from x axis  e.g (min,max) = (0,360) is will get a fountain, (179,181) will get just some water coming out from the emitter
    5/%, 6/^    : range of angle from y axis  i.e (20,25) will spawn more upward than (70,75)
    7/&, 8/*, 9/(, 0/) : similar to the above but control the initial angle of the fountain

  Colour of particles:
    a/A, s/S, d/D : red, green, blue
    z/Z, x/X, c/C : decrement / increment of red , green, blue, for each second

    Menu:
    right click : shows menu to change views, colours, and rendering type
