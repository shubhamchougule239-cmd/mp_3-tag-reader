#include<stdio.h>
#include<string.h>
#include<stdlib.h>

/* Colors for terminal messages */
#define RED   "\033[1;31m"
#define GREEN "\033[1;32m"
#define YELLOW "\033[1;33m"
#define CYAN "\033[1;36m"
#define RESET "\033[0m"

/* Function declarations */
void display_mp3();
void reverse(unsigned int *size);
void view_mode(const char *filename);

/* Global file pointer so it can be used in multiple functions */
FILE *fp;


/* ----------------------------------------------------
   view_mode()
   This function starts the view process.
   It opens the mp3 file and checks its ID3 information.
---------------------------------------------------- */
void view_mode(const char *filename)
{
    printf(CYAN "VIEW MODE STARTED...\n" RESET);
    
    /* Open the mp3 file in binary mode */
    fp = fopen(filename, "rb");

    if (fp == NULL)
    {
        printf(RED "Error opening file\n" RESET);
        return;
    }
    else
    {
        printf(GREEN "File opened successfully...\n" RESET);
    }

    /* Reading first 3 bytes to check ID3 tag */
    char tag[4];                                       
    fread(tag,1,3,fp);
    tag[3] = '\0';

    printf(YELLOW "MP3 Tag is %s\n" RESET, tag);

    /* Reading version of the ID3 tag */
    unsigned char version[2];
    fread(version,1,2,fp);

    printf(YELLOW "ID3 tag version is 2.%d.%d\n" RESET, version[0], version[1]);

    /* Move file pointer to start of metadata frames */
    fseek(fp,0,SEEK_SET);
    fseek(fp,10,SEEK_SET);

    /* Call function to display tag details */
    display_mp3();
}


/* ----------------------------------------------------
   display_mp3()
   This function reads metadata frames one by one
   and prints important details like title, artist etc.
---------------------------------------------------- */
void display_mp3()
{
    printf(CYAN "-------------------------------------------------------------\n" RESET);

    int choice = 0;
    char id[5];
    unsigned int size;

    /* Loop to read first few frames */
    while(choice <= 5)
    {
        /* Read frame id */
        fread(id,1,4,fp);
        id[4] = '\0';

        /* Read frame size */
        fread(&size,4,1,fp);

        /* Convert size format */
        reverse(&size);

        /* Skip 2 bytes of frame flags */
        fseek(fp,2,SEEK_CUR);

        /* Allocate memory to store frame data */
        char *data = malloc(size + 1);

        fread(data,1,size,fp);
        data[size] = '\0';

        /* Printing metadata based on order */
        if(choice == 0)
            printf(GREEN "TITLE   : %s\n" RESET, data+1);

        if(choice == 1)
            printf(GREEN "ARTIST  : %s\n" RESET, data+1);

        if(choice == 2)
            printf(GREEN "ALBUM   : %s\n" RESET, data+1);

        if(choice == 3)
            printf(GREEN "YEAR    : %s\n" RESET, data+1);

        if(choice == 4)
            printf(GREEN "CONTENT : %s\n" RESET, data+1);

        if(choice == 5)
            printf(GREEN "COMMENT : %s\n" RESET, data+1);

        choice++;

        /* Free allocated memory */
        free(data);
    }

    printf(CYAN "-------------------------------------------------------------\n" RESET);
}


/* ----------------------------------------------------
   reverse()
   Frame size in mp3 is stored in different byte order.
   This function swaps the bytes to correct the value.
---------------------------------------------------- */
void reverse(unsigned int *size)
{
    char *pos = (char*)size;

    /* Swap bytes from start and end */
    for(int i = 0; i < sizeof(unsigned int)/2; i++)
    {
        pos[i] ^= pos[(sizeof(unsigned int) - i - 1)];
        pos[(sizeof(unsigned int) - i - 1)] ^= pos[i];
        pos[i] ^= pos[(sizeof(unsigned int) - i - 1)];
    }
}