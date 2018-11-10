#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

//Create struct ListNode
struct ListNode
{
    char* data;
    struct ListNode* next;
};

//Store position of field for display selected field
struct ListPosNode
{
    int data;
    struct ListPosNode* next;
};

//Function declaration
//Generate new file
void createFile(char* name);

//Read in data
void read(char*** readField, struct ListNode** readVal, int* readSize, char* filename);

//Create struct array to store data
void createStructArray(struct ListNode** arrStruct, int keyValueSize);

//Insert Field in array
void insertField(char*** keyField, int* keyFieldSize, char* field);

//Insert value in struct data
void insertValue(struct ListNode* listVal, char* val);

//Write data to file
void writeToFile(char** keyWrite, struct ListNode* valWrite, int keyWriteSize, char* filename);

//Delete database
void dropTable(char* name);

//Show database
void displayTable(char** keyDisplay, struct ListNode* valDisplay, int keyDisplaySize);

//Search for position of field and value
int posField(char** keyPos, int sizePos, char* target);
int posValue(struct ListNode* listPos, int posField, char* targetVal);

//Add new value to table
void addValue(char* fieldAdd, char* valAdd, char* filename);

//Update value in table
void updateTable(char* fieldSet, char* valSet, char* fieldWhere, char* valWhere, char* filename);

//Delete row of table
void deleteItem(char* fieldDel, char* valDel, char* filename);

//Select value to display
void select(char* chooseField, char* fieldTarget, char* valTarget, char* filename);

int main()
{
    //Variable delaration
    char** key;
    struct ListNode* val;
    int keySize = 1;
    int pos = 0;

    char command[30];
    char name[100];
    char value[100];
    char field[100];
    char wvalue[100];//where value
    char wfield[100];//where field
    char sfield[100];//selected field
    
    printf("Enter command: \n");
    scanf("%s",command);

        //Program command
        if (strcmp(command,"CREATE") == 0)
        {
            printf("Enter table name:\n");
            scanf("%s",name);
            createFile(name);
            printf("Enter fields:\n");
            scanf("%s",field);
            insertField(&key, &keySize, field);
            writeToFile(key,val,keySize,name);
        }
        else if (strcmp(command,"UPDATE") == 0)
        {
            printf("Enter table name:\n");
            scanf("%s",name);
            printf("Enter set field:\n");
            scanf("%s",field);
            printf("Enter set value:\n");
            scanf("%s",value);
            printf("Enter where field:\n");
            scanf("%s",wfield);
            printf("Enter where value:\n");
            scanf("%s",wvalue);
            updateTable(field, value, wfield, wvalue, name);
        }
        else if (strcmp(command,"DELETE") == 0)
        {
            printf("Enter table name:\n");
            scanf("%s",name);
            printf("Enter where field:\n");
            scanf("%s",wfield);
            printf("Enter where value:\n");
            scanf("%s",wvalue);
            deleteItem(wfield, wvalue, name);
        }
        else if (strcmp(command,"SELECT") == 0)
        {
            printf("Enter table name:\n");
            scanf("%s",name);
            printf("Enter select field:\n");
            scanf("%s",sfield);
            printf("Enter where field:\n");
            scanf("%s",wfield);
            printf("Enter where value:\n");
            scanf("%s",wvalue);
            select(sfield,wfield,wvalue,name);
        }
        else if (strcmp(command,"INSERT") == 0)
        {
            printf("Enter table name:\n");
            scanf("%s",name);
            printf("Enter field:\n");
            scanf("%s",field);
            printf("Enter value:\n");
            scanf("%s",value);
            addValue(field,value,name);    
        }                
        else if (strcmp(command,"DROP") == 0)
        {
            printf("Enter table name:\n");
            scanf("%s",name);
            dropTable(name);
        }      

    return 0;
}

