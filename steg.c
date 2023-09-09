#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* The RGB values of a pixel. */
struct Pixel
{
    int red;
    int green;
    int blue;
};

/* An image loaded from a PPM file. */
struct PPM
{
    char magic_num[3];
    int width, height, max;
    struct Pixel *pixels;
};

/* Reads an image from an open PPM file.
 * Returns a new struct PPM, or NULL if the image cannot be read. */
struct PPM *getPPM(FILE *f)
{
    char magic_num[3];
    int width, height, max;

    if (fscanf(f, "%2s\n", magic_num) != 1) // If no magic number is found
    {
        fprintf(stderr, "Invalid PPM file: Missing magic number\n");
        return NULL;
    }

    if (fscanf(f, "%d %d\n%d\n", &width, &height, &max) != 3) // If the width, height or the colour range is missing
    {
        fprintf(stderr, "Invalid PPM image format: Missing dimensions or colour range\n");
        return NULL;
    }

    struct Pixel *pixels = malloc(width * height * sizeof(struct Pixel)); // allocate PPM images information into pixels
    if (pixels == NULL)                                                   // if nothing is found
    {
        fprintf(stderr, "Failed to allocate memory.\n");
        return NULL;
    }

    if (strcmp(magic_num, "P3") == 0) // if the magic number is 3
    {
        for (int i = 0; i < width * height; i++)
        {
            int red, green, blue;
            if (fscanf(f, "%d %d %d", &red, &green, &blue) != 3)
            {
                fprintf(stderr, "Invalid PPM image format: Pixel data missing\n");
                free(pixels);
                return NULL;
            }

            /*Assigns red,green and blue fields for the struct pixels */
            pixels[i].red = red;
            pixels[i].green = green;
            pixels[i].blue = blue;
        }
    }
    else if (strcmp(magic_num, "P6") == 0) // if the magic number is 6
    {
        /* create an array of bytes to store binary image*/
        unsigned char *bytes = malloc(width * height * 3 * sizeof(unsigned char));
        if (bytes == NULL)
        {
            fprintf(stderr, "Failed to allocate memory.\n");
            free(pixels);
            return NULL;
        }

        if (fread(bytes, sizeof(unsigned char), width * height * 3, f) != width * height * 3)
        {
            fprintf(stderr, "Invalid PPM image format: Pixel data missing\n");
            free(bytes);
            free(pixels);
            return NULL;
        }

        for (int i = 0; i < width * height; i++)
        {
            /*Assigns red,green and blue fields for the struct pixels using bytes */
            pixels[i].red = bytes[3 * 1];
            pixels[i].green = bytes[3 * i + 1];
            pixels[i].blue = bytes[3 * i + 2];
        }

        free(bytes); // memory freed
    }
    else // if neither P3 or P6
    {
        fprintf(stderr, "Invalid PPM image format.\n");
        free(pixels);
        return NULL;
    }

    struct PPM *img = malloc(sizeof(struct PPM)); // creates the actual PPM image
    if (img == NULL)
    {
        fprintf(stderr, "Failed to allocate memory.\n");
        free(pixels);
        return NULL;
    }

    /* Copies all of the required PPM information into the struct img */
    strcpy(img->magic_num, magic_num);
    img->width = width;
    img->height = height;
    img->max = max;
    img->pixels = pixels;

    return img;
}

/* Write img to stdout in PPM format. */
void showPPM(const struct PPM *img)
{
    /* Question 2b */
    /* Prints information about the PPM image*/
    printf("%s\n", img->magic_num);
    printf("%d %d\n", img->width, img->height);
    printf("%d\n", img->max);

    if (strcmp(img->magic_num, "P3") == 0) // if image is P3
    {
        for (int i = 0; i < img->width * img->height; i++)
        {
            /* Copies colours directly into the image */
            printf("%d %d %d ", img->pixels[i].red, img->pixels[i].green, img->pixels[i].blue);
        }
    }
    else if (strcmp(img->magic_num, "P6") == 0) // if image is P6
    {
        /* Uses fwrite to directly write the pixels in binary into stdout */
        fwrite(img->pixels, sizeof(struct Pixel), img->width * img->height, stdout);
    }
}

/* Opens and reads a PPM file, returning a pointer to a new struct PPM.
 * On error, prints an error message and returns NULL. */
struct PPM *readPPM(const char *filename)
{
    /* Open the file for reading */
    FILE *f = fopen(filename, "r");
    if (f == NULL)
    {
        fprintf(stderr, "File %s could not be opened.\n", filename);
        return NULL;
    }

    /* Load the image using getPPM */
    struct PPM *img = getPPM(f);

    /* Close the file */
    fclose(f);

    if (img == NULL)
    {
        fprintf(stderr, "File %s could not be read.\n", filename);
        return NULL;
    }

    return img;
}

/* Encode the string text into the red channel of image img.
 * Returns a new struct PPM, or NULL on error. */
struct PPM *encode(const char *text, const struct PPM *img)
{
    /* Question 2c */
    struct PPM *newimg = malloc(sizeof(struct PPM)); // creates struct newimg to place encoded data into

