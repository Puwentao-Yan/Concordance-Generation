#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define WORDMAX 25
#define LINEMAX 100
#define MAX_LINE_LEN 85
#define MAX_KEY_NUM 500

char arrin[LINEMAX][MAX_LINE_LEN];
char arrexcp[LINEMAX][WORDMAX];
char arrkey[MAX_KEY_NUM][WORDMAX];

int key_words = 0;  /* stores number of keywords */
int max_key_len = 0;

void print_res(void);
void initialize(void);
void convert(char *);
void getkey(char *);
void cpfile(FILE *, FILE *);
int excpcheck(char *);
int duplicate(char *);
int strcmp_wrapper(const void *, const void *);

int main(int argc, char *argv[]) {
    unsigned int cmp = 1;
    char buffer[WORDMAX];
    char arrkey2[WORDMAX];

    initialize();

    cmp = strncmp(argv[1], "-e", 2);
    if(cmp == 0) {
        FILE *excfile = fopen(argv[2], "r");
        FILE *infile = fopen(argv[3], "r");
        cpfile(excfile, infile);
        fclose(excfile);
        fclose(infile);
        qsort(arrkey, key_words, WORDMAX * sizeof(char), strcmp_wrapper);
        print_res(); 
        return 0;
    }
    else{
        int size = 1;
        FILE *infile = fopen(argv[1], "r");
        fgets(buffer, sizeof(char) * MAX_LINE_LEN, infile);
        fseek(infile, 0, SEEK_END);
        size = ftell(infile);
        fclose(infile);
        if(size != 0){
            buffer[strlen(buffer)-1] = '\0';
            strncpy(arrkey2, buffer, WORDMAX);
            convert(buffer);
            printf("%s", buffer);
            printf("%*c", 2, ' ');
            printf("%s (%d)\n", arrkey2, 1);
            return 0;
        }
        else{
            return 0;
        } 
    }
     
}

void initialize(void){
    for(int i = 0; i < LINEMAX; i++) {  /* initialize array */
        arrin[i][0] = '\0';
        arrexcp[i][0] = '\0';
    }
    for(int i = 0; i < MAX_KEY_NUM; i++) {  /* initialize array */
        arrkey[i][0] = '\0';
    }
}

void cpfile(FILE *excfile, FILE *infile) {
    char buffer1[WORDMAX];
    char buffer2[MAX_LINE_LEN];
    int i = 0;
    while(fgets(buffer1, sizeof(char) * WORDMAX, excfile)) {
        buffer1[strlen(buffer1)-1] = '\0';
        strncpy(arrexcp[i], buffer1, WORDMAX);
        i++;
    }
    i = 0;
    while(fgets(buffer2, sizeof(char) * MAX_LINE_LEN, infile)) {
        if((strlen(buffer2)) > 1){
            buffer2[strlen(buffer2)-1] = '\0';
            strncpy(arrin[i], buffer2, MAX_LINE_LEN);
            getkey(buffer2);
            i++;
        }
        else{
            strncpy(arrin[i], buffer2, MAX_LINE_LEN);
            getkey(buffer2);
            i++;
        }
    }   
}

void getkey(char *line){
    char *token;

    token = strtok(line, " ");
    while(token) {
        if((excpcheck(token)) == (duplicate(token)) == 1){
            if((int)(strlen(token)) > max_key_len){
                max_key_len = (int)strlen(token);
            }
            strncpy(arrkey[key_words], token, WORDMAX);
            key_words++;
        }
        token = strtok(NULL, " ");
    }

}


int excpcheck(char *token){  /* return 0 if token is excep. return 1 if token is keyword. */
    int i = 0;
    while(arrexcp[i][0]) {
        if((strncmp(arrexcp[i], token, WORDMAX)) == 0){  /* if(0==0), then token is excep. */
            return 0;
        }
        i++;
    }
    return 1;
}

int duplicate(char *token){
    int i = 0;
    while(arrkey[i][0]){
        if((strncmp(arrkey[i], token, WORDMAX)) == 0){  /* if(0==0), then token is duplicate. */
            return 0;
        }
        i++;
    }
    return 1;
}

int strcmp_wrapper(const void *a, const void *b) {
    char *sa = (char *)a;
    char *sb = (char *)b;

    return(strcmp(sa, sb));
}


void print_res(void){
    int i = 0;
    int j = 0;
    int counter = 0;

    char *token;
    char upper[WORDMAX];
    char buffer[MAX_LINE_LEN];

    while(arrkey[j][0]){
        i = 0;
        while(arrin[i][0]){
            counter = 0;
            strncpy(buffer, arrin[i], MAX_LINE_LEN); /* comparing line i */
            token = strtok(buffer, " ");
            while(token) {
                if((strncmp(arrkey[j], token, WORDMAX)) == 0){
                    counter++;
                }
                token = strtok(NULL, " ");
            }
            strncpy(upper, arrkey[j], WORDMAX);
            if((strlen(upper)) > 1){
                convert(upper);
                if(counter == 1){  /* counter == 1, one key in a line */
                    printf("%s", upper);
                    printf("%*c", (int)(max_key_len + 2 - strlen(arrkey[j])), ' ');
                    printf("%s (%d)\n", arrin[i], i+1);
                }
                else if(counter > 1){  /* counter > 1, more than one same key in a line */
                    printf("%s", upper);
                    printf("%*c", (int)(max_key_len + 2 - strlen(arrkey[j])), ' ');
                    printf("%s (%d%c)\n", arrin[i], i+1, '*');
                }
            }   
            i++;
        }
        j++;
    }
}


void convert(char *upper){
    int i = 0;
    while(upper[i] != '\0'){
        if (upper[i] >= 97 && upper[i] <= 122){
            upper[i] = upper[i] - 32;
        }
        i++;
    }

}