void addValue(char* fieldAdd, char* valAdd, char* filename)
{
    //Concate file type txt
    char* type = ".txt";
    char* str = (char*) malloc((strlen(filename)+strlen(type))*sizeof(char));
    if (str == NULL)
    {
        printf("Malloc failed!\n");
        exit(1);
    }

    strcpy(str, filename);
    strcat(str, type);

    char** key;
    struct ListNode* val;
    int keySize = 1;
    int addFieldPos = -1;
    int addValPos = -1;

    //Read in data to memory
    read(&key, &val, &keySize, str);

    //Check position
    addFieldPos = posField(key, keySize, fieldAdd);

    //Error handling
    if (addFieldPos == -2)
    {
        printf("Database Empty\n");
    }
    else if (addFieldPos == -1)
    {
        exit(1);
    }
    else
    {
        for (int i = 0; i < keySize; i++)
        {
            struct ListNode* current =  &val[i];
        
            while (current->data != NULL)
            {
                current = current->next;
            }

            //if (current->data == NULL)
            {
                
                if (i == addFieldPos)
                {
                    current->data = valAdd;
                }
                else
                {
                    current->data = NULL;
                }
                current->next = (struct ListNode*) malloc(sizeof(struct ListNode));
                current = current->next;
                current->data = NULL;
                current->next = NULL;
            }
        }
    }

    //Display and Write changes to file
    displayTable(key,val,keySize);
    writeToFile(key,val,keySize,filename);
}

