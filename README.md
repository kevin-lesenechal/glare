# GLARE: OpenGL, (A) Rendering Engine #

The GLARE (`libglare`) is a simple, lightweight rendering engine built over
modern OpenGL (3.3+). It is written in C++17, and aimed to be multiplatform
(see below).

This project is highly experimental and just started recently: it is absolutely
not stable, not yet documented, and not ported to anything else than GNU/Linux.

## Currently implemented features ##

 * C++ RAII wrappers over many OpenGL objects, with proper constructors,
   destructors, and move semantic; bindings are avoided through the use of
   Direct State Access if available (otherwise, GLARE falls back on bindings).
 * Basic shading management: loading shaders from files, compiling, linking
   programs; a GLSL preprocessor is included to apply some source
   transformations including macro definitions injection and file inclusion.
 * Texture loading: support for most types of textures, including 1D, 2D, 3D
   textures, array of textures, cubemaps, compressed textures; the library uses
   texture storage capacities if available through extensions.
 * A KTX texture loader is included based on the internally-developped library
   `libsimple_ktx` (see below).
 * Camera management: a configurable freefly camera controller is included that
   can be controlled with mouse and keyboard.
 * Skybox utilities (no shader included yet).
 * Model loading using Assimp, all textures and materials are fetched and
   organized; rendering of thoses models is also implemented.
 * Texture library to manage resources and load them if necessary from the
   filesystem.
 * Framebuffer and Renderbuffer objects are provided.
 * Basic off-screen rendering capabilities are offered for HDR rendering.
 * Gamma correction and support for proper sRGB/linear RGB textures.
 * Vertex attributes management through VBOs, VAOs, EBOs, etc.
 * Basic geometric primitives generation (unit cube, axis).
 * Debugging and testing GUI using Dear Imgui.
 * Input events management (keyboard, mouse, etc.).

## Planned features ##

Some ideas of features to implement in the near futures (may be update anytime):

 * Bloom effect;
 * Better GLSL inclusion mechanisme (including recursion and file names);
 * Antialiasing for HDR rendering;
 * Implement viewport resize for HDR rendering (urgent);
 * Include stock Blinn-Phong shading shaders alongside the engine;
 * Shadow mapping with multiple source lights;
 * Light spots support (currently only point and directional lights available);
 * Proper documentation (very important in my opinion);
 * Public example program;
 * Windows support (currently only developed and tested on GNU/Linux).

Feel free to suggest any additional feature!

## External dependencies ##

GLARE uses some third-party libraries either statically linked and embedded in
this repository, either dynamic linked (must be installed by user, checkout
your distribution to find the appropriate packages).

GLARE (rendering engine) dependencies:

 * **C++17** or later, currently some GCC extensions are used and compiler
   portability may be added, even though I'm not convinced of the necessity of
   supporting anything else than GCC. GLARE may take advantage of C++20 when the
   standard is officially published.
 * **OpenGL 3.3+** is required for rendering, the core profile is used.
 * **libepoxy** for querying and loading OpenGL extensions.
 * **glfw** to portably create the OpenGL context and manage the inputs.
 * **stb_image** (embedded), currently mandatory but may be made optional in the
   future; this lib is used to load a few image formats (mainly PNG and JPEG);
   KTX textures should be preferred.
 * **imgui** (embedded), for debugging GUI currently, may be made optional later
   for non-development versions.
 * **assimp** for loading models.

Internal tools (see `tools` directory) dependencies:

 * **Qt5** for the graphical interface of `ktx_viewer`.
 * **ImageMagick7** for image manipulation inside the KTX file converter tool
   `ktx_export`.

## Internal libraries ##

The GLARE project embeds some internally-developed libraries for both its own
use and the user's; currently, thoses subprojects are part of GLARE but will,
in the short term made standalone to be used in any projects outside GLARE.

Thoses libraries are:

 * `libsimple_ktx` (located at `libs/simple_ktx`): a simple library utility to
   read and write Khronos' KTX texture file format. It used inside GLARE itself
   for loading textures (compressed, non-compressed, cubemaps, etc.) and by the
   `ktx_export` tool (see below).
 * `gl_tables` (located at `libs/gl_tables`): library used to query names and
   values of OpenGL constants and certain texture format properties.

## Internal tools ##

Some programs are embedded with this project. You may want, or not, to build
them alongside the rendering engine library. Please note that some additional
dependencies will be required (see above).

 * `ktx_export` (located at `tools/ktx_export`) is a command-line tool to
   generate KTX texture files from ordinary image formats (including PNG, Targa,
   JPEG, etc.). It is able to compress them (typically using S3TC/DXT5 formats),
   to ensure proper image orientation, colorimetric spaces (sRGB), generate and
   save mipmaps. It can also handle 3D textures, arrays of textures and
   cubemaps.
 * `ktx_viewer` is a graphical Qt5 interface to view and inspect KTX texture
   files; it can display all the images embedded inside the KTX format including
   any mipmap level, array layer, z-slice, cubemap face. It also displays meta
   information about the texture (formats, key-values, dimensions, ...) and you
   can inspect the alpha channel (either alpha alone, RGB only or both).
