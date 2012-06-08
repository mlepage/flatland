Flatland
========

A prototype for a physics-based game idea written by Marc Lepage in a few months in 2003. The idea was it might be fun to flick things around the screen (Pocket PC or Palm) like air hockey or shuffleboard or pinball. Sort of an Angry Birds before its time.

Because this was in the days before Box2D, development started out by writing a simple physics engine. Basically, this was like a 2D version of Quake, using polygonal brushes/models/entities, and parametric equation solving to compute collisions. Good times.

Windows executables are provided for each milestone release. Instructions are in the release notes below. Enjoy!

Release History
===============

Milestone 3 - Aug 30, 2003
--------------------------

The third milestone is the "gameplay" release.

The first big enhancement is support for entity types. There are a few predefined entity types:

world - this should be the first entity.
monster - uncontrollable entities.
player - controllable entities.
control - a non-solid area for controlling player entities.
block - these are destroyed when player entities hit them.
path - these move around on a path.

The new "breakout-1" level illustrates all of these entity types except monster.

In the data files, support has been added for anonymous models. These are models that are defined right in the entity. They will not have an image, unless you also declare an image_name (and this must be after the model). Another addition is the attributes to support path entities. This is a bit of a kluge right now, setting a path_speed and a number of path_point attributes.

A new configuration variable has been added to set stylus_easy_mode. You can disable it for the older (harder) behaviour.

Top speed of thrown player entities is now limited. Also, friction will slow them down after a while. If you come to a dead stop, and a path entity doesn't bump you, simply exit and restart.

There have been some tweaks to the collision response, mostly because of the complication of path entities. The app should now gracefully exit if an infinite case arises, complaining of "too many infinite stuck" (exact wording may vary).

The controls remain:

stylus - control player entity in control area.
arrows - scroll view.
1 - pause/unpause.
2 - single step frame.
3 - exit.

Milestone 2 - Aug 23, 2003
--------------------------

I'm happy to report the release of Milestone 2 of my program. This release predominantly features user customizable options, levels, models, and graphics. It also upgrades to GapiDraw 2.04 (see www.gapidraw.com for details).

Let me go over the M2 release's files. In the application directory, there's the executable, a log file, and a config file. The config file contains these options:

    #set render_brush_bounds = 1
    #set render_brush_outline = 1
    #set render_entity_bounds = 1
    #set render_entity_identifier = 1
    #set render_entity_velocity = 1
    set render_entity_image = 1
    #set render_model_bounds = 1
    set first_level = doe3

All of these commands are run at program startup, and merely set configuration variables. Hash marks are comments and just a quick way of disabling commands. The render config variables control various things that are rendered. The only caveat is that if images are rendered, you can't see much else due to overdraw; so I recommend disabling images if you want to try things like brush outlines and bounds. The first level is the name of the level that is loaded and run.

The data directory contains all the data files, in three subdirectories: levels, models, and images.

Levels contain the levels that are loaded according to the first_level config variable. For example, "doe3" causes data/levels/doe3.txt to be loaded. That level looks like this:

    entity
    {
        model = doe
        mobile = 0
        colour = <0, 0, 127>
    }
    entity
    {
        model = ufo
        colour = <127, 127, 0>
        mobile = 1
        position = <300, 50>
        velocity = <1, 2>
    }
    entity
    {
        model = ufo
        colour = <127, 127, 0>
        mobile = 1
        position = <350, 250>
        velocity = <-2, -1>
    }
    entity
    {
        model = ufo
        colour = <127, 127, 0>
        mobile = 1
        position = <450, 150>
        velocity = <-1, -1>
    }

You can edit the level as you wish, just stick to the format in the sample levels. The first entity is the world, the rest are typically the mobile entities. What's important here is that models are specified by name.

Models live in the models subdirectory. These are just text files like this (data/models/doe.txt):

    model
    {
        brush
        {
            <0, 0>
            <345, 0>
            <345, 6>
            <190, 56>
            <0, 56>
        }
        brush
        {
            <345, 0>
            <465, 0>
            <465, 89>
            <365, 118>
            <345, 6>
        }
        brush
        {
            <465, 0>
            <472, 0>
            <472, 171>
            <466, 171>
            <465, 89>
        }
        brush
        {
            <472, 171>
            <472, 267>
            <393, 267>
            <379, 192>
            <466, 171>
        }
        brush
        {
            <472, 267>
            <472, 320>
            <271, 320>
            <271, 295>
            <393, 267>
        }
        brush
        {
            <271, 320>
            <0, 320>
            <0, 56>
            <190, 56>
            <271, 295>
        }
    }

Brushes are convex polygons in counter-clockwise order. You can have as many as you want, but if they are for a world, they should obviously enclose the level. If they are for a moving object, they may not bounce entirely "correctly" if they have more than one brush.

The model's brush vertices implicitly define a bounding box. For the doe model above, it's from <0,0> to <472,320>, but it need not start at <0,0>. Each model requires a corresponding image (even if you aren't rendering images) that is the same total size as the model's bounding box (this isn't currently checked though).

Images live in the images subdirectory, as PNG files. So you can see doe.png is size 472x320. (It's an image of Doe Library at a California university.)

So there you have it. You can make your own models (spec + image), levels, specify which level to load, and adjust some rendering options. Key controls are as follows: 1 is pause/unpause, 2 is frame step, 3 is exit. The log might help if you have problems, otherwise let me know.

Milestone 1 - Aug 01, 2003
--------------------------

Basic geometry is working, you can see a hardcoded map with entities and scroll around a bit.
