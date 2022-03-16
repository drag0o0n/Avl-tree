/*
this program shows a menu to the user
the user can choose any of the options
all of the options in the 1st menu is exclusive for avl tree and it is implementation
the implementation here is a dictionary
the program can read data from a file and store in the avl tree
balancing the tree by using the 2 kinds of rotation is done automatically
the user also can enter new words and elements to the dictionary (the tree)
delete some words or all or list them by lexicographic sort and other intresting options
the 2nd menu is for hashing and it is implemmentation
u can read data from file ,add to it , see the hashing function as a string
and also other options

Please check the file name to be the same as the ones in the program
Please also be sure to enter the right numbers to have access to the option u need

thx
*/

#include<string.h>
#include<conio.h>
#include<stdlib.h>
#include<strings.h>
#include<stdio.h>
#include<malloc.h>



#define MS 250
#define HT 1
int v;
FILE *out;
typedef struct NODE hashTabel;
typedef struct NODE//the hash table elements
{
    char oword[MS];
    char meanings[20][MS];
    char synonym[MS];
    char opposite[MS];
    int mnumber;
    int clear;
};
//the avl tree creation and elements
typedef struct node *tree;
typedef struct node
{
    char phrase[MS];
    char synonym[MS];
    char opposite[MS];
    char meanings[20][MS];
    int mnumber;
    tree leftside;
    tree rightside;
    int height;
};

hashTabel *h;


int get_height(tree root)//calculate tree height
{
    if(!root)
        return 0;
    else
        return root->height;
}

tree get_node(char val[MS],char meaning[MS], char synonym[MS], char opposite[MS])//avl creation
{
    tree new_node = (tree)malloc(sizeof(struct node));//allocate it in memory
    new_node->leftside = NULL;//intialize both sides with null
    new_node->rightside = NULL;
    new_node->mnumber = 0;
    strcpy(new_node->phrase, val);
    strcpy(new_node->synonym,synonym);
    strcpy(new_node->opposite,opposite);
    char mm[strlen(meaning)];
    strcpy(mm,meaning);
    char *p;
    p = strtok(mm,",");
    strcpy(new_node->meanings[0],p);
    new_node->mnumber++;
    while (p!= NULL)
    {
        p = strtok(NULL,",");
        if(p!= NULL)
        {
            strcpy(new_node->meanings[new_node->mnumber], p);
            new_node->mnumber++;
        }
    }
    new_node->height = HT;
    return new_node;
}

int biggest(int a, int b)//this function determine the biggest number
{
    return (a > b) ? a : b;
}

int get_balance(tree root)
{
    if(!root)
        return 0;
    return (get_height(root->leftside) - get_height(root->rightside));
}


tree lr(tree root)//left rotate for balancing tree
{
    tree rightside = root->rightside;

    tree leftside = rightside->leftside;

                                //rotation process
    rightside->leftside = root;

    root->rightside = leftside;

    root->height = biggest(get_height(root->leftside), get_height(root->rightside)) + 1;//change the  heights with new one

    rightside->height = biggest(get_height(rightside->leftside), get_height(rightside->rightside)) + 1;
    return rightside;
    //at end it will return the new root
}

tree rr(tree root)//right rotation for balancing
{
    tree leftside = root->leftside;

    tree rightside = leftside->rightside;

                                    //rotation process
    leftside->rightside = root;

    root->leftside = rightside;


    root->height = biggest(get_height(root->leftside), get_height(root->rightside)) + 1;//new heights after rotation
    leftside->height = biggest(get_height(leftside->leftside), get_height(leftside->rightside)) + 1;


    return leftside;

    //at end it will return the new root after the right rotation
}

tree inorder_succ_right_tree(tree root)
{
    tree space = root->rightside;
    while(space->leftside)
    {
        space = space->leftside;
    }
    return space;
}