void select(char* chooseField, char* fieldTarget, char* valTarget, char* filename)
{
    //Choose field to display
    struct ListPosNode* chooseFieldPos = (struct ListPosNode*) malloc(sizeof(struct ListPosNode));
    chooseFieldPos->next = NULL;
    struct ListPosNode* fieldPosHead = chooseFieldPos;

    char* type = ".txt";
    char* file = (char*) malloc((strlen(filename)+strlen(type))*sizeof(char));
    if (file == NULL)
    {
        printf("Malloc failed!\n");
        exit(1);
    }

    strcpy(file, filename);
    strcat(file, type);

    char** key;
    struct ListNode* val;
    int keySize = 1;

    //Read in data
    read(&key, &val, &keySize, file);

    int size = 10;
    int count = 1;
    int item = 0;
    int k = 0;
    char* str = (char*) malloc(size*sizeof(char));

    char c;

    //Find position of field element to display and store it ListPosNode
    for(int i = 0; i <= strlen(chooseField); ++i)
    {
        c = chooseField[i];
        if ((c != ',') && (c != '\n') && (c != '\0'))
        {
            count++;
            if (count >= size)
            {
                //Reallocating memory
                size = size*2;
                str = (char*)realloc(str, size*sizeof(char));  
            }
            str[k] = c;
            k++;
        }
        else
        {
            str[k] = '\0';

            int a = 0;
            int compare = 0;
            char** checkNull;
            for (int i = 0; i < keySize; ++i) {

                char *pos = key[i];
                checkNull = &key[i];
                if (**checkNull)
                {
                    while (*pos != '\0') {
                        if (str[a] == *(pos++))
                        {
                            compare++;
                        }
                        a++;
                    }
                    if (compare == strlen(str))
                    {
                        chooseFieldPos->data = i;
                        chooseFieldPos->next = (struct ListPosNode*) malloc(sizeof(struct ListPosNode));
                        chooseFieldPos = chooseFieldPos->next;
                        chooseFieldPos->next = NULL;   
                    }
                    a = 0;
                    compare = 0;
                }
                else
                {   
                    a = 0;
                    compare = 0;  
                    break;
                }
            }
            
            size = 10;
            count = 1;
            str = (char*) malloc(size*sizeof(char));
            k = 0;
        }  
    }

    int selectFieldPos = -2;
    int selectValPos = -2;

    //Search for position of element to display
    selectFieldPos = posField(key, keySize, fieldTarget);
    selectValPos= posValue(val, selectFieldPos, valTarget);

    if (selectFieldPos == -2)
    {
        printf("Database Empty\n");
    }
    else if (selectValPos == -2)
    {
        printf("Data Empty\n");
    }
    else if (selectFieldPos == -1 || selectValPos == -1)
    {
        exit(1);
    }
    //If * we will display everything
    else if (strcmp(chooseField,"*") == 0)
    {
        char* checkNull;

        for (int i = 0; i < keySize; ++i) {
            char *pos = key[i];
            checkNull = key[i];
            if (*checkNull != '\0')
            {
                while (*pos != '\0') {
                    printf("%c", *(pos++));
                }
                printf("\t");
            }
            else
            {   
                printf("\n"); 
                break;
            }
        }

        struct ListNode* current = &val[0];;
        int flag = 0;

        while (current->next != NULL)
        {
            
            for (int i = 0; i < keySize; ++i) 
            {
                current = &val[i];
                int count = 0;
                while (count != flag)
                {
                    if (current->next != NULL)
                    {
                        current = current->next;
                        count++;
                    }
                    else
                    {
                        break;
                    }
                }
                
                if (current->data != NULL)
                {
                    if (flag == selectValPos)
                    {
                        printf("%s\t", current->data);
                    }
                }
                else
                {   
                    //printf("\n");
                    current = &val[0];
                    count = 0;
                    while (count != flag)
                    {
                        current = current->next;
                        count++;
                    }
                    break;
                }
            }
        flag++; 
        }
        printf("\n");
    }
    //Display only select field
    else
    {
            struct ListPosNode* fieldPos = fieldPosHead;

            if (fieldPos->next == NULL)
            {
                printf("Cannot find fields\n");
                return;
            }

            while (fieldPos->next != NULL)
            {
                char *pos = key[fieldPos->data];
            
                while (*pos != '\0') 
                {
                    printf("%c", *(pos++));
                }
                printf("\t");
                fieldPos = fieldPos->next;
            } 
            printf("\n");

            struct ListPosNode* valPos = fieldPosHead;

            //while (valPos->next != NULL)
            {
                struct ListNode* current = &val[0];;
                int flag = 0;

                while (current->next != NULL)
                {
                    struct ListPosNode* valPosLoop = fieldPosHead;
                    
                    while (valPosLoop->next != NULL)
                    //for (int i = 0; i < keySize; ++i) 
                    {
                        current = &val[valPosLoop->data];
                        int count = 0;
                        while (count != flag)
                        {
                            if (current->next != NULL)
                            {
                                current = current->next;
                                count++;
                            }
                            else
                            {
                                break;
                            }
                        }
                        
                        if (current->data != NULL)
                        {
                            if (flag == selectValPos)
                            {
                                printf("%s\t", current->data);
                            }
                            valPosLoop = valPosLoop->next;
                        }
                        else
                        {   
                            printf("\n");
                            current = &val[0];
                            count = 0;
                            while (count != flag)
                            {
                                current = current->next;
                                count++;
                            }
                            break;
                        }
                    }
                    //printf("\n");
                    flag++; 
                }
                //valPos = valPos->next;
            }
    }
}

void deleteItem(char* fieldDel, char* valDel, char* filename)
{
    char* type = ".txt";
    char* str = (char*) malloc((strlen(filename)+strlen(type))*sizeof(char));
    if (str == NULL)
    {
        printf("Malloc failed!\n");
        exit(1);
    }

    strcpy(str, filename);
    strcat(str, type);

    char** key;
    struct ListNode* val;
    int keySize = 1;
    int delFieldPos = -1;
    int delValPos = -1;

    read(&key, &val, &keySize, str);

    //Find Position of element needed to delete
    delFieldPos = posField(key, keySize, fieldDel);
    delValPos = posValue(val, delFieldPos, valDel);

    if (delFieldPos == -2)
    {
        printf("Database Empty\n");
    }
    else if (delValPos == -2)
    {
        printf("Data Empty\n");
    }
    else if (delValPos == -1 || delFieldPos == -1)
    {
        exit(1);
    }
    else
    {
        //We delete the row by moving position of pointer and free the target node
        struct ListNode* current = NULL;
  
        for (int i = 0; i < keySize; ++i) 
        {
            current = &val[i];
            if (current->data == NULL)
            {
                break;  
            }
            
            if (delValPos == 0)
            {
                val[i] = *current->next;           
            }
            else
            {
                for (int i=0; current!=NULL && i<delValPos-1; i++)
                {
                    current = current->next;
                }
                if (current == NULL || current->next == NULL)
                {
                    printf("Target is out of range\n");
                    return;  
                }
                struct ListNode *next = current->next->next;
                free(current->next); 
                current->next = next;
            }
        }
        displayTable(key,val,keySize);
        writeToFile(key,val,keySize,filename);
    }
}

