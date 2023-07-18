
# Image Manipulation using Quadtrees

Quadtrees simplify various image manipulation functions.This project contains various image manipulation functions like Compression of an .ppm Image(Binary File), Decompression, Image Rotations(Upside-Down, Flip, Left by 90 degrees, Right by 90 degrees), Image Filters(Gray, Sepia, Negative) and Union/Overlay of two Images. To know more, go through above presentation. 



## Run Locally

Clone the project

```bash
  git clone https://github.com/Anvita0305/Image-Manipulation-Using-QuadTrees
```
## Build
```bash
  make final
```
```bash
  For Compression(image to binary file):
  ./final -c threshold input.ppm output.out
```
```bash
  For Decompression:
  ./final -d input.out output.ppm
```
```bash
  For Rotations:

    a)Mirror Image
    ./final -r m threshold input.ppm output.ppm

    b)Water Image
    ./final -r w threshold input.ppm output.ppm

    c)Rotate Left by 90 degrees
    ./final -r l90 threshold input.ppm output.ppm

    d)Rotate Right by 90 degrees
    ./final -r r90 threshold input.ppm output.ppm
```
```bash
  For Converting an Image to Grayscale:
  ./final -g threshold input1.ppm input2.ppm output.ppm
```
```bash
  For Converting an Image to Negative:
  ./final -n threshold input1.ppm input2.ppm output.ppm
```
```bash
  For Converting an Image to Sepia:
  ./final -s threshold input1.ppm input2.ppm output.ppm
```
```bash
  For Union of two Images:
  ./final -u threshold input1.ppm input2.ppm output.ppm
```



## Screenshots

### Original Image
![Original](https://user-images.githubusercontent.com/78889572/173231286-5e2e311a-45b6-4998-87bf-9e7cdc7afd0a.png)

### Mirror Image
![mirror](https://user-images.githubusercontent.com/78889572/172678341-768fb128-34bb-4c75-8a56-c993eb0437e9.png)

### Water Image
![water](https://user-images.githubusercontent.com/78889572/172678562-b414d5e3-ce09-4e2b-8c3b-78ab332a07df.png)

### Rotate Left by 90 Degrees
![Left](https://user-images.githubusercontent.com/78889572/172678940-a5ee9ae8-430e-4b2b-bde2-1a2ebb6cbc9a.png)

### Rotate Right by 90 Degrees
![Right](https://user-images.githubusercontent.com/78889572/172679017-e6302e00-66ac-4ef7-b030-37bd297c8323.png)

### GrayScale Image
![Gray](https://user-images.githubusercontent.com/78889572/172679616-97dd7dd2-786d-4b60-982e-19098dbd9173.png)

### Negative Image
![Negative](https://user-images.githubusercontent.com/78889572/172679670-e557ed2b-f7f9-4cd0-8c7d-e598cbad0f46.png)

### Sepia 
![Sepia](https://user-images.githubusercontent.com/78889572/172679695-f373ad42-c82a-4baf-be93-85393e61ed13.png)

### Union of Images
![union](https://user-images.githubusercontent.com/78889572/173231336-05587d19-2392-4707-a7e3-6e53f3e85879.png)


## Author

Anvita Mahajan- MIS 112003080
