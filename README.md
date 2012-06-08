Flatland
========

A prototype for a physics-based game idea written by Marc Lepage in a few months in 2003. The idea was it might be fun to flick things around the screen (Pocket PC or Palm) like air hockey or shuffleboard or pinball. Sort of an Angry Birds before its time.

Because this was in the days before Box2D, development started out by writing a simple physics engine. Basically, this was like a 2D version of Quake, using polygonal brushes/models/entities, and parametric equation solving to compute collisions. Good times.

Windows executables are provided for each milestone release. Instructions are in the release notes below. Enjoy!

Release History
===============

Milestone 7 - Oct 12, 2003
--------------------------

M7 is the "more editor and widget toolkit" milestone. The major user visible feature is a lot of work on the level editor. The major behind the scenes feature is the implementation of a custom widget toolkit.

First, the widget toolkit. It is essentially a watered down clone of qt, with some fundamental differences. First, qt uses its own custom signals/slots implementation; I used the public domain sigslot library from http://sigslot.sourceforge.net/. Second, event processing for my dialogs is totally different. Finally, there are other little differences in the way processing is handled that are tailored to my environment.

Right now my widget toolkit is neither broad nor deep. I obviously haven't implemented all widgets (breadth), and the ones I have implemented are missing some features (depth). I have only done as much as I need to for my game, and I have cut corners where feasible. That said, I find the toolkit to be more flexible and reusable than custom coding each window gadget individually.

I have button widgets for push buttons and tool buttons. I have a line edit widget for text input. I have a keyboard widget which fires genuine key events. And I have a dialog widget for file selection.

Currently, the only functionality using the widget toolkit is the save model dialog. I still have to change older functionality to use the widget toolkit.

Now, the editor. It is now possible to create a model (but not level) specification using the editor. I'll provide a short tutorial after I describe the editor's operation and commands.

The edited model consists of points, lines which join points, and polygons which are formed of lines. Right now there is no checking of this structure. A model will ultimately be compiled to a form which is just convex polygons of points. Right now there is no checking that polygons are convex. The edited model is distinct from a compiled model; they live in different files.

The file names of the initial edit model and image are hardcoded. The image is "data/images/edit_model.png" (or .jpg) and the model is "data/edits/edit_model.txt". The edit model can be empty but must at contain at least what the default edit model contains (just open the file). There is also a non-empty sample edit model you can look at to see the file format.

The model must cover the image. That is, the furthest points in each direction must match the four edges of the image. Currently, you'll have to do this manually. If you are off by a pixel or two, the image will not render exactly over top of the model during game play.

The toolbar buttons are as follows:

1) Select/move mode. In this mode, you can select points or lines (but not polygons). Just click close to a point or a line. Hold CTRL to multi-select (but it won't toggle selection as it should). There is a bug where you can multi-select points if only lines are selected. If you click where there is no point or line, you will deselect (unless you are holding CTRL). If you drag where there is no point or line, you will make a selection lasso. If you drag a point or line, it will move appropriately. However, if you have multiple lines selected, you will have to hold CTRL to drag them all (this is a bug, you don't have to hold CTRL to drag multiple points).

2) Create new point mode. Whereever you mouse down, you will create a new point.

3) Create new polygon mode. Wheever you mouse down, you will create a new polygon (actually a triangle).

4) Create polygon from points command. This will create a polygon from the points you selected. It will use the points in the order you selected them (which may not be convex, just rearrange them afterward). It will reuse lines between the points if they exist.

5) Create reverse polygon from points command. This is the most complex command in the editor, and is intended for making world models. It works on selected points, and I think it reuses existing lines, but it's intended for newly created points especially. The points must meet certain criteria (which aren't verified). Namely, there must be a top left and a bottom right point which define an imaginary rectangle around all the points; these points will be found if they exist in the selection. Next, each remaining selected point will be closest to one side of that rectangle (if it's closest to two sides, one will be picked); each side must have at least one such closest point. Finally, the points closest to a side must not loop back on themselves (actually it's probably OK but you won't get the results you intend). If all this works, then the rectangle will be filled in with polygons around the sides towards the remaining points, leaving the centre open. I guess you have to see it in action.

6) Delete command. Not yet implemented.

7) Split line command. Not yet implemented.

8) Grid command. Not yet implemented.

9) Toggle model display command. This toggles the model display on and off. Currently, user interaction continues if the display is off; this is a bug.

10) Toggle image display command. This toggles the image display on, off, and transparent.

Right now, the first three modal tool buttons do not indicate which mode you are currently in, so be sure to remember, and if in doubt click it again.

You cannot save your edit model, but you can compile your edit model into a model via the menu. Just type the name (no extension) and it will be saved into the application's directory. Existing files will be silently overwritten! So if you type "my-model" then a file will be saved "my-model.txt". To give it a try, move this file to "data/models/my-model.txt", manually include it in a level, and set that level as first_level in the config file.

