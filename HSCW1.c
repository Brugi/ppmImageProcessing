#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//linked list to hold comments
struct LinkedList{
    char* comment;
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
    char com[100]; //string to store the comments which then gets passed as a value into the linked list
    char ch; //char used for getting the next character
	struct LinkedList* head;
    head = (struct LinkedList *)malloc(sizeof(struct LinkedList));
    head->comment = NULL;
    struct LinkedList *currentNode;
    currentNode = (struct LinkedList *)malloc(sizeof(struct LinkedList));
	int counter; //counter for the array in which the pixels get stored in
    int magicNb, width, height, max, r, g, b;

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
        fscanf(fd,"%[^\n]", com );
        if(head->comment == NULL){
            head->comment = com;
            head->next = NULL;
            currentNode = head;
           // printf("#%s\n", currentNode->comment);

        }else{
            while (currentNode->next != NULL){
                currentNode = currentNode->next;
            }

            currentNode->next = (struct LinkedList *)malloc(sizeof(struct LinkedList));
            currentNode = currentNode->next;
            image->commentValue = head;
            currentNode->comment = malloc(sizeof(com));
            strcpy(currentNode->comment, com);
            currentNode->next = NULL;
           // printf("# %s\n", currentNode->comment);
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
    image->matrix = (colours *) malloc(image->width * image->height * sizeof(colours));
    for(counter=0; counter < image->width * image->height; counter++){
        r= fscanf(fd, "%d", &image->matrix[counter].Red);
		r= fscanf(fd, "%d", &image->matrix[counter].Green);
		r = fscanf(fd, "%d", &image->matrix[counter].Blue);
		//printf("%d %d %d \n", image->matrix[counter].Red, image->matrix[counter].Green, image->matrix[counter].Blue);

    }

	//returning the ppm image
    return image;


}

void showPPM(struct PPM * i){
	//printing the ppm format 
	printf("P3\n");
    
	struct LinkedList *currentNode = i->commentValue;
	int n;
	//printing ppm format from ppm structure but it doesn't work
	printf("%s\n", i->magicNb);

	//printing the comments but doesn't return anything
	while(currentNode != NULL){
	  printf("# %s\n", currentNode->comment);
	  currentNode = currentNode->next;
	}
  
	//printing width, height and max that is working 
    printf("%d %d\n", i->width, i->height);
    printf("%d\n", i->max);
	
	
	//printing the bits of every pixel returns only 0s
    for(n=0; n < i->width * i->height; n++){
		printf("%d %d %d\n ", i->matrix[n].Red,  i->matrix[n].Green,  i->matrix[n].Blue );
	}
	


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