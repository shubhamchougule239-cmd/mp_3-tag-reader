#include<stdio.h>
#include<string.h>
#include<stdlib.h>

/* Colors for better terminal output */
#define GREEN "\033[1;32m"
#define CYAN  "\033[1;36m"
#define YELLOW "\033[1;33m"
#define RESET "\033[0m"

/* ----------------------------------------------------
   help_mode()
   This function prints instructions for the user.
   It tells how to run the program in view mode and
   edit mode with different options.
---------------------------------------------------- */
void help_mode()
{
    printf(CYAN "-----------------------------------------------------------------------------\n" RESET);

    /* Showing that help section is started */
    printf(GREEN "HELP MODE STARTED\n" RESET);

    /* Instructions for viewing MP3 metadata */
    printf(YELLOW "TO View the details:\n" RESET);
    printf("./a.out -v filename.mp3\n");

    /* Instructions for editing MP3 metadata */
    printf(YELLOW "\nTo edit the details:\n" RESET);
    printf("USAGE :\n\t ./a.out -e -t/-a/-A/-y/-m/-c 'new name' filename.mp3\n");

    /* Explanation of each edit option */
    printf("-t\t : MODIFY a title\n");     
    printf("-a\t : MODIFY an Artist name\n");     
    printf("-A\t : MODIFY an Album name\n");     
    printf("-y\t : MODIFY a Year\n"); 
    printf("-m\t : MODIFY a Content\n");  
    printf("-c\t : MODIFY a comment\n");     

    printf(CYAN "-----------------------------------------------------------------------------\n" RESET);
    
    return;
}