    /* Copy information of the old image into the new image */
    strcpy(newimg->magic_num, img->magic_num);
    newimg->width = img->width;
    newimg->height = img->height;
    newimg->max = img->max;

    newimg->pixels = malloc(sizeof(struct Pixel) * img->width * img->height); // allocates memory for new image
    if (newimg->pixels == NULL)
    {
        fprintf(stderr, "Failed to allocate data for the new image");
        free(newimg);
        return NULL;
    }

    /* Initializes a random number generator with current time
       (time isn't stored due to NULL)*/
    srand(time(NULL));

    int text_index = 0; // index to stop placing char within the image once all of the message is placed

    for (int i = 0; i < img->width * img->height; i++)
    {
        int random_bit = rand() % 2; // Calls a random number with range from 0 to 1
        struct Pixel pixel = img->pixels[i];

        if (random_bit == 1 && text_index < strlen(text)) // if the bit value is 1 AND characters left from message
        {
            if (pixel.red % 2 == 0) // checks if the red pixel is least significant
            {
                /* Least significant red pixel is replaced with character from message */
                pixel.red |= (text[text_index]); // Perfroms bitwise OR operation
            }
            else
            {
                // Pixel contains a character so move on
                continue;
            }
            text_index++;
        }

        newimg->pixels[i] = pixel; // Modified pixels are stored in newimg at the same index
    }

    return newimg;
}

/* Extract the string encoded in the red channel of newimg, by comparing it
 * with oldimg. The two images must have the same size.
 * Returns a new C string, or NULL on error. */
char *decode(const struct PPM *oldimg, const struct PPM *newimg)
{
    /* Question 2d */

    /* Checks if images are of same size */
    if ((newimg->width * newimg->height) != (oldimg->width * oldimg->height))
    {
        fprintf(stderr, "Images are not of the same size");
        return NULL;
    }

    /* Calculates text length that is hidden in the image.
    Multiplied by 3 because 3 colours RGB divide by 8 due to bits*/
    int text_len = (oldimg->width * oldimg->height * 3) / 8;

    /* Allocate memory for text*/
    char *text = malloc(text_len + 1);

    if (text == NULL)
    {
        fprintf(stderr, "Failed to allocate memory for the message");
        return NULL;
    }

    int text_index = 0;

    /* Loop through each pixel in the old and new image */
    for (int i = 0; i < oldimg->width * oldimg->height; i++)
    {
        /* Gets the red pixels from the old and new image */
        struct Pixel oldpixel = oldimg->pixels[i];
        struct Pixel newpixel = newimg->pixels[i];
        int old_red = oldpixel.red;
        int new_red = newpixel.red;

        /* Checks if the least significant bit of the oldred and newred are different */
        if ((old_red & 1) != (new_red & 1))
        {
            /* Extract 0-7 bits of the newred value using bitwise AND to get ASCII code of character */
            char ch = (new_red & 7);

            /* Append character to final text */
            text[text_index++] = ch;

            /* Check if the text is complete */
            if (text_index >= text_len)
            {
                break;
            }
        }
    }

    /* Null terminator */
    text[text_index] = '\0';

    return text;
}

/* Question 3 */
int main(int argc, char *argv[])
{
    /* Initialise the random number generator, using the time as the seed */
    srand(time(NULL));

    /* Parse command-line arguments */
    if (argc == 3 && strcmp(argv[1], "t") == 0)
    {
        /* Mode "t" - test PPM reading and writing */

        struct PPM *img = readPPM(argv[2]);
        showPPM(img);
    }
    else if (argc == 3 && strcmp(argv[1], "e") == 0)
    {
        /* Mode "e" - encode PPM */

        struct PPM *oldimg = readPPM(argv[2]);

        /* prompt for a message from the user, and read it into a string */
        char text[1024];

        fprintf(stderr, "Enter message to encode: ");
        if (scanf("%1023s", text) != 1)
        {
            printf("Failed to read message.\n");
            return 1;
        }

        /* encode the text into the image with encode, and assign to newimg */
        struct PPM *newimg = encode(text, oldimg);

        /* write the image to stdout with showPPM */
        showPPM(newimg);
    }
    else if (argc == 4 && strcmp(argv[1], "d") == 0)
    {
        /* Mode "d" - decode PPM */

        /* get original file filename from argv, load it with
           readPPM, then assign to oldimg */
        struct PPM *oldimg = readPPM(argv[2]);

        /* get encoded file filename from argv, load it with
           readPPM, then assign to newimg */
        struct PPM *newimg = readPPM(argv[3]);

        /* decode the encodedPPM with the comparisonPPM and assign to message */
        char *message = decode(oldimg, newimg);
        /* print the decoded message to stdout */
        if (message != NULL)
        {
            printf("Decoded message: %s\n", message);
            free(message);
            return 0;
        }
    }
    else
    {
        fprintf(stderr, "Unrecognised or incomplete command line.\n");
        return 1;
    }

    return 0;
}
