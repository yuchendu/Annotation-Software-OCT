# Annotation-Software-OCT
This is an in-house software for lesions annotation of OCT images. Up to six different types of lesions are supported to be annotated. The software is implemented by C++, supported by the libraries of vtk, itk, and Qt.

## The Summary of the Software
The main window of the software is shown as below:

![The main window](https://github.com/yuchendu/Annotation-Software-OCT/blob/main/assets/images/image001.jpg)

## Annotation Procedure
### Image loading
OCT image could be loaded via push the "Open" button. Only one image could be loaded in for each time of annotaion.

![The main window](https://github.com/yuchendu/Annotation-Software-OCT/blob/main/assets/images/image012.jpg)

### Single lesion region annotation 
The left part of the window is the operation panel. It supports up to six types of lesions to be annotated, seperately. Pushing the button, the annotation procedure would be activated.

![The main window](https://github.com/yuchendu/Annotation-Software-OCT/blob/main/assets/images/image014.jpg)

For each type of lesions, up to 12 contours are supported. Pushing "+" to add a new contour, while pushing "-" to delete one. 

![The main window](https://github.com/yuchendu/Annotation-Software-OCT/blob/main/assets/images/image016.jpg)

### Annotated images saving
Push "save image" button to save the annotated binary images.

![The main window](https://github.com/yuchendu/Annotation-Software-OCT/blob/main/assets/images/image019.jpg)

## Contact
If you have any questions, please feel free to contact yuchendu@rocketmail.com
