ResizableArray
==============

Implementation and tests for my paper about Resizable Array which uses \sqrt[k](n) additional memory.
A resizable array, or a vector implements following interface defined in AVector.h :
* void push(int e);
* int & at(size_t idx);
* size_t size();
* size_t allocatedSize(); //for debug purpose: reports the total size of currently allocated memory
* int pop();

Table of contents
-----------------
* RegularVector.* my implementation which uses STL's vector, tweeked, to half the size whenever it is only quarter full
* WaterlooVector.* my implementation based on 
[Resizable Arrays in Optimal Time and Space](https://www.google.pl/url?sa=t&rct=j&q=&esrc=s&source=web&cd=1&cad=rja&ved=0CC8QFjAA&url=https%3A%2F%2Fcs.uwaterloo.ca%2F~imunro%2Fresize.ps&ei=XYM8UcHgKcXtsgbv7IBY&usg=AFQjCNHjrPqITgpmZe37sDibKM33seAJYg&bvm=bv.43287494,d.Yms)
by Andrej Brodnik, Svante Carlsson, Erik D. Demaine, J. Ian Ian Munro, Robert Sedgewick
* SuccinctVector.* my implementation of my Resizable Array data structure from [my paper](vanisoft.pl/~lopuszanski/public/resizable_arrays.pdf).
* all other files are just correctness, and speed tests.
