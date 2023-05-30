# township-roads

A solver for the township-roads geometry problem.

## The Problem Being Solved

There are a great many small oil and gas wells in Alberta, many of them in relatively isolated rural locations. Unfortunately, because they predate the existence of GPS, most of them do not have GPS coordinates on record. Instead, their location is given in terms of the legal land designation, and their perpendicular distance from the adjacent township road (E-W) and range road (N-S). However, because these roads typically don't run exactly parallel to geographical meridians and parallels, determining the precise latitude and longitude for a GPS location isn't straighforward.

The location and direction of the township or range road can be determined by taking a GPS fix at two points along it. Given these four points, and the perpendicular distance from each of the roads, what is the location of the well?

## The Solution

![Well location](regression.svg)

The well is located at the intersection of the two perpendiculars (thin black lines). However, the location of the base of each perpendicular isn't known because it depends on the location of the other perpendicular. Instead, we calculate the line that runs parallel to the road at an offset equal to the perpendicular distance (green lines). The well is then at the intersection of these lines. The parallel line can be defined by moving each of the defining points of the road by the perpendicular displacement. The latter is determined by rotating the direction vector of the road by a right angle, normalizing it (dividing its coordinates by its length) and multipying by the perpendicular distance.

Because the direction of the range road could be directly N-S, using a slope-intercept representation to calculate the intersection could run into an infinite slope situation. It's therefore much cleaner to use a vector method that's independent of slope, and in fact the entire solution works out much more nicely when using vectors throughout.

## The Program

The `township-roads` program takes input parameters on the comand line and outputs an SVG illustration of the location with its coordinates labelled. For example,

```
Usage: ./township-roads h dh v dv
   eg: ./township-roads 1.1,0.2:6.3,-0.8 6.1 -0.2,0.2:-0.5,9.3 5.8
       (should give: 5.42579,5.57989)
```

A line is given as two colon-separated points, and points are given as two comma-separated floating-point coordinates.

An example of the SVG output is given above.

There's also a GTK-based graphical program called `visualizer` that displays the illustration in a window. Currently it doesn't take command line arguments to define the problem parameters, but this is planned as a future enhancement. A further enhancement will be to allow direct manipulation of the parameters by dragging lines.

## The Code

The 2D-vector code has been split out into a small library in a subdirectory (`geometry2d`). It uses overloaded C++ operators to provide a visually pleasing representation of standard geometrical operations, including:

* Dot product
* Cross product
* Intersection
* Parallel offset

Non-operator functions include:

* Perpendicular direction
* Normalization

## Building

The basic command-line program (`township-roads`) has no dependencies and can be built using `make`. The `Makefile` also has a `check` target that compares the SVG output with a pre-stored version.

The graphical program (`visualizer`) depends on `gtkmm-2`. It uses an autotools build system and can be built with:

```shell
autoreconf -if
./configure
make
```

The `visualizer` directory also has Nix files, so a shell with dependencies can be started with:

```shell
nix develop -f shell.nix
```

Alternatively, a copy of `visualizer` can be built using

```shell
nix build -f shell.nix
```
