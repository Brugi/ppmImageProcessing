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
    arr = (colours *) malloc(width * height * sizeof(colours));
    for(counter=0; counter < image->width * image->height; counter++){
        
        fscanf(fd, "%d %d %d", &arr[counter].Red, &arr[counter].Green, &arr[counter].Blue);
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



    srand(time(NULL));
    int word = 0;
    int count = 0;
    int Green;
    while(word < strlen(text)){
        if( (int) text[word] == i->matrix[count].Red){
            Green = i->matrix[count].Green;
            i->matrix[count].Green = Green +1;
        }else{
            i->matrix[count].Red = (int) text[word];
        }
        word++;
        count += rand()%((i->width*i->height)/(strlen(text)-1));

    }

    return i;
}

char * decode(struct PPM * i1, struct PPM * i2){

	char * secret;
		secret = (char *) malloc((i1->height*i2->width)*sizeof(char));
		int n = 0;
		int i = 0;
		while(n <= (i1->height*i1->width)){

			if(i1->matrix[n].Red == i2->matrix[n].Red && i1->matrix[n].Green == i2->matrix[n].Green){
				n ++;
			}else{
				secret[i] = (char) (i2->matrix[n].Red);
				i++;
				n++;
			}


		}
	secret[n-1] = '\0';


    return secret;
}


int main(int argc,char ** argv){
    FILE * fin;
    FILE * enc;
    FILE * origin;
    struct PPM * toEncode;
    struct PPM * encoded;
    struct PPM * original;
    struct PPM * secret;
    char * msg;
	
	if(argc != 4 && argc != 3){
		printf("Not enough arguments\n");
		exit(0);
	}
	
    if (strcmp(argv[1], "e") == 0){

		
		fprintf(stderr, "Please enter a message:\n"); 
        fin = fopen(argv[2], "r");
        toEncode = getPPM(fin);
		msg = (char *) malloc(1000*sizeof(char));

		fgets(msg,100,stdin);
        encoded = encode(msg, toEncode);
        showPPM(encoded);
		fclose(fin);

    }else if(strcmp(argv[1], "d") == 0){
        origin = fopen(argv[2], "r");
        original = getPPM(origin);
        enc = fopen(argv[3], "r");
        secret = getPPM(enc);
        printf( "The secret message is: %s\n", decode(original, secret));
        fclose(enc);
        fclose(origin);

    }else{
        printf("Not a valid call");
		exit(0);
    }

}