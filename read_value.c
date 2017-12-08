
#include "read_value.h"
#include "structures.h"


int read_value(char* valueName) {
#ifdef TRACE
  printf("(read_value) from file %s",CONFIG_FILENAME);
#endif
    int i;
    int value = 0; //value to return
    char output[256],varName[50],tempChar;

    FILE* fpointer = fopen(CONFIG_FILENAME, "r"); //set to read only

    if (fpointer == NULL) {
        fprintf(stderr,"(read_value)Cant find file %s\n",CONFIG_FILENAME);
        return 0;
    }
    do{
        tempChar= (char)fgetc(fpointer);
        if(tempChar!='#'){
            for (i=0;(tempChar)!=' ' &&i<50;i++){
                varName[i]=tempChar;
                tempChar=(char)fgetc(fpointer);
            }
            varName[i] = '\0';
            if (strcmp(varName,valueName)==0){
                value = atoi(fgets(output,20,fpointer));
            }
        }
    }while (fgets(output,200,fpointer)!=NULL && value==0);

  fclose(fpointer);

  return value;
}

void clean_worlds_dir(){


#ifdef TRACE
    printf("\ncleaning");
#endif
    char fileName[50];
    int i=0;
    do{
        sprintf(fileName, "%s%d%s",FILE_PREFIX,i,FILE_SUFFIX);
#ifdef TRACE
        printf("\n  suppr: %s",fileName);
#endif
        i++;
    }while (remove(fileName)==0);

}

/**
 * save the given world in a file
 * the file name is defined by FILE_PREFIX and FILE_SUFFIX from the header file.
 * Cann write on stderr on error
 * @param my_world, the current step, the config structure
 * @return 0 if error, 1 if ok
 */
int export_world_t(World_t my_world, int step, Config_t* myConfig) {

    FILE* backup_file = NULL;
    char fileName[255];
    sprintf(fileName, "%s%d%s",FILE_PREFIX, step,FILE_SUFFIX);
    backup_file = fopen(fileName,"w");

    if ( backup_file == NULL ){
        fprintf(stderr, "\n(export_world_t) Cannot create the storage file.\n\tBe sure to have a worlds folder in the working directory!\n");
        return 0; /* indicate failure.*/
    }

    int i,j , temp=1;
    for (i=0;i<myConfig->CELLS;i++){
        for (j = 0;  j < myConfig->CELLS; j++) {
            temp = fprintf(backup_file,"%d",my_world[i][j]->status);
            //temp = fputc(my_world[i][j]->status,backup_file);
            //temp = fwrite()
            if (temp<=0){
                fprintf(stderr, "\n(export_world_t) Cannot write in the file!\n");
                return 0; /* indicate failure.*/
            }
        }
    }

    temp = fclose(backup_file);
    if (temp!=0){
        fprintf(stderr, "\n(export_world_t) Cannot close the file!\n");
        return 0; /* indicate failure.*/
    }
    return 1;
}


/**
 * Initialise the variables of the structure from the config file.
 * The configuration file name is define in the header file by CONFIG_FILENAME
 * @param pointer on the config structure.
 * @return the same pointer or NULL on error
 */
Config_t* initConfig(Config_t* myConfig) {

#ifdef TRACE
    printf("(initConfig) from file %s",CONFIG_FILENAME);
#endif
    int i=0,j=0;
    char output[256],varName[50],tempChar;


    //myConfig = (Config_t*) malloc(sizeof(Config_t));
    if ( myConfig == NULL){
        fprintf(stderr, "(initConfig) memory allocation failled (Config_t).\n");
        return NULL;
    }

    FILE* fpointer = fopen(CONFIG_FILENAME, "r"); //set to read only
    if (fpointer == NULL) {
        fprintf(stderr,"(initConfig) Cant find file %s\n",CONFIG_FILENAME);
        return NULL;
    }
    do{
        tempChar= (char)fgetc(fpointer);
        if(tempChar!='#'){
            for (i=0;(tempChar)!=' ' &&i<50;i++){
                varName[i]=tempChar;
                tempChar=(char)fgetc(fpointer);
            }
            varName[i] = '\0';
            if (strcmp(varName,"CELLS")==0){
                myConfig->CELLS =(unsigned int)  atoi(fgets(output,20,fpointer));
            }else if (strcmp(varName,"STEPS")==0){
                myConfig->STEPS =(unsigned int)  atoi(fgets(output,20,fpointer));
            }else if (strcmp(varName,"EMPTY")==0){
                myConfig->EMPTY = (unsigned int) atoi(fgets(output,20,fpointer));
            }else if (strcmp(varName,"INFECTED")==0){
                myConfig->INFECTED = (unsigned int) atoi(fgets(output,20,fpointer));
            }else if (strcmp(varName,"PROTECTED")==0){
                myConfig->PROTECTED = (unsigned int) atoi(fgets(output,20,fpointer));
            }else if (strcmp(varName,"AGING")==0){
                myConfig->AGING =(unsigned int)  atoi(fgets(output,20,fpointer));
            }else if (strcmp(varName,"BIRTH")==0){
                myConfig->BIRTH = (unsigned int) atoi(fgets(output,20,fpointer));
            }else if (strcmp(varName,"NATURAL_CONTAMINATION")==0){
                myConfig->NATURAL_CONTAMINATION = (unsigned int) atoi(fgets(output,20,fpointer));
            }else if (strcmp(varName,"INFECTIOUS_CONTAMINATION")==0){
                myConfig->INFECTIOUS_CONTAMINATION =(unsigned int)  atoi(fgets(output,20,fpointer));
            }else if (strcmp(varName,"INFECTED_STEP1")==0){
                myConfig->INFECTED_STEP1 = (unsigned int) atoi(fgets(output,20,fpointer));
            }else if (strcmp(varName,"INFECTED_STEP2")==0){
                myConfig->INFECTED_STEP2 =(unsigned int)  atoi(fgets(output,20,fpointer));
            }else if (strcmp(varName,"INFECTED_STEP3")==0){
                myConfig->INFECTED_STEP3 = (unsigned int) atoi(fgets(output,20,fpointer));
            }else if (strcmp(varName,"DEATH_DURATION")==0){
                myConfig->DEATH_DURATION =(unsigned int)  atoi(fgets(output,20,fpointer));
            }else if (strcmp(varName,"INFECTIOUSNESS1")==0){
                myConfig->INFECTIOUSNESS1 = (unsigned int) atoi(fgets(output,20,fpointer));
            }else if (strcmp(varName,"INFECTIOUSNESS2")==0){
                myConfig->INFECTIOUSNESS2 = (unsigned int) atoi(fgets(output,20,fpointer));
            }else if (strcmp(varName,"INFECTIOUSNESS3")==0){
                myConfig->INFECTIOUSNESS3 = (unsigned int) atoi(fgets(output,20,fpointer));
            }else if (strcmp(varName,"PROTECTION_DURATION")==0){
                myConfig->PROTECTION_DURATION = (unsigned int) atoi(fgets(output,20,fpointer));
            }else if (strcmp(varName,"INFECTED_SPREADING")==0){
                myConfig->INFECTED_SPREADING = (unsigned int) atoi(fgets(output,20,fpointer));
            }
        } else{
            j =fgets(output,200,fpointer)!=NULL;
        }
    }while (j && tempChar!=EOF);


    fclose(fpointer);

    return testConfig(myConfig);
}

