### Scop
A GPU model renderer written in C++.

#### *️⃣ Description
Scop is a model renderer running on OpenGL. It can load models in the .obj format, material libraries in the .mtl format and texture images in the .bmp format.

After loading the model, you can move it around, and display its triangles, texture or normals.

You can also take a screeshot of the image currently displayed.

Some example models, materials and textures are provided in the resources directory. The textures must be specified inside the material to be loaded.

![Bust](./previews/bust.png)
![Apple](./previews/apple.png)
![teapot](./previews/teapot.png)
![horse](./previews/horse.png)

It relies on the [LLA](https://github.com/Samael1254/LLA) linear algebrea library for mathematical operations.

#### 📦 Installation and usage
To compile the binary, just clone the repository and make the project:
```
git clone --recurse-submodules https://github.com/Samael1254/scop
cd scop
make
```
⚠️ This project uses submodules for libraries. If you forgot to clone them recursively, execute this command in the repository:
```
git submodule update --init --recursive
```
You can then run the program by providing it a .obj file
```
./scop resources/models/mymodel.obj
```

#### ⚙️ Configuration

You can configure scop in the scop.toml file. In it you can change parameters such as background color, fullscreen, window size, and others.

#### ⌨️ Keybindings
- WASDQE: Move model around
- Arrows: Rotate model
- Mouse wheel: Scale model
- HJKL: Rotate light
- T: Change display mode (plain, triangles, texture, normals)
- P: Change polygon mode (points, lines, triangles)
- Y: Enable antialiasing
- Space: Take screenshot

#### ⏩ Future improvements
- Adding normal maps
- Parse mesh as seperate groups to apply different materials to the same model.
- Add support for compressed .bmp images.
- Give information on the model (vertices and triangle count, texture size...)