void updateTable(char* fieldSet, char* valSet, char* fieldWhere, char* valWhere, char* filename)
{
    char* type = ".txt";
    char* str = (char*) malloc((strlen(filename)+strlen(type))*sizeof(char));
    if (str == NULL)
    {
        printf("Malloc failed!\n");
        exit(1);
    }

    strcpy(str, filename);
    strcat(str, type);

    char** key;
    struct ListNode* val;
    int keySize = 1;
    int updateFieldWhere = -2;
    int updateValWhere = -2;

    int updateFieldSet = -2;
    int targetPos = 0;

    read(&key, &val, &keySize, str);

    //Find position of target update node
    updateFieldWhere = posField(key, keySize, fieldWhere);
    updateValWhere = posValue(val, updateFieldWhere, valWhere);
    updateFieldSet = posField(key, keySize, fieldSet);

    if (updateFieldWhere == -2)
    {
        printf("Database Empty\n");
    }
    else if (updateValWhere == -2)
    {
        printf("Data Empty\n");
    }
    else if (updateFieldWhere == -1 || updateFieldWhere == -1 || updateFieldSet == -1)
    {
        exit(1);
    }
    else
    {
        //Move pointer to position and update value
        struct ListNode* update = &val[updateFieldSet];

        while(update->data != NULL)
        {
            if (targetPos == updateValWhere)
            {
                update->data = valSet;
                break;
            }
            else
            {
                update = update->next;
                targetPos++;
            }
        }

        displayTable(key,val,keySize);
        writeToFile(key,val,keySize,filename);
    }
}

void dropTable(char* name)
{
    char* type = ".txt";
    char* str = (char*) malloc((strlen(name)+strlen(type))*sizeof(char));
    if (str == NULL)
    {
        printf("Malloc failed!\n");
        exit(1);
    }

    strcpy(str, name);
    strcat(str, type);
    //Delete table
    if (remove(str) == 0)
      printf("Table Drop Success");
    else
      printf("Cannot Drop Table");
}   

void read(char*** readField, struct ListNode** readVal, int* readSize, char* filename)
{
    FILE *fp2 = fopen(filename, "r");

    if (fp2==NULL)
    {
        printf("Cannot open file\n");
        exit(1);
    }

	char line[1000];

    char** key;
    struct ListNode* val;
    int size = *readSize;
	int flag = -1;

    //Read in each line
	while (fgets(line, sizeof(line), fp2)) {
        flag++;
        if (flag == 0)
        {
            //Insert field to array
            insertField(&key, &size, line);
        } 
        else if (flag == 1)
        {
            //Create an array of struct to insert value
            //Check for first line of data with flag
            createStructArray(&val, size);
            insertValue(val,line);
        } 
        else
        {
            insertValue(val,line);
        }
    }

    if (flag == 0)
    {
        createStructArray(&val, size);
    }

    if (flag != -1)
    {
        *readField = key;
        *readVal = val;
        *readSize = size;
    }
    else
    {
        *readField = NULL;
        *readVal = NULL;
    }
	fclose(fp2);
}

void createStructArray(struct ListNode** arrStruct, int keyValueSize)
{
    //Create node for struct array
    struct ListNode* arr = malloc(keyValueSize * sizeof(struct ListNode));
    struct ListNode* head = arr;
    struct ListNode* current;
    for (int i = 0 ; i < keyValueSize; i++)
    {
        current = &arr[i];
        current->data = NULL;
        current->next = NULL;
    }
    *arrStruct = &*head;
}

