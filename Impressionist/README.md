# Impressionist
check list:  
W per new brush	&emsp; 		 			yes (magic star brush & scatter magic star brush)      
W border clipping	&emsp;				yes    
W (red) marker	&emsp;					yes  
W swap	&emsp;						yes   
B RGB scale		&emsp;				yes (note 1)  
B blur/sharp brush	&emsp;				yes  
B 1 level undo		&emsp;				yes  (Flie->Undo)
B dissolve		&emsp;				yes (File-> LoadAnother)  
BW fade in/out input on output canvas	&emsp;		no  
BW mural (blending)		&emsp;			yes (File-> New Mural Image)  
BW alpha brush	&emsp;					yes  (note 2)  
BW automatic paint whole image		&emsp;		yes  (note 7)
BW gradient from user-specified image	&emsp;		yes (note 3)  
BW edge clipping			&emsp;		yes  (note 5)
2B user-specified kernel	&emsp;			no  
2B edge detection + user-specified edge clipping&emsp;	yes (note 4)
2B multiresolution&emsp;					no  
2B warp brush		&emsp;				no  
2B curved brush following image gradient&emsp;	no  
4B mosaic				&emsp;		no   
4B impressionist video with temporal coherency&emsp;		no  
8B Bayesian matting			&emsp;		no  
On Bells & Whistles

## Extra Notes
1. Possible Bug on Bell: RGB scale with initialization of Fl_Color_Chooser Class, if called rgb(), invalid memory access may happen, 
the way to do it without leading to error is to uncomment lines 56 ~ 62 and manually set the values for rgb in the color setting dialog.
2. To use alpha brush, you need first load an image as alpha map. Click File->Load Alpha Map, then File->Brush->Alpha Brush  
3. To load a user-specified image as a gradient map, you need to load a image as background first (File->Load Image). Then you can choose a image with size same as the background (File->Load Gradient Map). Next click File->Brushes->choose Gradient Brush Based On Other Image.
4. The edge detection is displayed by enabling users to automatically paint an edge image given a base image on the paint view canvas, and user can toggle the threshold for this edge auto-generation in the brush dialog. The light button for user-specified edge clipping is also in the brush dialog, called Custom Edge Clip.
5. The "Another Image Edge Clip" to the left of "Custom Edge Clip" is for the purpose of clipping to the edge of a specifically loaded edge image. To use this function properly, user has to first load an image as base map, and then load the corresponding edge map. Click File->Load Image, then click File->Load Edge Map.
6. Edge clipping functions in 4. and 5. are both for Line Brush.
7. Automatic Painting Control Widgets are all located in Brush Dialog, below Alpha Value Silder, where from left to right, user specify the average space between each brush stroke, whether Randomized Size based on current Brush Size is enabled, and Generation of the Auto Painted image given the parameters-setting.
