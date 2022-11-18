#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int main(){
    FILE *ham_train;
    FILE *spam_train;
    
    char line_ham[1024];
    ham_train = fopen("csv/test/dataset_ham_test20.csv","r");
    if(ham_train==NULL){
        printf("error");
        return 0;
    }

    while(ham_train)

    fclose(ham_train);

    return 0;
}

