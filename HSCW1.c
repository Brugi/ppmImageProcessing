#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct LinkedList{
    char* comment;
    struct LinkedList * next;
};




//structure to hold a PPM image
struct PPM{
    char magicNb[2];
    struct LinkedList *commentValue;
    int width;
    int height;
    int max;
    int *matrix;
};

struct PPM * getPPM(FILE * fd){

    struct PPM *image;
    image = (struct PPM *) malloc (sizeof(struct PPM));
    char com[100];
    char ch;

    struct LinkedList* head;
    head = (struct LinkedList *)malloc(sizeof(struct LinkedList));
    head->comment = NULL;
    struct LinkedList *currentNode;
    currentNode = (struct LinkedList *)malloc(sizeof(struct LinkedList));
    int counter;
    int magicNb, width, height, max;

    fscanf(fd,"%[^\n]", &(image->magicNb) );
	
    ch = getc(fd); //to skip the new line after the magic number
    if(strcmp((image->magicNb),"P3\n") !=0){

    }else{
        printf("Wrong PPM format");
		exit(0);
    }

    //getting the comments and storing them as nodes in a linked list.
    ch = getc(fd);


    while(ch == '#'){
        fscanf(fd,"%[^\n]", com );
        if(head->comment == NULL){
            head->comment = com;
            head->next = NULL;
            currentNode = head;
            printf("#%s\n", currentNode->comment);

        }else{
            while (currentNode->next != NULL){
                currentNode = currentNode->next;
            }

            currentNode->next = (struct LinkedList *)malloc(sizeof(struct LinkedList));
            currentNode = currentNode->next;
            //image->commentValue = head;
            currentNode->comment = malloc(sizeof(com));
            strcpy(currentNode->comment, com);
            currentNode->next = NULL;
            printf("# %s\n", currentNode->comment);
        }

        //add(head, com); // adding a comment at the end of the linked list. THIS produces the segmaentation fault.
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

    image->matrix = malloc(3*(image->width*image->height)*sizeof(int));
    for(counter=0; counter<3*(image->width*image->height); counter++){
        fscanf(fd, "%d", &(image->matrix[counter]));
        printf("%d\n", image->matrix[counter]);

    }

    return image;


}

void showPPM(struct PPM * i){
	printf("P3\n");
    
      struct LinkedList *currentNode = i->commentValue;
      int n;
      printf("%d\n", i->magicNb);

      
      while(currentNode != NULL){
          printf("# %s\n", currentNode->comment);
          currentNode = currentNode->next;
      }
  /*
    printf("%d %d\n", i->width, i->height);
    printf("%d", i->max);
    for(n=0;n<3*(i->width*i->height);i++){
		printf("%d", i->matrix);
	}
	
*/

}

struct PPM * encode(char * text, struct PPM * i){

}

char * decode(struct PPM * i1, struct PPM * i2){

}


int main(int argc,char ** argv){

    FILE * fd;

    fd = fopen(argv[1],"r");

    getPPM(fd);
    showPPM(getPPM(fd));
    fclose(fd);

}