tree putin(tree root, char val[50],char meaning[MS],char synonym[MS], char opposite[MS])
{//the function add node in tree
    //normal insertion
    if(!root)
    {
        tree new_node = get_node(val,meaning,synonym,opposite);
        return new_node;
    }
    if(strcmp(root->phrase,val) > 0)
    {
        root->leftside = putin(root->leftside, val,meaning,synonym,opposite);
    }
    else
    {
        root->rightside = putin(root->rightside, val,meaning,synonym,opposite);
    }

    root->height = biggest(get_height(root->leftside), get_height(root->rightside)) + 1;//new heights

    int balance = get_balance(root);//balance checking

    if(balance > 1 && strcmp(root->leftside->phrase, val) > 0)
    {//for left left
        root = rr(root);
    }

    else if(balance < -1 && strcmp(root->rightside->phrase, val)< 0)
    {//for right right
        root = lr(root);
    }

    else if(balance > 1 && strcmp(root->leftside->phrase, val)< 0)
    {//for left right
        root->leftside = lr(root->leftside);
        root = rr(root);
    }
    else if(balance < -1 && strcmp(root->rightside->phrase,val) > 0)
    {//for right left
        root->rightside = rr(root->rightside);
        root = lr(root);
    }

    return root;
}

tree balance_tree(tree root)//balancing tree
{
    tree q, *w;
    int lefth,righth;//left and right heights
    lefth = get_height(root->leftside);
    righth = get_height(root->rightside);
    if(lefth >= righth)
        q = root->leftside;
    else
        q = root->rightside;
    lefth = get_height(q->leftside);
    righth = get_height(q->rightside);
    if(q == root->leftside)
    {
        if(lefth >= righth)
        {
            w = q->leftside;
        }
        else
            w = q->rightside;
    }
    if(q == root->rightside)
    {
        if(lefth > righth)
        {
            w = q->leftside;
        }
        else
            w = q->rightside;
    }

    if(root->leftside == q && q->leftside == w)
    {
        root = rr(root);
    }

    else if(q == root->rightside && q->rightside == w)
    {
        root = lr(root);
    }

    else if(q == root->leftside && w == q->rightside)
    {
        root->leftside = lr(root->leftside);
        root = rr(root);
    }
    else if(q == root->rightside && w == q->leftside)
    {
        root->rightside = rr(root->rightside);
        root = lr(root);
    }
    return root;
}
tree erase(tree root, char val[MS])
{
    //normal deletion
    if(!root)
        return NULL;
    if(strcmp(root->phrase,val) > 0)
    {
        root->leftside = erase(root->leftside, val);
    }
    else if(strcmp(root->phrase,val) < 0)
    {
        root->rightside = erase(root->rightside, val);
    }
    else
    {
        tree space;
        if(root->leftside == NULL || root->rightside == NULL)
        {
            if(root->leftside)
                space = root->leftside;
            else if(root->rightside)
                space = root->rightside;
            else
                space = NULL;
            root = NULL;
            free(root);
            return space;
        }
        else
        {

            space = inorder_succ_right_tree(root);

            strcpy(root->phrase, space->phrase);

            strcpy(root->synonym, space->synonym);
            strcpy(root->opposite, space->opposite);

            for (v = 0 ; v < root->mnumber; v ++)
            {
                strcpy(root->meanings[v],space->meanings[v]);
            }
            root->rightside = erase(root->rightside,space->phrase);
        }
    }
    if(root)
    {

        root->height = biggest(get_height(root->leftside), get_height(root->rightside)) + 1;//new heights
        int balance = get_balance(root);
        if(balance > 1 || balance < -1)
            root = balance_tree(root);
    }
    return root;
}

void sorted(tree root)
{//printing sorted tree
    if(!root)
        return;
    sorted(root->leftside);
    printf("%s :\n",root->phrase);
    for (v = 0 ; v < root->mnumber ; v++){
        printf("meaning the  :%d  is : %s\n",(v+1),root->meanings[v]);
    }
    printf("synonym is : %s\n",root->synonym);

    printf("antonym is : %s\n",root->opposite);//opposite = antonym

    sorted(root->rightside);
}