Now, a short tutorial on making a simple world model. First, take out a sheet of paper, draw your world, and scan it in. Make it donut-shaped, rectangular on the outside, but wavy on the inside. Or just use the default image I have provided.

Next, open the editor and verify that your image is present. Change to create new point mode, and create two new points at the top left and bottom right of the image. As explained earlier, these points should be *exactly* on the corners of the image, move them as close as you can to where they should be (in select/move mode, of course).

Now, along the interior of your world image, create new points whereever you want a polygon point to appear, on the edge between the playing area and the inside of the walls. Put more points where there are more curves.

Remember, you can place new points in this mode, but you cannot move them. Just place a bunch of points, then switch to select/move mode to finalize their position. Don't make too many extra points as you cannot delete them. (Just leave them off to the side if you do.)

Once you are satisfied, change to select/move mode and select all points (but not any extra ones you accidentally made). Since there is no autoscroll, the easiest way to do this is to use the lasso and the CTRL key, manually scrolling in between selections. Be sure all points are selected.

Now, the moment of truth. Click the create reverse polygon from points tool button. Your simple world model will be made. You can tweak it as you like before you compile it into a model.

Once you have your model compiled, move its file into the models directory. Copy a level file to make a new one, and change it to use your world model and image. Make sure other level elements are in the right place for your new world model. Change the first_level config variable to your new level name, and you can try your new world model.

Program controls remain:

Menu: access with ESC, use with ARROWS/ENTER or mouse.
Scrolling: use ARROWS; ENTER toggles autoscroll (in game).
CTRL key: multiple selection in editor; toggle rendering in game.

Milestone 6 - Sep 28, 2003
--------------------------

M6 is the "editor" milestone. The major feature is the beginning of a level editor.

The editor currently loads an image and a model from hardcoded file names. Currently, the model doesn't match the image. This isn't a problem, as eventually you'll be creating your own model to match your own image.

In the editor, you can scroll the view or access the menu as normal. You can select and move either a point or a line. Holding down the control key will allow you to select more than one point or line.

Support has been added for rotated entities. The entity can be rotated at any angle, and will render and bounce correctly. However, it won't change its angle due to physical interaction (i.e. no angular forces or impulses in the simulation).

There are some new graphics in the demo level.

Images can now be JPEG in addition to PNG.

New config variables determine whether the console and frame info are displayed.

Some of the controls have changed.

Menu: access with ESC, use with ARROWS/ENTER or mouse.
Scrolling: use ARROWS; ENTER toggles autoscroll (in game).
CTRL key: multiple selection in editor; toggle rendering in game.

Milestone 5 - Sep 24, 2003
--------------------------

M5 is the "state" milestone, adding various elements of program state, plus a new level and a few gameplay enhancements.

The new level is hand-drawn using pencil and markers. It was imported using a digital camera (no scanner yet) and cleaned up in The GIMP, where an animated background was added.

The "drill" entity was created by taking multiple photos of a rotating drill bit against a blue screen (actually a recycling box).

The program now goes from the splash screen to a title screen. When a game is started, it displays a loading screen, then runs the game. When the game is finished, it returns to the title screen. Upon quitting, it displays a nag screen (eventually only for unregistered copies).

An onscreen menu is active on the title screen, and during game pause. It is drawn primitively in this release, but reacts properly to the keys and stylus. You can use the stylus, or the keys up, down, and enter. The game pause menu can also be dismissed by unpausing, of course.

Currently, the only title screen menu items that work are "new game" and "quit". During game pause, all three menu items work: "resume game", "end game", and "quit".

Autoscroll is now implemented. The enter key toggles autoscroll on and off. Scrolling manually with the arrow keys also has the effect of turning autoscroll off. The entity that will be autoscrolled is the last player entity you controlled.

A new entity type "force" has the ability to capture the player entity after a delay. When overlaid over a control entity, this makes game play much easier.

The program controls are now as follows:

- Stylus: Menu and game interaction.
- Arrow keys: Menu interaction and game scrolling.
- Enter key: Menu selection and autoscroll toggle.
- 3 key: Toggle game pause (with menu).
- 2 key: Single step a game frame.
- 1 key: Toggle wireframe graphics.

Milestone 4 - Sep 02, 2003
--------------------------

The fourth milestone is the "animation" milestone.

Images are now no longer contained directly by models or entities, but are accessed through animations. An image once again spans the bounding box of a model, but can contain multiple tiled frames. Then, an animation specifies which frames to display, and for how long. Multiple animations can reference the same image. Animations can be defined inline, or placed in their own file for reuse. See the sample level for details.

A splash screen now displays at program startup.

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