int posValue(struct ListNode* listPos, int posField, char* targetVal)
{
    if (listPos == NULL)
    {
        return -2;
    }

    struct ListNode* current =  &listPos[posField];
    int count = 0;
    while(current->data != NULL)
    {
        //Compare to find target value
        if (strcmp(current->data,targetVal) == 0)
        {
            return count;
        }
        else
        { 
            current = current->next;
            count++;
        }
    }
    printf("Value Does Not Exist\n");
    return -1;
}

int posField(char** keyPos, int sizePos, char* target)
{
    if (keyPos == NULL)
    {
        return -2;
    }

    char** checkNull;
    int k = 0;
    int count = 0;

    //Find position for field
    for (int i = 0; i < sizePos; ++i) {
        char *pos = keyPos[i];
        checkNull = &keyPos[i];
        if (*checkNull != NULL)
        {
            while ((*pos != '\0')) {
                if (target[k]==*(pos++))
                {
                    count++;
                }
                k++;
            }
            //Compare each character if found mean string length equal
            if (count == strlen(target))
            {
                //printf("%s%d\n","Field Found\n",i);
                return i; 
            }
            else
            {
              k = 0;
              count = 0;
            }
        }
        else
        {   
            printf("Field Not Found\n");
            return -1;
        }
    }
    return 0;  
}

void writeToFile(char** keyWrite, struct ListNode* valWrite, int keyWriteSize, char* filename)
{
    char* type = ".txt";
    char* str = (char*) malloc((strlen(filename)+strlen(type))*sizeof(char));
    if (str == NULL)
    {
        printf("Malloc failed!\n");
        exit(1);
    }

    strcpy(str, filename);
    strcat(str, type);

    FILE * f;
    f = fopen(str, "w");
    if (f == NULL)
    {
        printf("Error opening file!\n");
        exit(1);
    }

    //Display Value
    char** checkNull;

    if (*keyWrite == '\0')
    {
        //printf("%s", "Field Empty\n");
        return;
    }

    for (int i = 0; i < keyWriteSize; ++i) {
        char *pos = keyWrite[i];
        checkNull = &keyWrite[i];
        if (**checkNull)
        {
            while (*pos != '\0') {
                fprintf(f, "%c", *(pos++));
            }
            fprintf(f, ",");
        }
        else
        {   
            fprintf(f, "\n"); 
            break;
        }
    }

    if (valWrite == NULL)
    {    
        //printf("%s", "Data Empty\n");
        return;   
    }

    //Display Value
    struct ListNode* current = &valWrite[0];
    struct ListNode* run = current;
    int flag = 0;

    while (current->next != NULL)
    {
        
        for (int i = 0; i < keyWriteSize; ++i) 
        {
            current = &valWrite[i];
            int count = 0;
            if (current->data != NULL)
            {
                run = current;
                //int count = 0;
                while (count != flag)
                {
                    if (run->next != NULL)
                    {
                        run = run->next;
                        count++;
                    }
                    else
                    {
                        break;
                    }
                }
                fprintf(f,"%s,", run->data);
            }
            else
            {   
                fprintf(f,"\n");
                current = &valWrite[0];
                run = current;
                count = 0;
                while (count != flag)
                {
                    current = current->next;
                    count++;
                }
                break;
            }
        }
      flag++; 
    }
    fclose(f);
}