tree charErase(tree root, char val)
{
    tree space;
    //normal deletion
    if(!root)
        return NULL;
    if(root->phrase[0] >val)
    {
        root->leftside = charErase(root->leftside, val);
    }
    else if(root->phrase[0] <val )
    {
        root->rightside = charErase(root->rightside, val);
    }
    else
    {

        if(root->leftside == NULL || root->rightside == NULL)
        {
            if(root->leftside)
                space = root->leftside;
            else if(root->rightside)
                space = root->rightside;
            else
                space = NULL;
            root = NULL;
            free(root);
            return space;
        }
        else
        {
            space = inorder_succ_right_tree(root);
            strcpy(root->phrase, space->phrase);
            strcpy(root->synonym, space->synonym);
            strcpy(root->opposite, space->opposite);

            for (v = 0 ; v< root->mnumber; v ++)
            {
                strcpy(root->meanings[v],space->meanings[v]);
            }
            root->rightside = erase(root->rightside,space->phrase);
        }
    }
    if(root)
    {

        root->height = biggest(get_height(root->leftside), get_height(root->rightside)) + 1;
        int balance = get_balance(root);
        if(balance > 1 || balance < -1)
            root = balance_tree(root);
    }
    return root;
}
void preorder(tree root)
{//this function will print the tree on the file , without sorting
    if(!root)
        return;

    fprintf(out,"%s:",root->phrase);
    for (v= 0;v< root->mnumber ;v++)
    {
        if (v!= (root->mnumber- 1))//loop for meaning they may be more than 1
        {
            fprintf(out,"%s,",root->meanings[v]);
        }
        else
            fprintf(out,"%s",root->meanings[v]);
    }
    fprintf(out,"/%s*",root->synonym);//print synonym and antonym for the word on the file
    fprintf(out,"%s",root->opposite);

    preorder(root->leftside);
    preorder(root->rightside);
}

void psorted4(tree root,char g[MS])
{
    if(!root)
        return;
    psorted4(root->leftside,g);//the function prints out synonym and antonym
    if (strcmp(root->phrase, g) == 0)
        printf("antonym is %s , synonym is %s",root->opposite,root->synonym);
    psorted4(root->rightside,g);
}
void psorted2(tree root,char g)
{
    if(!root)
        return;
    psorted2(root->leftside,g);
    if (root->phrase[0] == g)
    {
        printf("%s ::\n",root->phrase);
        for (v = 0 ; v < root->mnumber ; v++){//for loop because there is one or more meanings
            printf("the meaning number ::%d  , : %s\n",(v+1),root->meanings[v]);
        }
        printf(" synonym is : %s\n",root->synonym);
        printf(" antonym is : %s\n",root->opposite);

    }
    psorted2(root->rightside,g);
}

int psorted3(tree root,char g[MS],int y)
{//search for a word , edit or not
      if(!root)
         return y;
         psorted3(root->leftside,g,y);
     if (strcmp( root->phrase, g) == 0)
    {


        int choice2;//ask user if he wants to edit?
        printf("please choose one of the choices \n");
        printf("1 ** update the information \n");
        printf("2 ** do not update anything\n");


        scanf("%d",&choice2);
        int choice3;
        char news[MS];
        char newm[MS];
         char newo[MS];

        switch(choice2){
            case 1 :

            printf("choose one of these \n");
            printf("1 ** change synonym\n");
            printf("2 ** add meaning \n");
            printf("3 ** change the antonym\n");
            scanf("%d",&choice3);
            switch(choice3){
                case 1 :

                printf("Changing synonym\n");
                printf("Pls enter your new synonym \n");

                scanf("%s",news);
                strcpy(root->synonym,news);
                break;
                case 2 :
                printf("Add meaning \n");

                scanf("%s",&newm);

                strcpy(root->meanings[root->mnumber],newm);
                root->mnumber++;
                case 3 :
                printf("Change antonym \n");
                printf("Pls enter your new antonym \n");

                scanf("%s",newo);
                strcpy(root->opposite,newo);
                break;




            }
            case 2 :
            return y;
            break;
            default :
            printf("Error in input \n");
            printf("the valid values are 1 , 2 ,3 \n");
            break;
        }





    }
    psorted3(root->rightside,g,y);
}

