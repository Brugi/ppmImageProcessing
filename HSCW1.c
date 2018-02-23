#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

//linked list to hold comments
struct LinkedList{
    char * value;
    struct LinkedList * next;
};

//pixel structure
typedef struct colours{
    int Red, Green, Blue;
}colours;


//structure to hold a PPM image
struct PPM{
    char magicNb[2];
    struct LinkedList * commentValue;
    int width;
    int height;
    int max;
    colours * matrix;
};

struct PPM * getPPM(FILE * fd){

    struct PPM *image;
    image = (struct PPM *) malloc (sizeof(struct PPM));
    char *com; //pointer to store the comments which then gets passed as a value into the linked list
    int ch; //char used for getting the next character
    struct LinkedList* head = NULL;
    struct LinkedList *currentNode = NULL;
    int counter; //counter for the array in which the pixels get stored in
    int width, height, max;

    colours * arr;
    //getting the PPM format P3
    fscanf(fd,"%[^\n]", image->magicNb );

    ch = getc(fd); //to skip the new line after the PPM format
    if(strcmp((image->magicNb),"P3\n") !=0){

    }else{
        printf("Wrong PPM format");
        exit(0);
    }
    //getting the comments and storing them as nodes in a linked list.
    ch = getc(fd);
    while(ch == '#'){
        com = malloc(70);
        ungetc(ch, fd);
        fscanf(fd,"%[^\n]", com );

        if(head == NULL){
            head = (struct LinkedList *)malloc(sizeof(struct LinkedList));
            head->value = com;
            head->next = NULL;
            image->commentValue = head;
            currentNode = head;
            //printf("%s\n", currentNode->comment);

        }else{
            while (currentNode->next != NULL){
                currentNode = currentNode->next;
            }

            currentNode->next = (struct LinkedList *)malloc(sizeof(struct LinkedList));
            currentNode = currentNode->next;
            currentNode->value = malloc(sizeof(com));
            strcpy(currentNode->value, com);
            currentNode->value = com;
            currentNode->next = NULL;
            //printf("%s\n", currentNode->comment);
        }


        ch = getc(fd); //to skip the new line char.
        ch = getc(fd); //to read the first char of the next line.

    }



    //getting width and height.
    //go back to the previous read char as it read the first digit of width.
    ungetc(ch, fd);
    fscanf(fd, "%d %d", &width, &height);
    image->width=width;
    image->height=height;

    //getting the max value
    fscanf(fd, "%d", &max);
    image->max = max;

    //getting the pixels storing 3 bits into one colour struct which is then added to an array
   // image->matrix = (colours *) malloc(image->width * image->height * sizeof(colours));

    arr = (colours *) malloc(width * height * sizeof(colours));
    for(counter=0; counter < image->width * image->height; counter++){
        /*
        fscanf(fd, "%d", &image->matrix[counter].Red);
        fscanf(fd, "%d", &image->matrix[counter].Green);
        fscanf(fd, "%d", &image->matrix[counter].Blue);
         */
        fscanf(fd, "%d %d %d", &arr[counter].Red, &arr[counter].Green, &arr[counter].Blue);
       // fscanf(fd, "%d %d %d", &image->matrix[counter].Red, &image->matrix[counter].Green, &image->matrix[counter].Blue);
        //printf("%d %d %d\n", image->matrix[counter].Red, image->matrix[counter].Green, image->matrix[counter].Blue);
    }
    image->matrix = arr;


    //returning the ppm image

    return image;


}

void showPPM(struct PPM * i){
    struct LinkedList *currentNode = i->commentValue;
    int counter;

    //printing ppm format from ppm structure
    printf("%s\n", i->magicNb);

    //printing the comments
    while(currentNode != NULL){

        printf("%s\n", currentNode->value);
        currentNode = currentNode->next;
    }

    //printing width, height and max
    printf("%d %d\n", i->width, i->height);
    printf("%d\n", i->max);

    //printing the bits of every pixel
    for(counter=0; counter < i->width * i->height; counter++){
        printf("%d %d %d\n", i->matrix[counter].Red, i->matrix[counter].Green, i->matrix[counter].Blue);

    }



}

struct PPM * encode(char * text, struct PPM * i){


    int count = 1;
    int word = 0;
    int Red;
    int character;
    int letter;
    //printf("%d", strlen(text));

    i->matrix[0].Red = strlen(text);
    while(word < strlen(text)){
        character = (int)text[word];
        Red = (i->matrix[count].Red/3);

        letter =  character + Red;

        i->matrix[count].Red= letter;

        count++;
        word++;

    }

    return i;
}

char * decode(struct PPM * i1, struct PPM * i2){
    int strLength = i2->matrix[0].Red;
    int n = 1;
    int i = 0;
    char * secret;
    secret = (char *) malloc(strLength*sizeof(char));

    while(n <= strLength ){
        //printf("%d\n", strLength);

        secret[i] = (char)  ((i2->matrix[n].Red)- (i1->matrix[n].Red/3));

        n++;
        i++;
    }
    secret[n-1] = '\0';

    return secret;
}


int main(int argc,char ** argv){

    FILE * fd;
    FILE * test;
    FILE * fx;
    char * msg = "abcdefghijklmnopqrstuvwxyz";
    struct PPM * image;
    struct PPM * image2;
    fd = fopen("C:/Users/Mark/Documents/test.ppm","r");
    test = fopen("C:/Users/Mark/Documents/enc.ppm","r");
    fx = fopen("C:/Users/Mark/Documents/write.ppm","w+");
    image = getPPM(fd);
    image2 = getPPM(test);
    //image = (struct PPM *) malloc (sizeof(struct PPM));



    if (strcmp(argv[1], 'e') == 1){

    }

        //printf("Enter a message:");
    //scanf("%[^\n]", msg);

    // printf("meesfage is:");
    //printf("%s\n", msg);
    //fd = fopen(argv[1],"r");

    //getPPM(fd);
  // showPPM(image);
   // printf("----------------\n");
    //image2 = encode(msg, image);
   //showPPM(encode(msg,image));
   fprintf(fx ,encode(msg,image));
    //decode(image, encode(msg, image));
  printf("Secret msg:%s",decode(image, image2));
    fclose(fd);
    fclose(fx);
    fclose(test);

}