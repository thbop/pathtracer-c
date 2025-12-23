# pathtracer-c

Writing another pathtracer in C.

## Build Instructions
- Download the devel-mingw version of [SDL3](https://github.com/libsdl-org/SDL/releases/tag/release-3.2.28)
- Extract the `SDL3` folder containing headers to the include folder
- Create a `/bin` directory (in the main directory) and extract `SDL3.dll` into it
- Create a `/lib` directory (also in the main directory) and extract `libSDL3.dll.a` into it
- Download the latest release of [chelp](https://github.com/thbop/chelp/releases) and extract libchelp.a into the `/lib` directory (the headers for chelp are already included in this repo)
- Run `make`, `pathtracer.exe` should be generated in the `/bin` folder

## Reports

<details>
    <summary>Chapter 0</summary>
    <img src="images/0.png" />
    <p>In this section, I created a thread which renders pixels to the screen using SDL3.</p>
</details>

<details>
    <summary>Chapter 1</summary>
    <img src="images/1.png" />
    <p>In this section, I created a camera and generated rays for each pixel (ray directions represented as colors).</p>
</details>

<details>
    <summary>Chapter 2</summary>
    <img src="images/2_sphere_intersection.png"/>
    <img src="images/2.png"/>
    <p>In this section, I solved for ray-sphere intersection, drawing a flat sphere.</p>
    
</details>

<details>
    <summary>Chapter 3</summary>
    <img src="images/3_antialiasing.png" />
    <img src="images/3.png" />
    <p>In this section, I upscaled the render output, flipped the vertical axis right-side-up, and implemented configurable subpixel rays for antialiasing.</p>
</details>

<details>
    <summary>Chapter 4</summary>
    <img src="images/4.png" />
    <p>In this section, I started bouncing light.</p>
</details>

<details>
    <summary>Chapter 5</summary>
    <img src="images/5_object_design.png" />
    <img src="images/5.png" />
    <p>In this section, I refactored the intersection tests and material properties into separate classes. Shapes and materials ought to be more extendable.</p>
</details>

<details>
    <summary>Chapter 6</summary>
    <img src="images/6.png" />
    <p>Successful render of multiple spheres, though light does not appear to be behaving as expected.</p>
</details>

<details>
    <summary>Chapter 7</summary>
    <img src="images/7.png" />
    <p>Fixed some mistaken math and made rays sort objects better. Now it is kind of rendering, though I believe the green sphere should not be that flat.</p>
</details>

