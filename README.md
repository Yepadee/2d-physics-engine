//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
How to use:
  Left click press: Spawn in a new rigid body if cursor not touching rigid body.
  Left click hold: Move a rigid body (External walls can be dragged) (Rigid bodies can be thrown when the simulation is running).
  Right click: Deletes the rigid body under the cursor.
  T: Show / hide polygons axis aligned bounding boxes and quad tree nodes.
  G: Toggle gravity.
  Enter: Pause / Resume the simulation.
  ESC: Quit.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Overview:
  The following program is a basic linear 2D collision engine between convex polygons.
  Much focus was put into making the engine as fast as possible. For example, rigid bodies
  are put into a quad tree so the number of collision tests can be reduced, and before we
  perform the expensive algorithm to see if two polygons are touching, we first perform
  a less expensive test to see if their axis aligned bounding boxes are touching.

  The engine makes use of a fixed time step loop to ensure the time between updates is
  constant by looking at the systems time.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Some structures worth talking about:
  Rigid Bodies:
    The rigid body structure stores all the information related to rigid bodies such as its
    mass (inverse mass as this is needed in equations often), velocity, coefficient of restitution and shape
    (stored as a polygon). A Boolean called immovable determines whether or not a polygon can be moved by other polygons.
    If not the rigid body is given an infinite mass (represented by setting the rigid bodies inverse mass to 0).
    The walls in the simulation are an example of immovable rigid bodies.
    You'll find some unused variables such as moment of inertia (mOfInertia) as I ran out of time trying to include
    rotational dynamics.

  Polygon:
    The polygon structure contains information that describes a polygon. The polygon.c file contains functions
    used to draw polygons to the screen. It also contains useful functions such as testing if a point
    lies within a polygon, finding the minimum translation vector between two intercepting polygons and finding
    a polygons axis aligned bounding box.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