tree erasecahracter(tree root,char g,int *f)
{//erase words begins with same letter
    if(!root)
        return;
    erasecahracter(root->leftside,g,f);
    if (root->phrase[0] == g)
    {
        *f = 1;
        root = charErase(root,g);
    }
    erasecahracter(root->rightside,g,f);
    return root;
}
void readFile(int *);
const char * extractData();
const char * getWord(char *);
const char * getMeanings(char *);
const char * getSynonyms(char *);
const char * getOposites(char *);
int c= 0;
void getData(char g[][MS]);
void hashMenu();
int main()
{
    int counter =0;
    char word[MS];
    char meaning[MS];
    char synonym[MS];
    tree mm = NULL;
    char opposite[MS];
//main tree functions menu

    printf("Welcome\n");
    printf("Please choose one of the options from the Avl tree menu down below\n");

    printf("Hit enter every time to go back to main menu \n");
    printf("1  ** Read data From file (words.txt) \n ");
    printf("2  ** User add a new word to the dictionay \n");
    printf("3  ** List the words in lexicographic order of sorting \n");
    printf("4  ** Find a word from dictionary and edit it (if u wish :) \n");
    printf("5  ** List all words starts with the same first charecter(u can choose the character)\n");
    printf("6  ** List all synonyms and opposites (antonyms) of a given word \n");
    printf("7  ** Delete chosen word from the dictionary \n");
    printf("8  ** Delete all words starts with specific character(u can choose the character)\n");
    printf("9  ** Save the words to the file (dictionary.data)\n");
    printf("10 ** Go to Hash function \n");
    printf("0  ** EXIT \n");
    printf("Pls be sure just enter numbers and in [0-10]\n");
    int choice;
    scanf("%d",&choice);
     readFile(&counter);
    char phrase[counter][MS];
    while (choice != 0)
    {
        system("cls");//clear screen
        // upon on user choice do what is needed
        if (choice == 0){//exit
            system("cls");
            break;
        }
        if(choice == 1)
        {//read data from file

            printf("be sure the name of file is inputWords.txt \n");

            for (v = 0 ; v < counter ; v++)
            {
                        getData(phrase);//this function will save the data in an array of strings
            }
            for (v=0 ; v < counter ;v++)
            {
                //processes on the array of string in order to split it as the formula and save it in avl tree
                strcpy(word,getWord(phrase[v]));
                strcpy(meaning,getMeanings(phrase[v]));
                strcpy(synonym,getSynonyms(phrase[v]));
                strcpy(opposite,getOposites(phrase[v]));
                mm = putin(mm,word,meaning,synonym,opposite);
            }
            printf("hit enter to see menu\n");

        }

        if (choice == 2)
        {//add new word entered by user
             system("cls");
             int meaningnum;
             char meanWords[MS];
             meaning[0] = '\0';
            printf("New wrod \n");//asking the user for entering the elements
            printf("Pls enter the word to be added \n");
            scanf("%s",word);
            printf("how many meanings u want to add (number) \n");
            scanf("%d",&meaningnum);
            for (v = 0 ;v < meaningnum ; v++)
            {
                printf("enter meaning (its number %d ) ",(v+1));
                scanf("%s",meanWords);
                strcat(meaning,meanWords);
                strcat(meaning,",");
            }
            meaning[strlen(meaning) - 1] = '\0';
            printf("enter a synonym : ");
            scanf("%s",synonym);
            printf("enter an antonym : ");
            scanf("%s",opposite);
            mm = putin(mm,word,meaning,synonym,opposite);
             printf("hit enter \n");
        }
        if (choice == 3)
        {//print all words after sorting them lexicographic way

            sorted(mm);


              printf("hit enter to see menu\n");

        }
        if (choice == 4)
        {//search for word and edit it
            int z = 0;
            printf("enter a word : ");
            char g[MS];
            scanf("%s",g);
            z = psorted3(mm,g,z);
            if (z == 0){
                printf("sorry could not find word :( ");
            }
            printf("hit enter \n ");
        }
        if (choice == 5)
        {
            char opt;
            system("cls");
            printf("enter the char u want \n");
            fflush(stdin);
            scanf("%c",&opt);
            fflush(stdin);
            psorted2(mm,opt);
        }
        if (choice == 6)
        {//after user enters a word it will print out synonyms and antonyms of the word
            char g[MS];
            system("cls");
            printf("what is the word (Enter it)\n ");
            scanf("%s",g);
            fflush(stdin);
            psorted4(mm,g);
        }
        if (choice == 7)
        {//delete word
             char g[MS];
            system("cls");
            printf("deletion\n");
            printf("pls enter the word to be deleted \n");
            fflush(stdin);
            scanf("%s",g);
            fflush(stdin);
            mm = erase(mm,g);
            printf("finish\n");
        }
        if (choice == 8)
        {//after user enters a character all words begin with same chracter will be erased

            system("cls");
            char opt;
            printf("enter the character u chosen \n");
            fflush(stdin);
            scanf("%c",&opt);
            fflush(stdin);
            for (v = 0 ; v < counter; v++)
            {
                if (phrase[v][0] == opt)
                {
                char todelete[90];
                strcpy(todelete,phrase[v]);
                char *pp;
                pp = strtok(todelete,":");
                mm= erase(mm,pp);
                }
            }


            printf("finish \n");
            getchar();
            fflush(stdin);
        }
        if (choice == 9)
        {//save words to the file
           out = fopen("dictionary.data.txt","w");
            preorder(mm);
            fclose(out);
            printf("saved\n");
        }

          if (choice == 10)
        {//close this menu and enter the hash menu
            system("cls");
            break;
        }
        fflush(stdin);
        getchar();
        fflush(stdin);
        system("cls");
        printf("Welcome\n");
       //
    printf("Please choose one of the options from the Avl tree menu down below\n");
        printf("Hit enter every time to go back to main menu \n");

    printf("1  ** Read data From file (words.txt) \n ");
    printf("2  ** User add a new word to the dictionay \n");
    printf("3  ** List the words in lexicographic order of sorting \n");
    printf("4  ** Find a word from dictionary and edit it (if u wish :) \n");
    printf("5  ** List all words starts with the same first charecter(u can choose the character)\n");
    printf("6  ** List all synonyms and opposites (antonyms) of a given word \n");
    printf("7  ** Delete chosen word from the dictionary \n");
    printf("8  ** Delete all words starts with specific character(u can choose the character)\n");
    printf("9  ** Save the words to the file (dictionary.data)\n");
    printf("10 ** Go to Hash function \n");
    printf("0  ** EXIT \n");
    printf("Pls be sure just enter numbers and in [0-10]\n");
        scanf("%d",&choice);
        if (choice ==0)
            exit(1);
    }

    printf("hash \n");
   printf("Please chose one of the option \n");
       printf("Hit enter every time to go back to main menu \n");

   printf("1 ** Read data from file \n");
   printf("2 ** Print hash table \n");
   printf("3 ** add new record to hash table \n");
   printf("4 ** erase a record \n");
   printf("5 ** print out the size \n");
   printf("6 ** print hash function as string \n");
   printf("7 ** save hash in file \n");
   printf("0 ** EXIT\n");
    scanf("%d",&choice);
    fflush(stdin);
     int numberwords;
    int sizeh;//this represents the hash table size
    while (choice != 0)//if choice = 0 ,exit
    {
        if(choice == 1)
        {//read data from file
            readFile2(&numberwords);
            sizeh = getsizeh(numberwords);
            h = malloc(sizeof(hashTabel) * sizeh);
            for (v = 0 ;v < sizeh;v++)
            {

                h[v].clear = 0;
                strcpy(h[v].oword, " ");
                strcpy(h[v].synonym, " ");
                strcpy(h[v].opposite, " ");
                h[v].mnumber = 0;
            }
            char WORDS[numberwords][MS];
            getData2(WORDS);
            insertsavehfile(WORDS,numberwords,sizeh);
            printf("done \n");
            getchar();
            fflush(stdin);
        }
        if (choice == 2)
        {//print hash table e
            for (v = 0 ; v <sizeh; v++)
            {
                printf("%d %s ",v,h[v].oword);
                if (h[v].clear == 1)
                {
                    int j;
                    printf(" meanings : ");
                    for (j = 0 ; j < h[v].mnumber; j++)//for loop it may be more tthan 1 meaning
                        printf(" %s,",h[v].meanings[j]);
                    printf("   synonym : %s",h[v].synonym);
                    h[v].opposite[strlen(h[v].opposite) - 1] = '\0';
                    printf("   antonym : %s \n",h[v].opposite);
                    h[v].opposite[strlen(h[v].opposite) ] = '\n';
                }


            }
            fflush(stdin);
            getchar();
            fflush(stdin);
        }
        if (choice == 3)
        {//add word


                char word[MS],synonym[MS],meaning[MS],meanWords[MS],opposite[MS];
            printf("pls enter the word \n ");
            scanf("%s",word);
            printf("pls enter the meanings number ");
            int numberOfMeanings;
            scanf("%d",&numberOfMeanings);
            meaning[0] = '\0';
            for (v = 0 ; v < numberOfMeanings ; v++)
            {
                printf("enter meaning no %d : ",(v+1));
                scanf("%s",meanWords);
                strcat(meaning,meanWords);
                strcat(meaning,",");
            }
            meaning[strlen(meaning) - 1] = '\0';
            printf("enter a synonym  \n");
            scanf("%s",synonym);
            printf("enter an antonym  \n");
            scanf("%s",opposite);
            putinhash(word,meaning,synonym,opposite,&sizeh);
            printf("done,hit enter to see menu \n");
            getchar();
            fflush(stdin);

        }
        if (choice == 4)
        {//erase
            system("cls");

            char word[MS];
            printf("pls enter the word to be deleted \n ");

            scanf("%s",word);
            eraseWordFromHash(word,sizeh);
            getchar();
            fflush(stdin);
        }
        if (choice == 5)
        {//hash size
            printf("tabel size = %d",sizeh);
            fflush(stdin);
            getchar();
            fflush(stdin);
        }
        if (choice == 6)
        {

            printf(" the hash function is  (Sum(ASCII(st[a]*j)*pow(32,i)))%% sizeh\n");
            printf("sizeh = the size of the hash table\n");
            fflush(stdin);
            getchar();
            fflush(stdin);


        }
        if (choice == 7)
        {//save it back to file
            out = fopen("dictionary.data.txt","w");
            savehfile(sizeh);
            fclose(out);
        }
        system("cls");
        printf("hash \n");
        printf("Please chose one of the option \n");
            printf("Hit enter every time to go back to main menu \n");

        printf("1 ** Read data from file \n");
        printf("2 ** Print hash table \n");
        printf("3 ** add new record to hash table \n");
        printf("4 ** erase a record \n");
            printf("5 ** print out the size \n");
        printf("6 ** print hash function as string \n");
         printf("7 ** save hash in file \n");
        printf("0 ** EXIT\n");
        scanf("%d",&choice);
        if (choice == 0)
            break ;
    }
    return 0;
}
void savehfile(int sizeh)
{//save hash table to file
    int b;
    for (b = 0 ; b < sizeh ; b++)
    {
        if(h[b].clear == 1)
        {
            fprintf(out,"%s:",h[b].oword);
            int j;
            for (j = 0 ; j < h[b].mnumber ; j++)
            {
                if (j != (h[b].mnumber - 1))
                    fprintf(out,"%s,",h[b].meanings[j]);
                else
                    fprintf(out,"%s/",h[b].meanings[j]);
            }
            fprintf(out,"%s*",h[b].synonym);
            fprintf(out,"%s",h[b].opposite);
        }
    }
}
void insertsavehfile(char WORDS[][MS],int numberwords,int sizeh)
{//add data to hash table from file
    int i;

    char word[100];
    char meaning[100];
    char synonym[100];
    char opposite[100];
    for (i = 0 ; i < numberwords ; i++)
    {
        strcpy(word,getWord(WORDS[i]));
        strcpy(meaning,getMeanings(WORDS[i]));
        strcpy(synonym,getSynonyms(WORDS[i]));
        strcpy(opposite,getOposites(WORDS[i]));
        putinhash(word,meaning,synonym,opposite,&sizeh);
    }
}
int searchHash(char g[100],int sizeh)
{



    int j = 0;
    int place = placeinarr(g,sizeh,j);
    while (h[place] .clear != 0 )
    {
        if (strcmp(g,h[place].oword) == 0)
        {
            return place;
        }
    }
    return -1;
}
void getData(char g[][MS])
{//copy data to array of strings
    FILE *in;
   // printf("be sure the name of file is inputWords.txt \n");
    in = fopen("inputWords.txt","r");
    char p[250];
    int i = 0;
    while (fgets(p,250,in) != NULL)
    {
        strcpy(g[i],p);
        i++;
    }
}
int isPrime(int x)
{
    int aa;
    for (aa= 2 ; aa < x ; aa++)
    {
        if (x%aa == 0)
            return 0;
    }
    return 1;
}
void eraseWordFromHash(char g[100],int sizeh)
{
    int j = 1;
    int place = placeinarr(g,sizeh,j);
    while (h[place] .clear != 0 )
    {
        if (strcmp(g,h[place].oword) == 0)
        {
            h[place].clear = 0;
            strcpy(h[place].oword," ");
            h[place].mnumber = 0;
            strcpy(h[place].synonym," ");
            strcpy(h[place].opposite," ");
            break;
        }
        place = placeinarr(g,sizeh,++j);
    }
}

