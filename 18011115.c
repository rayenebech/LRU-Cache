#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <locale.h>
#define MAX_LENGTH 10

struct node {
  char id[MAX_LENGTH]; 
  char name[MAX_LENGTH];
  char surname[MAX_LENGTH];
  int birth_year;
  char address[MAX_LENGTH];
  struct node* next;
};
typedef struct node node;


typedef struct hash_table{
	int id;
	char TC[MAX_LENGTH];
	int order;
	int reserved;
} hash_table;

node*  user_interface(node* cache_head, int* cache_left, int array_size, hash_table* array);
int horner(char* str);
int findSize(int N);
void readCache(node** head, int order);
void printCache(node* head);
int sreach_in_hash_table(int id, hash_table* array, int m);
void print_hash_table(hash_table* harray, int array_size);
int readFile(node* newPerson,char TC[MAX_LENGTH]);
void increment_order(hash_table* array, int array_size);
char* cache_delete_last(node* cache_head);
void delete_from_harray(int id_deleted,hash_table* array, int array_size);
node* add_first_node(node* cache_head,node* newPerson);

int main() {
	setlocale(LC_ALL, "Turkish");
	printf("Merhaba, Dünya!");
	node* cache_head = NULL; 
	hash_table* harray;
	int i,cache_size, array_size;
	printf("\nPlease enter the cache size: ");
	scanf("%d",&cache_size);
	array_size = findSize(cache_size);
	harray = (hash_table*) malloc(sizeof(hash_table) * array_size);
	for(i=0;i< array_size; i++){
		harray[i].id= -1;
		harray[i].reserved=0;
	}
	printf("\nPrinting hash table: \n");
	print_hash_table(harray,array_size);
	while(1==1){
		cache_head = user_interface(cache_head,&cache_size,array_size,harray);
	}
	free(cache_head);
	free(harray);
  return 0;
}


node* user_interface(node* cache_head, int* cache_left, int array_size, hash_table* array){
	char TC[MAX_LENGTH], TC_deleted[MAX_LENGTH];
	int id, index, id_deleted; 
	node* newPerson,*p;
	
	printf("\nPlease enter the TC number of the person: ");
	scanf("%s",&TC);
	id = horner(TC);
	index = sreach_in_hash_table(id,array,array_size);
	if (index == -1){
		return;
	}
	else if(array[index].id == -1 || array[index].reserved == 0){
		printf("\nValue does not exist in the hash table so we will add it to both the cache and the table\n ");
		/*Create a new node*/
		newPerson = (node*) malloc(sizeof(node));
		newPerson->next= NULL;
		if (readFile(newPerson,TC) ==-1){
			return;
		}
		printf("\nInformation of the newPerson: \n");
		printf("%s, %s, %s, %d, %s",newPerson->id,newPerson->name,newPerson->surname, newPerson->birth_year,newPerson->address);
		printf("\nNow let's add it to the cache \n");
		/*check if the cache is full*/
		if(*cache_left == 0){
			printf("\nThe cache is full so delete the last one");
			/*if full: delete last element from the cache and from the table */
			strcpy(TC_deleted,cache_delete_last(cache_head));
			if(strcmp(TC_deleted,"-1") ==0){
				return;
			}
			printf("\n The Id to be delted is: %s", TC_deleted);
			id_deleted= horner(TC_deleted);
			delete_from_harray(id_deleted,array,array_size);
		}
		else{
			printf("\nChache is not full and its size now is: %d",*cache_left);
			*cache_left = *cache_left -1;
		}
		/*add at begining of the cache */ 
		printf("\nAdd at the begining of the cache:\n");
		cache_head= add_first_node(cache_head,newPerson);
		printCache(cache_head);
		/*add at at index for the hash table*/
		increment_order(array,array_size);
		array[index].id= id;
		strcpy(array[index].TC,TC);
		array[index].order= 0;
		array[index].reserved=1;
		print_hash_table(array,array_size);
				
	}
	else if(array[index].id == id ){
		printf("The value already exists, reading from cache: \n");
		if(array[index].reserved){
			/*Value exists so read it and update it*/

			readCache(&cache_head,array[index].order);
			if(array[index].order !=0){
				increment_order(array,array_size);
				array[index].order= 0;
			}
			printf("\nUpdating the hashtable: \n");
			print_hash_table(array,array_size);
			printf("\nUpdating the cache:\n");
			printCache(cache_head);
		}
		else{
			/*value was deleted so insert it again?*/
		}
		
	}
	return cache_head;

}
void increment_order(hash_table* array, int array_size){
	int i;
	for(i=0;i<array_size;i++){
		array[i].order ++;
	}
}
char* cache_delete_last(node* cache_head){
	node *q ,*p= cache_head;
	char ID[MAX_LENGTH];
	if (p==NULL){
		printf("\nError: Attempt to delete while the cache is empty! \n");
		return "-1";
	}
	while(p->next != NULL){
		q=p;
		p=p->next;
	}
	strcpy(ID, p->id);
	q->next= NULL;
	return ID;
}
void delete_from_harray(int id_deleted,hash_table* array, int array_size){
	int i=0, found=0;
	while(i< array_size & !found){
		if(id_deleted==array[i].id){
			found=1;
			array[i].reserved=0;
		}
		i++;

	}
}
node*  add_first_node(node* cache_head,node* newPerson){
	if(cache_head == NULL){
		printf("\ncache is empty so newPerson will be the head\n");
		cache_head= newPerson;
		return cache_head;
	}
	printf("\nCache is not empty\n");
	node *p = cache_head->next; 
	newPerson->next= cache_head;
	(newPerson->next)->next= p;
	return newPerson;
}

