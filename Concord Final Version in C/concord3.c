/*
 * concord3.c
 *
 * Starter file provided to students for Assignment #3, SENG 265,
 * Summer 2021
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "emalloc.h"
#include "seng265-list.h"


void print_word(node_t *node, void *arg)
{
    char *format = (char *)arg;
    printf(format, node->text);
}


#ifdef DEBUG

/*
 * Just showing the use of the linked-list routines.
 */

void _demo() {
printf("DEBUG: in _demo\n");
    char *words_german[] = {"Der", "Kater", "mit", "Hut."};
    int   words_german_len = 4;

    char *words_english[] = {"The", "cat", "in", "the", "hat."};
    int   words_english_len = 5;

    node_t *temp_node = NULL;
    node_t *head = NULL;

    int i;

    /* Add the words in German, at the front. */
    for (i = 0; i < words_german_len; i++) {
        temp_node = new_node(words_german[i]);
        head = add_front(head, temp_node);
    }

    /* Add the words in English, at the end. */
    for (i = 0; i < words_english_len; i++) {
        temp_node = new_node(words_english[i]);
        head = add_end(head, temp_node);
    }

    /* Print the list of words. */

    apply(head, print_word, "--> %s\n");

    /* Free up the memory. This is done rather deliberately
     * and manually.  Asserts are liberally used here as they
     * express state that *must* be true if all of the code is
     * correctly working.
     */

    temp_node = head;
    while (temp_node != NULL) {
        assert(temp_node != NULL);
        head = remove_front(head);
        free(temp_node);
        temp_node = head;
    }
   
    assert(head == NULL); 
}

#endif

void free_list(node_t *head){   // obtained from demo().
    node_t *temp_node = NULL;
    temp_node = head;
    while (temp_node != NULL) {
        assert(temp_node != NULL);
        head = remove_front(head);
        free(temp_node);
        temp_node = head;
    }
}

int key_check(char *token, node_t *list){   // return 1 if no duplicate keyword or not in exception list
     while(list != NULL){
        if(strcmp(token, list->text) == 0){
            return 0;
        }
        list = list->next;
     }
     return 1;
}

void lower(char *token){
    int i = 0;
    while(token[i] != '\0'){
        if (token[i] >= 65 && token[i] <= 90){
            token[i] = token[i] + 32;
        }
        i++;
    }
}

void upper(char *token){
    int i = 0;
    while(token[i] != '\0'){
        if (token[i] >= 97 && token[i] <= 122){
            token[i] = token[i] - 32;
        }
        i++;
    }
}

node_t *get_key(node_t *input, node_t *exception, int *max_key_len){
    char line[LIST_MAXLEN_TEXT];
    node_t *temp = NULL;
    node_t *head = NULL;
    
    while (input != NULL){  // check every line of the input file
        strncpy(line, input->text, LIST_MAXLEN_TEXT);
        if (strcmp(line, "\n") == 0){   // skip the empty line
            input = input->next;
            continue;
        }
        char *token;    // token conrains the word in a line.

        token = strtok(line, " ");
        while (token){  // check every word in the line
            lower(token);
            // strcmp != --> it's keyword. We still need to check if keyword is in the key_list.
            if(key_check(token, exception) && key_check(token, head)){
                if (*max_key_len < (int)(strlen(token))){   // obtain max_key_len
                    *max_key_len = (int)(strlen(token));
                }
                temp = new_node(token);
                head = add_inorder(head, temp);
            }
            token = strtok(NULL, " ");
        }
        input = input->next;
    }
    return head;

}

node_t *copy_file(FILE *file){
    char buffer[LIST_MAXLEN_TEXT];
    node_t *temp = NULL;
    node_t *head = NULL;

    while(fgets(buffer, sizeof(char) * LIST_MAXLEN_TEXT, file)){
        if (strcmp(buffer, "\n") != 0){
            buffer[strlen(buffer)-1] = '\0';
        }
        temp = new_node(buffer);
        head = add_end(head, temp);
    }
    return head;
}

void print_out(char key[], char input[], int line_num, int appear_time, int *max_key_len){
    upper(key);
    printf("%s", key);
    printf("%*c", (int)(*max_key_len + 2 - strlen(key)), ' ');
    if (appear_time > 1){
        printf("%s (%d*)\n", input, line_num);
    }
    else {
        printf("%s (%d)\n", input, line_num);
    }
    lower(key);
}


void final_frame(node_t *key, node_t *input, int *max_key_len){
    node_t *temp = NULL;
    temp = input;
    while(key != NULL){ // loop through each keyword
        int line_num = 1;
        temp = input;
        while(temp != NULL){   // loop through each line
            char buffer[LIST_MAXLEN_TEXT];
            char *token;    // token conrains the word in a line.
            int appear_time = 0;
            strncpy(buffer, temp->text, LIST_MAXLEN_TEXT);

            token = strtok(buffer, " ");
            while(token){
                lower(token);
                if(strcmp(token, key->text) == 0){
                    appear_time++;
                }
                token = strtok(NULL, " ");
            }
            
            if (appear_time > 0){   // end up here b/c we found the same word as keyword
                print_out(key->text, temp->text, line_num, appear_time, max_key_len);
            }

            temp = temp->next;
            line_num ++;
        }

        key = key->next;
    }
    
}

int main(int argc, char *argv[])
{
    char *exception_file = NULL;
    char *input_file = NULL;
    int max_key_len = 0;
    int i;  

    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-e") == 0 && i+i <= argc) {
            exception_file = argv[++i];
        }
        else {
            input_file = argv[i];
        }
    }
    // start copying file
    if (exception_file == NULL){

    }
    
    FILE *input = fopen(input_file, "r");
    node_t *exception_list = NULL;
    node_t *input_list = NULL;

    if (exception_file != NULL){    // argv contains exception file and input file
        FILE *exception = fopen(exception_file, "r");
        exception_list = copy_file(exception);
        fclose(exception);
    }

    input_list = copy_file(input);
    fclose(input);  // Finished copy file
    // start getting keywords
    node_t *key_list = NULL;
    key_list = get_key(input_list, exception_list, &max_key_len);
    // build frame of final output and print it out.
    final_frame(key_list, input_list, &max_key_len);
    // free memory
    free_list(exception_list);
    free_list(input_list);
    free_list(key_list);

/* 
 * Showing some simple usage of the linked-list routines.
 */

/*#ifdef DEBUG
    _demo();
#endif
*/
    exit(0);
}