void getData2(char g[][MS])
{
    FILE *in;
    in = fopen("dictionary.data.txt","r");
    char p[MS];
    int i = 0;
    while (fgets(p,250,in) != NULL)
    {
        strcpy(g[i],p);
        i++;
    }
}

int getsizeh(int g)
{//calculate table size
    int f = g * 2;
    for(;;)
    {
        if (isPrime(++f))
            return f;//f=table size
    }
}

const char * getOposites(char g[MS])
{//for antonym
    char f[strlen(g)];
    strcpy(f,g);
    char *p;
    p = strtok(f,":");
    p = strtok(NULL,"/");
    p = strtok(NULL,"/");
    char k[strlen(p)];
    strcpy(k,p);
    char *pp;
    pp = strtok(k,"*");
    pp = strtok(NULL,"*");
    return pp;//pp=antonym
}
const char * getSynonyms(char g[MS])
{
    char f[strlen(g)];
    strcpy(f,g);
    char *p;
    p = strtok(f,":");
    p = strtok(NULL,"/");
    p = strtok(NULL,"/");
    char k[strlen(p)];
    strcpy(k,p);
    char *pp;
    pp = strtok(k,"*");
    return pp;
}
int placeinarr(char *key,int sizeh, int doesExist)
{
    unsigned long hValue=0;
    int i=0;//when collision occurs j increase
    while(key[i]!='\0') {
        hValue=(hValue<<5)+key[i]*doesExist;
        i++;
    }
    return hValue%sizeh;
}
const char * getMeanings(char g[MS])
{
    char f[strlen(g)];
    strcpy(f,g);
    char *p;
    p = strtok(f,":");
    p = strtok(NULL,"/");
    return p;
}
const char * getWord(char g[MS])
{
    char p[strlen(g)];
    strcpy(p,g);
    char *pp;
    pp = strtok(p,":");
    return pp;
}
void readFile2(int *counter)
{
    FILE *in;//read from dictionary.data.txt
    in = fopen("dictionary.data.txt","r");
    char g[MS];
    *counter = 0;
    while (fgets(g,250,in)!= NULL)
    {
        (*counter)++;
    }
    fclose(in);
}
void readFile(int *counter)
{
    FILE *in;
    in = fopen("inputWords.txt","r");
    char g[MS];
    *counter = 0;
    while (fgets(g,250,in)!= NULL)
    {
        (*counter)++;
    }
    fclose(in);
}


