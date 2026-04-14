/*------------------------------------------------------------------------------------
  Project Title : MP3 Tag Reader and Editor 
  Description   : Entry point of the MP3 Tag Reader and Editor project.
                  This file handles command line arguments and decides
                  whether the program runs in view mode, edit mode, or help mode.

  Name          : Shubham Chougule
  Date          : 20/03/2026
------------------------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>

/* Color Macros for Terminal Output */
#define RED   "\033[1;31m"
#define GREEN "\033[1;32m"
#define YELLOW "\033[1;33m"
#define CYAN "\033[1;36m"
#define RESET "\033[0m"

/* Function declarations */
void view_mode(const char *filename);   
void edit_mp3_details(const char *tar_tag, const char *new_name, const char *input);
void help_mode();


int main(int argc, char *argv[])
{
    /* Check if minimum arguments are provided */
    if (argc < 2)
    {
        printf(RED "ERROR: No arguments provided\n" RESET);
        printf(YELLOW "Use '--help' for usage information\n" RESET);
        return 0;
    }

    /* Help mode : Displays usage instructions */
    if (strcmp(argv[1], "--help") == 0)
    {
        printf(CYAN "\n------ MP3 TAG READER / EDITOR HELP ------\n" RESET);
        help_mode();
        return 0;
    }

    /* View mode : Used to display MP3 metadata */
    else if (strcmp(argv[1], "-v") == 0)
    {
        /* Check correct number of arguments */
        if (argc != 3)
        {
            printf(RED "Usage: ./a.out -v <filename.mp3>\n" RESET);
            return 0;
        }

        /* Check if input file has .mp3 extension */
        if (strstr(argv[2], ".mp3") == NULL)
        {
            printf(RED "ERROR: Input file must have .mp3 extension\n" RESET);
            return 0;
        }

        printf(GREEN "Read and validation successful...\n" RESET);

        /* Call view mode function */
        view_mode(argv[2]);
    }

    /* Edit mode : Used to modify MP3 tag details */
    else if (strcmp(argv[1], "-e") == 0)
    {
        /* Check correct number of arguments */
        if (argc != 5)
        {
            printf(RED "Input commands are not added correctly\n" RESET);
            printf(YELLOW "Usage: ./a.out -e <tag> new_name filename.mp3\n" RESET);
            return 0;
        }

        /* Validate MP3 file extension */
        if (strstr(argv[4], ".mp3") == NULL)
        {
            printf(RED "ERROR: Input file must have .mp3 extension\n" RESET);
            return 0;
        }

        printf(GREEN "Edit mode validation successful...\n" RESET);

        /* Edit Title */
        if (strcmp(argv[2], "-t") == 0)
        {
            printf(CYAN "Editing Title...\n" RESET);
            edit_mp3_details("TIT2", argv[3], argv[4]);
        }

        /* Edit Artist */
        else if (strcmp(argv[2], "-a") == 0)
        {
            printf(CYAN "Editing Artist...\n" RESET);
            edit_mp3_details("TPE1", argv[3], argv[4]);
        }

        /* Edit Album */
        else if (strcmp(argv[2], "-A") == 0)
        {
            printf(CYAN "Editing Album...\n" RESET);
            edit_mp3_details("TALB", argv[3], argv[4]);
        }

        /* Edit Year */
        else if (strcmp(argv[2], "-y") == 0)
        {
            printf(CYAN "Editing Year...\n" RESET);
            edit_mp3_details("TYER", argv[3], argv[4]);
        }

        /* Edit Composer */
        else if (strcmp(argv[2], "-c") == 0)
        {
            printf(CYAN "Editing Composer...\n" RESET);
            edit_mp3_details("TCOM", argv[3], argv[4]);
        }

        /* Edit Genre */
        else if (strcmp(argv[2], "-m") == 0)
        {
            printf(CYAN "Editing Genre...\n" RESET);
            edit_mp3_details("TCON", argv[3], argv[4]);
        }

        /* Invalid tag option */
        else
        {
            printf(RED "ERROR: Invalid tag option\n" RESET);
        }
    }

    /* Invalid input case */
    else
    {
        printf(RED "ERROR : ./a.out -> INVALID ARGUMENTS\n" RESET);
        printf(YELLOW "To view  -> ./a.out -v filename.mp3\n" RESET);
        printf(YELLOW "To edit  -> ./a.out -e -t/-a/-A/-m/-y/-c 'new_name' filename.mp3\n" RESET);
        printf(YELLOW "For help -> ./a.out --help\n" RESET);
    }

    return 0;   
}