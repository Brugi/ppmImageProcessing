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
struct colours{
    int Red, Green, Blue;
}colours;


//structure to hold a PPM image
struct PPM{
    char magicNb[2];
    struct LinkedList * commentValue;
    int width;
    int height;
    int max;
    struct colours * matrix;
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
	struct colours * arr;
    
	//getting the PPM format P3
    fscanf(fd,"%[^\n]", image->magicNb );

    ch = getc(fd); //to skip the new line after the PPM format
	//checking PPM format
    if(strcmp(image->magicNb,"P3") == 0){
	
		
		ch = getc(fd);
		
		while(ch == '#'){
			com = malloc(70);
			ungetc(ch, fd);
			//reads whole line of comments and assigns it to com which is later stored as a node in the LinkedList struct
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



		//go back to the previous read char as it read the first digit of width.
		ungetc(ch, fd);
		
		
		//getting width and height.
		fscanf(fd, "%d %d", &width, &height);
		image->width=width;
		image->height=height;

		//getting the max value
		fscanf(fd, "%d", &max);
		image->max = max;

		//getting the pixels and storing its 3 bits into one colours struct which is then added to an array
		arr = (struct colours *) malloc(width * height * sizeof(struct colours));
		for(counter=0; counter < image->width * image->height; counter++){
			fscanf(fd, "%d %d %d", &arr[counter].Red, &arr[counter].Green, &arr[counter].Blue);
			
			if(arr[counter].Red > image->max || arr[counter].Green > image->max || arr[counter].Blue > image->max){
				fprintf(stderr, "Colour higher than max colour value: %d ", image->max);
				exit(0);
			}
		}
		image->matrix = arr;
		
		//returning the ppm image
		return image;
	
	}else{
        fprintf(stderr, "PPM is not of the right format\n"); 
        exit(1);
    }
   


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

	srand(time(NULL)); //making a random variable.
    int word = 0;
    int count = 0;
    int Green;
	
	
    while(word < strlen(text)){
		
		if(text[word] > i->max){
			fprintf(stderr, "Message cannot be encoded because letter value(%d) is bigger than max(%d)\n", text[word], i->max); 
			exit(1);
		}
		
		//if the character has the same value as the red bit then, the red bit is not changed 
		// and 1 is added to the green bit of that pixel so that while decoding the program will
		//know that that specific red bit is a char.
        if( (int) text[word] == i->matrix[count].Red){
            Green = i->matrix[count].Green;
            i->matrix[count].Green = Green -5;
        
		//otherwise the red bit is replaced by the value of the character
		}else{
            i->matrix[count].Red = (int) text[word];
        
		}
        word++;
		
		//every time a character is encoded a random number is generated to place the next character in the ppm
		//this random number is moduloed by the size of the ppm divided by the length of the string-1 so that
		// the whole string can fit into the ppm and the random number will not be too big.
        count += rand()%((i->width*i->height)/(strlen(text)-1));
    }

    return i;
}

char * decode(struct PPM * i1, struct PPM * i2){
	
	char * secret;
	secret = (char *) malloc((i1->height*i2->width)*sizeof(char));
	int n = 0;
	int i = 0;
	
	//goes through the whole ppm, if the green and red bits of i1 and i2 are the same then they are skipped 
	// otherwise they are added to the secret message.
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


	//checks if the number of arguments is correct
	if(argc != 4 && argc != 3){
		
		fprintf(stderr,"Not enough arguments\n");
		exit(1);
	
	}
	
	//if e is detected then it does the encode operation
    if (strcmp(argv[1], "e") == 0){
		
		if( argc != 3){
		
		fprintf(stderr,"Not enough arguments\n");
		exit(1);
	
		}
	
		fprintf(stderr, "Please enter a message:\n"); 
        fin = fopen(argv[2], "r");
		
		//checks if file is not empty
		if (fin == NULL){
			fprintf(stderr, "Can't open file %s\n", argv[2]);
			exit(0);
		}
		
        toEncode = getPPM(fin);
		msg = (char *) malloc(toEncode->width*toEncode->height*sizeof(char));
		fgets(msg,toEncode->width*toEncode->height,stdin);
        encoded = encode(msg, toEncode);
        showPPM(encoded);
		fclose(fin);
	
	//if d is detected then it proceeds with the decode operation
    }else if(strcmp(argv[1], "d") == 0){
		
		if( argc != 4){
		
		fprintf(stderr,"Not enough arguments\n");
		exit(1);
	
		}
		
		origin = fopen(argv[2], "r");
		
		//checks if file is not empty
		if (origin == NULL){
			fprintf(stderr, "Can't open file %s\n", argv[2]);
			exit(0);
		}
		
		original = getPPM(origin);
		enc = fopen(argv[3], "r");
		
		//checks if file is not empty
		if (enc == NULL){
			fprintf(stderr, "Can't open file %s\n", argv[3]);
			exit(0);
		}
		
		secret = getPPM(enc);
		printf( "The secret message is:\n%s\n", decode(original, secret));
		fclose(enc);
		fclose(origin);

	//else it throws an error  
    }else{
       
		fprintf(stderr,"Not a valid call\n");
		exit(0);
    }

}