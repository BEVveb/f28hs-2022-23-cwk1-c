# Who I am

- Name: Bogdan Egorov
- Student ID: H00418676

# Program design

### Problem Analysis

The goal of the coursework is to work with PPM images, furthermore, questions 1-2 are fully
focused on structuring, reading, writing, encoding & decoding PPM images. When it comes to 
the first 3 points, the problem is straight forwards of having the program to understand
the magic number, width, height and max pixels of the picture, and producing relevant 
messages if there is an error. Encoding and decoding functions are also complex, as
for encoding, characters have to be randomly allocated to red pixels, and 
decoding has to go through all of the mentioned pixels and find these characters,
in order to output them.

PPM images, except for the magic number, which should be char, should be identified as
integers, however the coursework stated that "All integers are in decimal.", which
brings in the confusion of using float instead of integers. In the end, it was decided
to use integers for the actual values. RGB is mentioned beforehand, and for the new image 
(encoded), all of the mentioned aspects should be transferred without any major changes. 

Taking regards of Q4, it was decided to edit the showPPM and getPPM functions, so that they would be
able to read P6 PPM images as well. To work with P6, a dynamically allocated array should be created,
which should store the bytes of colours. 'fread', 'fwrite' should be used respectfully.

The outputs of the whole CW should be a copy of PPM image, or an encoded version of it.

### Program Structure

The program should be linearly done, as questions 1-2 are dependant on each other. Yet
to check the implemenation of encoding and decoding, question 3 has to be fully done, thus
when encoding was reached, question 3 was completed at the same time. Question 4 was done
last, implementing the possibility of working with P6 images as well.


# Choice of data structures and algorithms

### Data Structures and Algorithms for 'getPPM'

- The algrorithm checks if PPM image is valid (magic number, width, height, max), then allocates the 
information into pixels. Then checks if the PPM image is P3/P6, and allocates the pixels accordingly.
Finally, creates the actual image and copies the required information into it, returning the image.
- Data structures such as struct (to represent and store info for PPM, Pixel), 
arrays (static, 'char magic_num[3]', and dynamic, /unsigned char bytes'), and pointers 
(to an array of 'Struct Pixel') are used.

### Data Structures and Algorithms for 'showPPM'

- The algorithm prints all of the information regarding the PPM image. If the image is P3, copies
the data using decimals accordingly, if P6, copies using bytes.
- Data structure of struct is used, specifically of Pixel, to copy the information from it into the 
PPM image for P6.

### Data Structures and Algorithms for 'encode'

- The algorithm creates a 'newimg' to store the original PPM image data into it, to then copy all
of the previous data into it. Then it allocates memory for it and begins inputting the text into the
encoded PPM image. It does so by iterating through the whole image, chosing randomly between 0 and 1 byte
and if 1 is chosen and there is still characters left, stores the char in the least significant red pixel.
Bitwise is used in the process of storage, to combine the least significant bit of red pixels with the bit
of the character to be encoded. Once one of the characters is encode, the index is increased and process
continues until all of the text is stored. Then the modified pixels are stored within the newimg and the
newimg is returned.
- The same data structures as in showPPM are used.

### Data Structures and Algorithms for 'decode'

- The algorithm checks if the old and new image are of the same sizes, and then calculates the length
of the hidden text within the image. It then loops through the old image pixels, and compares them
with the new pixels, if not same, copies the ASCII value of the character and send it into an
array which stores the encoded text. Once all of the characters are found, the code breaks and
the text is returned. 
- The same data structues as in showPPM and encode are used, for the exception of 'char*', which is a
dynamically allocated character array, used to store the encoded message within it.