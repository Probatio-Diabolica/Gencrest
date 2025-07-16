# Hill Climbing Image Reconstructor 
An image reconstructor that recreates an image using a steepest ascent hill climbing algorithm.

## Features::

- Hill climbing optimization
- Procedural shape-based image generation
- Uses SFML 3.0 for rendering

## Working::

The algorithm starts with a blank canvas and iteratively adds random ellipses. If a new ellipse improves the image's similarity to the target image, it is kept; otherwise, it's discarded.

Each step:
1. Mutate the latest ellipse
2. Render to a new buffer
3. Compare fitness to original image
4. Keep or discard based on improvement

## Dependencies

- **SFML 3.0**
- **CMake ≥ 3.20**
- **C++23-compatible compiler** (GCC 13+)
- **Unix-like System**

## Build Instructions for unix
```bash
git clone https://github.com/Probatio-Diabolica/Gencrest.git

./build.sh
```

## Running
```bash
./picasoSingh <image_path> <no_of_ellipses >= 1>
```