//Display is similar to write to file
void displayTable(char** keyDisplay, struct ListNode* valDisplay, int keyDisplaySize)
{
    char** checkNull;

    if (*keyDisplay == '\0')
    {
        printf("%s", "Field Empty\n");
        return;
    }

    for (int i = 0; i < keyDisplaySize; ++i) {
        char *pos = keyDisplay[i];
        checkNull = &keyDisplay[i];
        if (*checkNull != NULL)
        {
            while (*pos != '\0') {
                printf("%c", *(pos++));
            }
            printf("\t");
        }
        else
        {   
            printf("\n"); 
            break;
        }
    }

    if (valDisplay == NULL)
    {    
        printf("%s", "Data Empty\n");
        return;   
    }

    struct ListNode* current = &valDisplay[0];
    struct ListNode* run = current;
    int flag = 0;

    while (current->next != NULL)
    {
        
        for (int i = 0; i < keyDisplaySize; ++i) 
        {
            current = &valDisplay[i];
            int count = 0;
            if (current->data != NULL)
            {
                run = current;
                //int count = 0;
                while (count != flag)
                {
                    if (run->next != NULL)
                    {
                        run = run->next;
                        count++;
                    }
                    else
                    {
                        break;
                    }
                }
                
                //Check if data is null to print
                {
                    printf("%s\t", run->data);
                }
            }
            else
            {   
                printf("\n");
                current = &valDisplay[0];
                run = current;
                count = 0;
                while (count != flag)
                {
                    current = current->next;
                    count++;
                }
                break;
            }
        }
      flag++; 
    }
}

void insertField(char*** keyField, int* keyFieldSize, char* field)
{
    //The field size will dynamically resize according to data size
    char c;
    //int keyFieldSize = 1;
    char** key = (char**)malloc(*keyFieldSize*sizeof(char**));
    
    //char* field = "field1,field2";
    int size = 10;
    int count = 1;
    int item = 0;
    int k = 0;
    char* str = (char*) malloc(size*sizeof(char));
    for(int i = 0; i <= strlen(field); i++)
    {
        c = field[i];
        if ((c != ',') && (c != '\n') && (c != '\0'))
        {
            count++;
            if (count >= size)
            {
                //Reallocating memory
                size = size*2;
                str = (char*)realloc(str, size*sizeof(char));  
            }
            str[k] = c;
            k++;
        }
        else
        {
            str[k] = '\0';
            key[item] = str;
            item++;
            size = 10;
            count = 1;
            str = (char*) malloc(size*sizeof(char));
            k = 0;
            
            if (item >= *keyFieldSize)
            {
                *keyFieldSize = *keyFieldSize*2;
                key = (char**)realloc(key, *keyFieldSize*sizeof(char*));
            }
        }  
    }
    key[item] = 0;
    *keyField = key;
    free(str);
}

void insertValue(struct ListNode* listVal, char* val)
{
    //The size of insertvalue depends on struct size
    char c;
    int size = 10;
    int count = 1;
    int item = 0;
    int k = 0;
    int posVal = 0;
    char* str = (char*) malloc(size*sizeof(char));
    
    struct ListNode* current =  &listVal[posVal];
    
    for(int i = 0; i <= strlen(val); i++)
    {
        c = val[i];
        if (c != ',')
        {
            count++;
            if (count >= size)
            {
                //Reallocating memory
                size = size*2;
                str = (char*)realloc(str, size*sizeof(char));  
            }
            str[k] = c;
            k++;
        }
        else
        {
            str[k] = '\0';
            
            while(current->data != NULL)
            {
                current = current->next;  
            }
            
            if (current->data == NULL)
            {
                if (strcmp(str,"(null)")==0)
                {
                    current->data = NULL;
                }
                else
                {
                    current->data = str;
                }
                current->next = (struct ListNode*) malloc(sizeof(struct ListNode));
                current = current->next;
                current->data = NULL;
                current->next = NULL;
            }
            
            posVal++;
            current =  &listVal[posVal];
            item++;
            size = 10;
            count = 1;
            str = (char*) malloc(size*sizeof(char));
            k = 0;
        }
    }
}

void createFile(char* name)
{
    char* type = ".txt";
    char* str = (char*) malloc((strlen(name)+strlen(type))*sizeof(char));
    if (str == NULL)
    {
        printf("Malloc failed!\n");
        exit(1);
    }

    strcpy(str, name);
    strcat(str, type);

    //Open file to write data to
    FILE * fp;
    fp = fopen(str, "w");
    if (fp ==  NULL)
    {
        perror("Failed to create data file");
        exit(1);
    }
    else
    {
        fclose(fp);
    }
}