void putinhash(char word[MS],char meaning[MS],char synonym[MS],char opposite[MS], int *sizeh)
{//add record to hash table
    int counter = 0,a,j = 1;
    int sizeh1 = *sizeh;
    for (a = 0 ; a <sizeh1 ; a++)
    {
        if (h[a].clear == 1)
        {
            counter++;
        }
    }
    if ((counter+1)*2 > sizeh1)
    {
        rehash(sizeh);
        for (a = 0 ; a < *sizeh ; a++)
            if (strcmp(h[a].oword," ") == 0)
                h[a].clear = 0;
    }
    int place = placeinarr(word,*sizeh,j);
    while (h[place] .clear != 0 )
    {
        place = placeinarr(word,*sizeh,++j);
    }
    h[place].clear = 1;
    strcpy(h[place] .oword,word);
    h[place].mnumber = 0;
    strcpy(h[place].synonym,synonym);
    strcpy(h[place].opposite,opposite);
    char mm[strlen(meaning)];
    strcpy(mm,meaning);
    char *p;
    p = strtok(mm,",");
    strcpy(h[place].meanings[0],p);
    h[place].mnumber++;
    while (p!= NULL)
    {
        p = strtok(NULL,",");
        if(p!= NULL)
        {
            strcpy(h[place].meanings[h[place].mnumber], p);
            h[place].mnumber++;
        }
    }
}