void readCache(node** head, int order){
	node *previous, *next, *p= *head;
	int i;
	for(i=0;i<order;i++ ){
		previous=p;
		p = p->next;
	}
	printf("id: %s name %s, surname: %s birthyear %d address: %s", p->id, p->name,p->surname,p->birth_year,p->address);
	if(order!=0){
		/*change order to the first*/
		next=p->next;
		previous->next = next;
		p->next= *head;
		*head= p;
	}
	
}
void printCache(node* head){
	node* p = head;
	if(p==NULL){
		printf("\nThe cache is empty!\n");
		return;
	}
	printf("\nPrinting the cache: \n");
	while (p!=NULL)
	{
		printf("\nid: %s name %s, surname: %s birthyear %d address: %s\n|\n", p->id, p->name,p->surname,p->birth_year,p->address);
		p=p->next;
	}
	

}

int horner(char str[MAX_LENGTH]){
	int hash_value=0;
	int i=0, size = strlen(str);
	while(str[i]!= NULL){
		hash_value = hash_value + pow(31,size-i-1)*(str[i]- '0');
		i++;
	}
	return hash_value;	
}

int sreach_in_hash_table(int id, hash_table* array, int m){
	int i=0,j, h1, h2;
	h1 = id % m;
	/*for i=0 j = h1*/
	j=h1;
	while(i<m && array[j].id != id && array[j].id != -1 && array[j].id != id){
		h2 = 1 + (id % (m-1));
		j = (h1 + i * h2) %m;
		i++;
	}
	if(i==m){
		printf("Value does not exist and hashtable is full");
		return -1;
	}
	else{
		return j;
	} 	
}

int findSize(int N){
	int i,d, prime=0;
	d= N / 0.6; /* 0.6 is the load factor*/
	while(!prime){
		i=2;
		while(i < sqrt(d) && d%i !=0) i++;
		if(d%i ==0) d++;
		else prime=1;
	}
	return d;	
}
void print_hash_table(hash_table* harray, int array_size){
	int i;
	printf("TC | order | reserved\n");
	for(i=0;i<array_size;i++){
		printf("%s |%d | %d\n",harray[i].TC, harray[i].order, harray[i].reserved);
	}
}

int readFile(node* newPerson,char TC[MAX_LENGTH]){
	char line[256],line2[256], file_TC[MAX_LENGTH];
	int found =0;
	FILE *fp;
	fp = fopen("test2.txt", "r");
	if (fp == NULL){
		printf("The file does not exist");
		return -1;
	}
	while(fgets(line, sizeof(line),fp) && !found){
		strcpy(line2,line);
		sscanf(line, "%s ", &file_TC);

		if(strcmp(TC, file_TC)==0){

			found=1;
		}
	}
	if(found){
		sscanf(line2, "%s %s %s %d %s", &newPerson->id, &newPerson->name, &newPerson->surname, &newPerson->birth_year, &newPerson->address );
		newPerson->next=NULL;
	}
	else{
		printf("\nThis ID does not exist");
		return -1;
	}
	fclose(fp);
	return 0;
}
