#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SIZE 50

//typedef FILE FILESTREAM

int main()
{
    FILE* fp;

    //fopen("myfile.txt"); // returns address
    //fopen("C: \a myfile.txt");
    //fopen("myfile", "w"); // "a" - append(), "w" - write, "r" - read, "w+" - write and read
    
    // for binary "rb", "wb"

    /*char str[50];
    char c;
    fp = fopen("myfile", "w");
     
    if(NULL == fp)
    {
        perror("Problem to open file"); // always adds ": No such file or directory"
        exit(1); // return of main
    }

    fprintf(fp, "Hello world from file");

    if(NULL == fgets(str, 50, fp))
    {
        perror("Reading error");
        exit(2);
    }

    while(EOF != (c = fgetc(fp)))
    {

    }
    fprintf(stdout, "%s", str);

    fopen("myfile", "r");*/

    //-----------------------------------------------------
    
    /*char str[SIZE];

    fp = fopen("myfile.txt", "w");

    if(NULL == fp)
    {
        perror("Problem 1");
        exit(1);
    }

    fprintf(fp, "Hello world\n");
    fprintf(fp, "Hello world\n");
    fprintf(fp, "Hello world\n");
    fprintf(fp, "Hello world\n");
   
    fclose(fp);

    fp = fopen("myfile", "r");
    
    int i = 0;

    while(NULL != fgets(str, SIZE, fp))
    {
        while (str[i] != '\0')
        {
            if(str[i] >= 'a' && str[i] <= 'z')
            {
                str[i] -= 32;
            }
            i++;
        } 
        i = 0;
        printf("%s", str);
    }

  
    fclose(fp);*/

    //-------------------------------------------------------

    int number;
    char ch;
    float f;

    printf("Enter num, ch, db : ");
    scanf("%d %c %f", &number, &ch, &f);

    fp = fopen("file.bin", "wb");

    if(NULL == fopen("file.bin", "wb"))
    {
        perror("Problem opening");
        exit(1);
    }

    if(1 != fwrite(&number, sizeof(number), 1, fp))
    {
        perror("Problem 3");
        exit(3);
    }
    
    if(1 != fwrite(&f, sizeof(f), 1, fp))
    {
        perror("Problem 3");
        exit(3);
    }
    
    if(1 != fwrite(&ch, sizeof(ch), 1, fp))
    {
        perror("Problem 3");
        exit(3);
    }


    fclose(fp);

    fp = fopen("file.bin", "rb"); 
    
    fseek(fp, 0, SEEK_SET);

    if(1 != fread(&number, sizeof(number), 1, fp))
    {
        perror("Problem 3");
        exit(3);
    }
    
    if(1 != fread(&f, sizeof(f), 1, fp))
    {
        perror("Problem 3");
        exit(3);
    }
    
    if(1 != fread(&ch, sizeof(ch), 1, fp))
    {
        perror("Problem 3");
        exit(3);
    }

    fclose(fp);
    return 0;
}