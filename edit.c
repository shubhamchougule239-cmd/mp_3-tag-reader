#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ----------- Terminal Color Macros ----------- */
#define RED   "\033[1;31m"
#define GREEN "\033[1;32m"
#define YELLOW "\033[1;33m"
#define CYAN "\033[1;36m"
#define RESET "\033[0m"

/* Function to reverse byte order (Big Endian <-> Little Endian) */
void reverse_bytes_unsigned(unsigned int *num);

void reverse_bytes_unsigned(unsigned int *num)
{
    /* Treat integer as array of bytes */
    unsigned char *pos = (unsigned char*) num;

    /* Swap bytes from start and end */
    for (int i = 0; i < sizeof(unsigned int) / 2; i++)
    {
        pos[i] ^= pos[sizeof(unsigned int) - i - 1];
        pos[sizeof(unsigned int) - i - 1] ^= pos[i];
        pos[i] ^= pos[sizeof(unsigned int) - i - 1];
    }
}


/* Function to edit MP3 metadata frame */
void edit_mp3_details(const char *tar_tag, const char *new_name, const char *input)
{
    /* Open original MP3 file in binary read mode */
    FILE *ip = fopen(input, "rb");
    if (!ip)
    {
        printf(RED "Cannot open the input file\n" RESET);
        return;
    }

    /* Create temporary file to store modified data */
    FILE *temp = fopen("temp_file.mp3", "wb");
    if (temp == NULL)
    {
        printf(RED "Cannot create temporary file\n" RESET);
        fclose(ip);
        return;
    }

    /* Read first 10 bytes (ID3 header) */
    unsigned char header[10];
    if (fread(header, 1, 10, ip) != 10)
    {
        printf(RED "Failed in reading header\n" RESET);
        fclose(ip);
        fclose(temp);
        return;
    }

    /* Write header to temporary file */
    fwrite(header, 1, 10, temp);

    int tag_found = 0;

    /* Read frames one by one */
    while (1)
    {
        unsigned char tag[4], flags[2];
        unsigned int size;

        /* Read frame tag */
        if (fread(tag, 1, 4, ip) != 4) break;

        /* Read frame size */
        if (fread(&size, 4, 1, ip) != 1) break;

        /* Read frame flags */
        if (fread(flags, 1, 2, ip) != 2) break;

        /* Convert size from big-endian to little-endian */
        reverse_bytes_unsigned(&size);

        /* Stop if frame size is zero */
        if (size == 0 || tag[0] == 0)
        {
            break;
        }

        /* Allocate memory for frame data */
        unsigned char *info = malloc(size);
        if (info == NULL)
        {
            printf(RED "Memory allocation failed\n" RESET);
            break;
        }

        /* Read frame content */
        if (fread(info, 1, size, ip) != size)
        {
            free(info);
            break;
        }

        /* Check if this is the tag we want to edit */
        if (strncmp((char *)tag, tar_tag, 4) == 0)
        {
            unsigned int be_size = size;

            /* Convert back to big-endian before writing */
            reverse_bytes_unsigned(&be_size);

            /* Write tag information */
            fwrite(tag, 1, 4, temp);
            fwrite(&be_size, 1, 4, temp);
            fwrite(flags, 1, 2, temp);

            /* Write encoding byte */
            unsigned char encoding = 0x00;
            fwrite(&encoding, 1, 1, temp);

            /* Length of new metadata */
            size_t newlen = strlen(new_name);

            /* Truncate if new string is longer than frame */
            if (newlen > size - 1)
            {
                newlen = size - 1;
            }

            /* Write new metadata */
            fwrite(new_name, 1, newlen, temp);

            /* Fill remaining frame with zero padding */
            for (unsigned int i = newlen + 1; i < size; i++)
                fputc(0, temp);

            tag_found = 1;

            printf(CYAN "Edited tag: %s → %s\n" RESET, tar_tag, new_name);
        }
        else
        {
            /* Copy frame as it is */
            unsigned int be_size = size;
            reverse_bytes_unsigned(&be_size);

            fwrite(tag, 1, 4, temp);
            fwrite(&be_size, 1, 4, temp);
            fwrite(flags, 1, 2, temp);
            fwrite(info, 1, size, temp);
        }

        /* Free allocated memory */
        free(info);
    }

    /* Copy remaining MP3 audio data */
    unsigned char buffer[4096];
    size_t bytes;

    while ((bytes = fread(buffer, 1, sizeof(buffer), ip)) > 0)
        fwrite(buffer, 1, bytes, temp);

    /* Close files */
    fclose(ip);
    fclose(temp);

    /* Replace original file with modified file */
    remove(input);
    rename("temp_file.mp3", input);

    printf("\n" YELLOW "..........................................................\n" RESET);

    if (tag_found)
        printf(GREEN "Your metadata has been updated successfully!\n" RESET);
    else
        printf(RED "Tag %s not found.\n" RESET, tar_tag);

    printf(YELLOW "..........................................................\n" RESET);
}