Config_t* testConfig(Config_t *myConfig) {
    if(myConfig==NULL){
        fprintf(stderr,"\n(testConfig) NULL pointer\n");
        return NULL;
    }
    if(myConfig->CELLS > 10000) {
        fprintf(stderr,"\n(testConfig) CELLS\n");
        return NULL;
    }
    if(myConfig->STEPS > 5000) {
        fprintf(stderr,"\n(testConfig) STEPS\n");
        return NULL;
    }
    if(myConfig->EMPTY > 100) {
        fprintf(stderr,"\n(testConfig) EMPTY\n");
        return NULL;
    }
    if(myConfig->INFECTED > 100) {
        fprintf(stderr,"\n(testConfig) INFECTED\n");
        return NULL;
    }
    if(myConfig->PROTECTED > 100) {
        fprintf(stderr,"\n(testConfig) PROTECTED\n");
        return NULL;
    }
    if(myConfig->AGING > 1000) {
        fprintf(stderr,"\n(testConfig) AGING\n");
        return NULL;
    }
    if(myConfig->BIRTH > 10000) {
        fprintf(stderr,"\n(testConfig) BIRTH\n");
        return NULL;
    }
    if(myConfig->NATURAL_CONTAMINATION > 100) {
        fprintf(stderr,"\n(testConfig) NATURAL_CONTAMINATION\n");
        return NULL;
    }
    if(myConfig->INFECTIOUS_CONTAMINATION > 100) {
        fprintf(stderr,"\n(testConfig) INFECTIOUS_CONTAMINATION\n");
        return NULL;
    }
    if(myConfig->INFECTED_STEP1 > 50) {
        fprintf(stderr,"\n(testConfig) INFECTED_STEP1\n");
        return NULL;
    }
    if(myConfig->INFECTED_STEP2 > 50) {
        fprintf(stderr,"\n(testConfig) INFECTED_STEP2\n");
        return NULL;
    }
    if(myConfig->INFECTED_STEP3 > 50) {
        fprintf(stderr,"\n(testConfig) INFECTED_STEP3\n");
        return NULL;
    }
    if(myConfig->DEATH_DURATION > 1000) {
        fprintf(stderr,"\n(testConfig) DEATH_DURATION\n");
        return NULL;
    }
    if(myConfig->INFECTIOUSNESS1 > 100) {
        fprintf(stderr,"\n(testConfig) INFECTIOUSNESS1\n");
        return NULL;
    }
    if(myConfig->INFECTIOUSNESS2 > 100) {
        fprintf(stderr,"\n(testConfig) INFECTIOUSNESS2\n");
        return NULL;
    }
    if(myConfig->INFECTIOUSNESS3 > 100) {
        fprintf(stderr,"\n(testConfig) INFECTIOUSNESS3\n");
        return NULL;
    }
    if(myConfig->PROTECTION_DURATION > 1000) {
        fprintf(stderr,"\n(testConfig) PROTECTION_DURATION\n");
        return NULL;
    }
    if(myConfig->INFECTED_SPREADING > 100) {
        fprintf(stderr,"\n(testConfig) INFECTED_SPREADING\n");
        return NULL;
    }

    return myConfig;
}