void putinhash2(hashTabel x,hashTabel hh[],int tabelsize)
{
    int j = 0;
    int pos = placeinarr(x.oword,tabelsize,j);
    while (hh[pos] .clear != 0 )
    {
        pos = placeinarr(x.oword,tabelsize,++j);
        pos = (pos % tabelsize);
    }
    hh[pos].clear = 1;
    strcpy(hh[pos].oword,x.oword);
    int i;
    for ( i = 0 ; i < x.mnumber; i++)
        strcpy(hh[pos].meanings[i],x.meanings[i]);
    hh[pos].mnumber = x.mnumber;
    strcpy(hh[pos].synonym,x.synonym);
    strcpy(hh[pos].opposite,x.opposite);
}
void rehash(int *oldSize)
{//hash the hashed table one more time
    int a;
    getchar();
    int newSize = getsizeh(*oldSize);
    hashTabel *hh = malloc(sizeof(hashTabel) * newSize);
    for (a = 0 ; a < newSize ; a++)
    {
        strcpy(hh[a].oword," ");
        hh[a].clear = 0;
        hh[a].mnumber = 0;
        strcpy(hh[a].synonym," ");
        strcpy(hh[a].opposite," ");
    }
    for (a=0;a< *oldSize ;a++)
    {
        if (h[a].clear== 1)
        {
            putinhash2(h[a],hh,newSize);
        }
    }
    h = hh;
    *oldSize = newSize;
}
