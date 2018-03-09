/**
 * comp2129 - assignment 2
 * justin harding
 * 450221916
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <stdbool.h>

#include "snapshot.h"

entry* entry_head = NULL;
entry* entry_tail = NULL;

snapshot* snapshot_head = NULL;
snapshot* snapshot_tail = NULL;

int main(void) {

	char line[MAX_LINE];

	entries_init();
	snapshots_init();

	while (true) {
		printf("> ");

		if (fgets(line, MAX_LINE, stdin) == NULL)
			command_bye();

		if(!strcmp(line,"\n"))
			continue;

		// remove new line character from end of input string
		char *input = strtok(line,"\n");

		check_command(input);
	}
	return 0;
}

void command_bye () {

	printf("bye\n");
	exit(0);
}

void command_help() {
	printf("%s\n", HELP);
}

/*
 *check if command exists and refer to corresponding function if it does
 */
void check_command(char *line) {

	char cmd[MAX_COMMAND];
	char key[MAX_KEY];
	size_t length=0;
	size_t size=10;

	char *token = strtok(line," ");
	int *value = (int*)malloc(size * sizeof(int));
	char **keys = (char**)malloc(size * sizeof(char*));

	if (!strcasecmp(token,"BYE")) {

		entry *current_entry = entry_head->next;
		snapshot *current_snapshot = snapshot_head->next;

		while(current_entry!=entry_tail) {
			entry *temp = current_entry;
			current_entry = current_entry->next;
			current_entry->prev = entry_head;
			free(temp->values);
			free(temp);
		}
		while (current_snapshot!=snapshot_tail) {
			if (current_snapshot->entries!=NULL) {
				entry *first_entry = current_snapshot->entries;
				if (first_entry->next!=NULL) {
					entry *current_entry = first_entry->next;
					while (current_entry->next!=NULL) {
						entry *temp = current_entry;
						current_entry = current_entry->next;
						free(temp->values);
						free(temp);
					}
					free(current_entry->values);
					free(current_entry);
				}
				free(current_snapshot->entries->values);
				free(current_snapshot->entries);
			}
			snapshot *s_temp = current_snapshot;
			current_snapshot = current_snapshot->next;
			current_snapshot->prev = snapshot_head;
			free(s_temp);
		}
		free(value);
		free(keys);
		free(entry_head);
		free(entry_tail);
		free(snapshot_head);
		free(snapshot_tail);
		command_bye();
	}

	if (!strcasecmp(token,"HELP")) {
		command_help();
		free(value);
		free(keys);
		return;
	}

	if (!strcasecmp(token,"SNAPSHOT")) {
		snapshot_add();
		free(value);
		free(keys);
		return;
	}

	strncpy(cmd,token,MAX_COMMAND);
	token = strtok(NULL, " ");
	strncpy(key,token,MAX_KEY);
	token = strtok(NULL, " ");

	if (!strcasecmp(cmd,"GET")) {
		if (get_values(key))
			printf("\n");
		free(value);
		free(keys);
		return;
	}

	if (!strcasecmp(cmd,"DEL")) {
		entry_delete_key(key);
		free(value);
		free(keys);
		return;
	}
	if (!strcasecmp(cmd,"PURGE")) {
		entry_purge(key);
		free(value);
		free(keys);
		return;
	}
	if (!strcasecmp(cmd,"LIST")&&!strcasecmp(key,"KEYS")) {
		list_keys();
		free(value);
		free(keys);
		return;
	}
	if (!strcasecmp(cmd,"LIST")&&!strcasecmp(key,"ENTRIES")) {
		list_entries();
		free(value);
		free(keys);
		return;
	}
	if (!strcasecmp(cmd,"LIST")&&!strcasecmp(key,"SNAPSHOTS")) {
		list_snapshots();
		free(value);
		free(keys);
		return;
	}
	if (!strcasecmp(cmd,"POP")) {
		pop_value(key);
		free(value);
		free(keys);
		return;
	}
	if (!strcasecmp(cmd,"MIN")) {
		min_value(key);
		free(value);
		free(keys);
		return;
	}
	if (!strcasecmp(cmd,"MAX")) {
		max_value(key);
		free(value);
		free(keys);
		return;
	}
	if (!strcasecmp(cmd,"SUM")) {
		sum_value(key);
		free(value);
		free(keys);
		return;
	}
	if (!strcasecmp(cmd,"LEN")) {
		len_value(key);
		free(value);
		free(keys);
		return;
	}
	if (!strcasecmp(cmd,"REV")) {
		rev_value(key);
		free(value);
		free(keys);
		return;
	}
	if (!strcasecmp(cmd,"UNIQ")) {
		if (uniq_value(key))
			printf("ok\n\n");
		else
			printf("no such key\n\n");
		free(value);
		free(keys);
		return;
	}
	if (!strcasecmp(cmd,"SORT")) {
		if (sort_value(key))
			printf("ok\n\n");
		else
			printf("no such key\n\n");
		free(value);
		free(keys);
		return;
	}
	if (!strcasecmp(cmd,"DROP")) {
		snapshot_delete(atoi(key));
		free(value);
		free(keys);
		return;
	}
	if (!strcasecmp(cmd,"ROLLBACK")) {
		snapshot_rollback(atoi(key));
		free(value);
		free(keys);
		return;
	}
	if (!strcasecmp(cmd,"CHECKOUT")) {
		snapshot_checkout(atoi(key));
		free(value);
		free(keys);
		return;
	}
	if (!strcasecmp(cmd,"DIFF")||!strcasecmp(cmd,"INTER")||!strcasecmp(cmd,"UNION")) {
		char firstkey[MAX_KEY];
		strcpy(firstkey,key);

		for (; token; length++) {

		// check if run out of space in keys array
			if (length==size-3) {
				size = size*2;
				keys = (char**)realloc(keys,size * sizeof(char*));
			}
			strcpy(key,token);
			keys[length] = key;
			token = strtok(NULL, " ");
		}

		if (!strcasecmp(cmd,"DIFF"))
			diff_value(firstkey,keys,length);

		if (!strcasecmp(cmd,"INTER"))
			inter_value(firstkey,keys,length);

		if (!strcasecmp(cmd,"UNION"))
			union_value(firstkey,keys,length);

		free(value);
		free(keys);
		return;
	}
	// take rest of tokens into value array
	for (; token; length++) {

		// check if run out of space in value array
		if (length==size-3) {
			size = size*2;
			value = (int*)realloc(value,size * sizeof(int));
		}
		value[length] = atoi(token);
		token = strtok(NULL, " ");
	}

	if (!strcasecmp(cmd,"SET")) {

		entry* node = find_key(key);

		if (node==NULL) {
			printf("ok\n\n");
			entry_add(key,value,length);
			free(value);
			free(keys);
			return;
		}
		entry_edit(node,key,value,length);
		free(value);
		free(keys);
		return;
	}

	if (!strcasecmp(cmd,"PUSH")) {
		push_values(key,value,length);
		free(value);
		free(keys);
		return;
	}
	if (!strcasecmp(cmd,"APPEND")) {
		append_values(key,value,length);
		free(value);
		free(keys);
		return;
	}
	if (!strcasecmp(cmd,"PICK")) {
		pick_value(key,value[0]);
		free(value);
		free(keys);
		return;
	}
	if (!strcasecmp(cmd,"PLUCK")) {
		pluck_value(key,value[0]);
		free(value);
		free(keys);
	}
}

/*
 * initializes entries with head and tail sentinel nodes
 */
void entries_init() {

	entry_head = (entry*)malloc(sizeof(entry));
	entry_tail = (entry*)malloc(sizeof(entry));
	entry_head->next = entry_tail;
	entry_head->prev = entry_head;
	entry_tail->next = entry_tail;
	entry_tail->prev = entry_head;
}

/*
 * initializes snapshots
 */
void snapshots_init() {

	snapshot_head = (snapshot*)malloc(sizeof(snapshot));
	snapshot_tail = (snapshot*)malloc(sizeof(snapshot));
	snapshot_head->next = snapshot_tail;
	snapshot_head->prev = snapshot_head;
	snapshot_tail->next = snapshot_tail;
	snapshot_tail->prev = snapshot_head;
}

/*
 * adds new entry
 */
void entry_add(char *key, int *value, size_t length) {

	entry* new_entry = create_entry(key,value,length);

	// changes pointers such that new_entry is right after head
	new_entry->next = entry_head->next;
	new_entry->prev = entry_head;

	entry_head->next->prev = new_entry;
	entry_head->next = new_entry;
}

/*
 * creates a new entry
 */
entry* create_entry(char *key, int *value, size_t length) {

	// makes room for new entry
	entry* new_entry = (entry*)malloc(sizeof(entry));

	strcpy(new_entry->key,key);
	new_entry->length = length;

	// makes room for new entry's values
	new_entry->values = (int*)malloc((sizeof(int)*length));
	if (sizeof(value)>0) {
		// copy given values to new entry's value int array
		for (int i = 0; i < length; i++)
			new_entry->values[i] = value[i];

	}
	return new_entry;
}

/*
 * used to edit values in already existing key
 */
void entry_edit(entry *node, char *key, int *value, size_t length) {

	node->values = (int*)realloc(node->values,(sizeof(int)*(length)));
	node->length = length;
	for (int i = 0; i < length; i++)
		node->values[i] = value[i];

	printf("ok\n\n");
}

/*
 * adds new snapshot
 */
void snapshot_add() {

	int id = 1;
	snapshot* current_snapshot = snapshot_head;

	for ( ; current_snapshot->next!=snapshot_tail; id++)
		current_snapshot = current_snapshot->next;

	snapshot* new_snapshot = create_snapshot(id);
	new_snapshot->dropped = false;

	// changes pointers such that new_snapshot is right before tail
	new_snapshot->next = snapshot_tail;
	new_snapshot->prev = snapshot_tail->prev;

	snapshot_tail->prev->next = new_snapshot;
	snapshot_tail->prev = new_snapshot;

	// creates copy of entries in current state
	entry *current_entry = entry_head;

	if (current_entry->next==entry_tail) {
		printf("saved as snapshot %d\n\n",id);
		return;
	}

	current_entry = entry_tail->prev;
	new_snapshot->entries = create_entry(current_entry->key,current_entry->values,current_entry->length);
	entry *current_snapshot_entry = new_snapshot->entries;
	new_snapshot->entries->prev = new_snapshot->entries;

	while (current_entry->prev!=entry_head) {
		current_entry = current_entry->prev;
		entry *new_snapshot_entry = create_entry(current_entry->key,current_entry->values,current_entry->length);
		current_snapshot_entry->next = new_snapshot_entry;
		new_snapshot_entry->prev = current_snapshot_entry;
		current_snapshot_entry = current_snapshot_entry->next;
	}
	current_snapshot_entry->next = NULL;
	printf("saved as snapshot %d\n\n",id);
}

/*
 * creates a new snapshot
 */
snapshot* create_snapshot(int id) {

	// makes room for new snapshot
	snapshot* new_snapshot = (snapshot*)malloc(sizeof(snapshot));

	new_snapshot->id = id;
	new_snapshot->entries = NULL;

	return new_snapshot;
}

/*
 * restores current state to snapshot and deletes newer snapshots
 */
void snapshot_rollback(int id) {

	snapshot_checkout(id);

	snapshot *node = find_snapshot(id);
	if (node==NULL)
		return;
	while (node->next!=snapshot_tail) {
		node = node->next;
		node->dropped = true;
	}
}

/*
 * replaces current state with copy of snapshot
 */
void snapshot_checkout(int id) {

	snapshot *node = find_snapshot(id);

	if (node==NULL||node->dropped) {
		printf("no such snapshot\n\n");
		return;
	}
	// delete all entries in current state
	while(entry_head->next!=entry_tail)
		entry_delete_node(entry_head->next);

	entry *temp = node->entries;
	if (temp==NULL) {
		printf("ok\n\n");
		return;
	}
	// fill current state with copy of entries from snapshot
	entry_add(temp->key,temp->values,temp->length);
	while (temp->next!=NULL) {
		temp = temp->next;
		entry_add(temp->key,temp->values,temp->length);
	}
	printf("ok\n\n");
}

/*
 * finds snapshot using given id
 */
snapshot* find_snapshot(int id) {

	snapshot *current = snapshot_head;

	while(current->next!=snapshot_tail) {
		current = current->next;

		if (id==current->id)
			return current;
	}
	return(NULL);
}

/*
 * deletes snapshot by id
 */
void snapshot_delete(int id) {

	snapshot* current = snapshot_head;

	while(current->next!=snapshot_tail) {
		current = current->next;

		if (id == current->id) {
			current->dropped = true;
			printf("ok\n\n");
			return;
		}
	}
	printf("no such snapshot\n\n");
}

/*
 * deletes entry from current state using key
 */
void entry_delete_key(char *key) {

	entry* node = find_key(key);

	if (node==NULL) {
		printf("no such key\n\n");
		return;
	}
	entry* temp = node;

	temp->prev->next = temp->next;
	temp->next->prev = temp->prev;

	free(temp->values);
	free(temp);
	printf("ok\n\n");
}

/*
 * deletes given entry
 */
void entry_delete_node(entry *node) {

	if (node->prev!=node)
		node->prev->next = node->next;
	if (node->next!=NULL) {
		if (node->prev!=node) {
			node->next->prev = node->prev;
		}	else {
			node->next->prev = node->next;
		}
	}
	free(node->values);
	free(node);
}

/*
 * deletes entry from current state and snapshots
 */
void entry_purge(char *key) {

	snapshot *current_snapshot = snapshot_head;

	while (current_snapshot->next!=snapshot_tail) {
		current_snapshot = current_snapshot->next;
		if (current_snapshot->dropped)
			continue;

		if (current_snapshot->entries!=NULL) {
			if (!strcmp(key,current_snapshot->entries->key)) {

				entry *temp = current_snapshot->entries;
				current_snapshot->entries = current_snapshot->entries->next;
				entry_delete_node(temp);

				if (current_snapshot->entries==NULL) {
					free(current_snapshot->entries);
				}
				continue;
			}
			entry *current_entry = current_snapshot->entries;
			while (current_entry->next!=NULL) {
				current_entry = current_entry->next;
				if (!strcmp(key,current_entry->key)) {
					entry_delete_node(current_entry);
					break;
				}
			}
		}
	}
	entry *node = find_key(key);

	printf("ok\n\n");

	if (node==NULL)
		return;
	entry_delete_node(node);
}

/*
 * displays an entry's values
 */
bool get_values(char *key) {

	entry* node = find_key(key);

	if (node==NULL) {
		printf("no such key\n\n");
		return false;
	}
	// check if values empty
	if (node->length==0) {
		printf("[]\n");
		return true;
	}
	printf("[");
	for (int i = 0; i < node->length-1; i++)
		printf("%d ",node->values[i]);

	printf("%d]\n",node->values[node->length-1]);
	return true;
}

 /*
  * finds node containing given key
  */
entry *find_key(char *key) {

	entry* current = entry_head;

	while(current->next!=entry_tail) {
		current = current->next;
		if (!strcmp(key,current->key))
			return current;
	}
	return(NULL);
}

/*
 * displays all keys in current state
 */
void list_keys() {

	entry* current = entry_head;

	if (current->next==entry_tail) {
		printf("no keys\n\n");
		return;
	}
	while(current->next!=entry_tail) {
		printf("%s\n",current->next->key);
		current = current->next;
	}
	printf("\n");
}

/*
 * displays all keys and matching values in current state
 */
void list_entries() {

	entry* current = entry_head;

	if (current->next==entry_tail) {
		printf("no entries\n\n");
		return;
	}
	while(current->next!=entry_tail) {
		printf("%s ",current->next->key);
		get_values(current->next->key);
		current = current->next;
	}
	printf("\n");
}

/*
 * display all snapshots in the database
 */
void list_snapshots() {

	snapshot *current = snapshot_tail;
	bool no_snapshots = true;

	while (current->prev!=snapshot_head) {
		current = current->prev;

		if (!current->dropped) {
			printf("%d\n",current->id);
			no_snapshots = false;
		}
	}
	if (no_snapshots)
		printf("no snapshots\n");

	printf("\n");
}

/*
 * pushes given values to front
 */
void push_values(char *key, int *value, size_t length) {

	entry* node = find_key(key);

	if (node==NULL) {
		printf("no such key\n\n");
		return;
	}
	// store old values
	int *old_values = malloc(sizeof(int)*node->length);
	memcpy(old_values,node->values,sizeof(int)*node->length);

	node->values = (int*)realloc(node->values,(sizeof(int)*(node->length+length)));

	// insert new values first
	for (int i = 0;i < length;i++)
		node->values[i] = value[length-i-1];

	// then insert old values afterwards
	for (int j = length;j < node->length+length;j++)
		node->values[j] = old_values[j-length];

	node->length = node->length+length;
	printf("ok\n\n");
	free(old_values);
}

/*
 * appends given values to end
 */
void append_values(char *key, int *value, size_t length) {

	entry* node = find_key(key);

	if (node==NULL) {
		printf("no such key\n\n");
		return;
	}
	// store old values
	int *old_values = malloc(sizeof(int)*node->length);
	memcpy(old_values,node->values,sizeof(int)*node->length);

	node->values = (int*)realloc(node->values,(sizeof(int)*(node->length+length)));

	// insert new values after old
	for (int j = node->length; j < node->length+length; j++)
		node->values[j] = value[j-node->length];

	node->length = node->length+length;
	printf("ok\n\n");
	free(old_values);
}

/*
 * displays value at given index
 */
void pick_value(char *key, int index) {

	entry* node = find_key(key);

	if (node==NULL) {
		printf("no such key\n\n");
		return;
	}
	if (index>node->length||index<1) {
		printf("index out of range\n\n");
		return;
	}
	printf("%d\n\n",node->values[index-1]);
}

/*
 * displays and removes value at index
 */
void pluck_value(char *key,int index) {

	entry* node = find_key(key);

	if (node==NULL) {
		printf("no such key\n\n");
		return;
	}
	if (index>node->length||index<1) {
		printf("index out of range\n\n");
		return;
	}
	printf("%d\n\n",node->values[index-1]);

	// shift all elements after index down by 1 after reading element at index
	for (int i = index-1; i < node->length-1; i++)
		node->values[i] = node->values[i+1];

	// subtract 1 from values length since element removed
	if (node->length > 0)
		node->length = node->length-1;
}

/*
 * displays and removes the front value
 */
void pop_value(char *key) {

	entry* node = find_key(key);

	if (node==NULL) {
		printf("no such key\n\n");
		return;
	}
	if (node->length==0) {
		printf("nil\n\n");
		return;
	}
	printf("%d\n\n",node->values[0]);

	// shift all elements down by 1
	for (int i = 0; i < node->length-1; i++)
		node->values[i] = node->values[i+1];

	// subtract 1 from values length since element removed
	if (node->length > 0)
		node->length = node->length-1;
}

/*
 * displays minimum value
 */
void min_value(char *key) {

	entry* node = find_key(key);

	if (node==NULL) {
		printf("no such key\n\n");
		return;
	}
	if (node->length==0) {
		printf("nil\n\n");
		return;
	}
	int min = node->values[0];

	for (int i = 0; i < node->length; i++) {
		if (node->values[i]<min)
			min = node->values[i];
	}
	printf("%d\n\n",min);
}

/*
 * displays maximum value
 */
void max_value(char *key) {

	entry* node = find_key(key);

	if (node==NULL) {
		printf("no such key\n\n");
		return;
	}
	if (node->length==0) {
		printf("nil\n\n");
		return;
	}
	int max = node->values[0];

	for (int i = 0; i < node->length; i++) {
		if (node->values[i]>max)
			max = node->values[i];
	}
	printf("%d\n\n",max);
}

/*
 * displays sum of values
 */
void sum_value(char *key) {

	entry* node = find_key(key);

	if (node==NULL) {
		printf("no such key\n\n");
		return;
	}
	if (node->length==0) {
		printf("0\n\n");
		return;
	}
	int sum = node->values[0];

	for (int i = 1; i < node->length; i++)
		sum = sum + node->values[i];

	printf("%d\n\n",sum);
}

/*
 * display number of values
 */
void len_value(char *key) {

	entry* node = find_key(key);

	if (node==NULL) {
		printf("no such key\n\n");
		return;
	}
	if (node->length==0) {
		printf("0\n\n");
		return;
	}
	printf("%zu\n\n",node->length);
}

/*
 * reverses order of values
 */
void rev_value(char *key) {

	entry* node = find_key(key);

	if (node==NULL) {
		printf("no such key\n\n");
		return;
	}
	if (node->length==0) {
		printf("ok\n\n");
		return;
	}
	for (int i = 0; i < node->length / 2; i++) {
		int temp = node->values[i];
		node->values[i] = node->values[node->length - i - 1];
		node->values[node->length - i - 1] = temp;
	}
	printf("ok\n\n");
}

/*
 * removes repeated adjacent values
 */
bool uniq_value(char *key) {

	entry* node = find_key(key);

	if (node==NULL)
		return false;

	if (node->length==0)
		return true;

	for (int i = 0; i < node->length-1; i++) {
		if (node->values[i]==node->values[i+1]) {
			for (int j = i; j < node->length-1; j++)
				node->values[j] = node->values[j+1];

			if (node->length > 0)
				node->length = node->length-1;
			i--;
		}
	}
	return true;
}

/*
 * sort values in ascending order
 */
 bool sort_value(char *key) {

	entry* node = find_key(key);

	if (node==NULL)
		return false;

	for (int i = 0; i < node->length-1; i++) {
		for (int j = 0; j < node->length-i-1; j++) {
			if (node->values[j]>node->values[j+1]) {
				int temp = node->values[j+1];
				node->values[j+1] = node->values[j];
				node->values[j] = temp;
			}
		}
	}
	return true;
}

/*
 * finds symmetrical set difference of values in keys
 *
 * next three functions are ugly because I'm running out of time
 */
void diff_value(char *key,char **keys,size_t length) {

	entry *first_key_node = find_key(key);
	if (first_key_node==NULL) {
		printf("no such key\n\n");
		return;
	}
	int result_length = first_key_node->length;
	int result[result_length];
	for (int i = 0; i < result_length; i++)
		result[i] = first_key_node->values[i];

	// sort first key
	for (int m = 0; m < result_length-1; m++) {
		for (int n = 0; n < result_length-m-1; n++) {
			if (result[n]>result[n+1]) {
				int temp = result[n+1];
				result[n+1] = result[n];
				result[n] = temp;
			}
		}
	}
	// uniq first key
	for (int i = 0; i < result_length-1; i++) {
		if (result[i]==result[i+1]) {
			for (int j = i; j < result_length-1; j++)
				result[j] = result[j+1];

			if (result_length > 0)
				result_length = result_length-1;
			i--;
		}
	}
	// iterate through keys
	for (size_t j = 0; j < length; j ++) {
		entry *current = find_key(keys[j]);
		if (current==NULL) {
			printf("no such key\n\n");
			return;
		}
		int temp_length = current->length;
		int temp[temp_length];
		for (int i = 0;i < temp_length;i++)
			temp[i] = current->values[i];
		// sort second key
		for (int m = 0; m < temp_length-1; m++) {
			for (int n = 0; n < temp_length-m-1; n++) {
				if (temp[n]>temp[n+1]) {
					int p = temp[n+1];
					temp[n+1] = temp[n];
					temp[n] = p;
				}
			}
		}
		// uniq second key
		for (int i = 0; i < temp_length-1; i++) {
			if (temp[i]==temp[i+1]) {
				for (int j = i; j < temp_length-1; j++)
					temp[j] = temp[j+1];

				if (temp_length > 0)
					temp_length = temp_length-1;
				i--;
			}
		}
		// put in final list
		int final_length = result_length + temp_length;
		int final[result_length + temp_length];
		for (int k = 0; k < result_length; k++)
			final[k] = result[k];
		for (int j = result_length; j < final_length; j++)
			final[j] = temp[j-result_length];
		// sort final list
		for (int m = 0; m < final_length-1; m++) {
			for (int n = 0; n < final_length-m-1; n++) {
				if (final[n]>final[n+1]) {
					int temp = final[n+1];
					final[n+1] = final[n];
					final[n] = temp;
				}
			}
		}
		// new array to find inter
		int inter_length = result_length + temp_length;
		int inter[inter_length];
		for (int i = 0; i < inter_length; i++) {
			inter[i] = final[i];
		}
		// remove non repeated to get inter
		for (int i = 0; i < inter_length-1; i++) {
			if (inter[i]!=inter[i+1]) {
				for (int j = i; j < inter_length-1; j++)
					inter[j] = inter[j+1];

				if (i == inter_length-2)
					inter_length = inter_length-1;

				if (inter_length > 0)
					inter_length = inter_length-1;
				i--;
			}
		}
		// uniq inter
		for (int i = 0; i < inter_length-1; i++) {
			if (inter[i]==inter[i+1]) {
				for (int j = i; j < inter_length-1; j++)
					inter[j] = inter[j+1];

				if (inter_length > 0)
					inter_length = inter_length-1;
				i--;
			}
		}
		// uniq final list
		for (int i = 0; i < final_length-1; i++) {
			if (final[i]==final[i+1]) {
				for (int j = i; j < final_length-1; j++)
					final[j] = final[j+1];

				if (final_length > 0)
					final_length = final_length-1;
				i--;
			}
		}
		// if final list entry is in inter then remove from final values
		for (int i = 0; i < final_length; i++) {
			for (int j = 0; j < inter_length; j++) {
				if(final[i]==inter[j]){
					for (int k = i; k < final_length-1; k++)
						final[k] = final[k+1];
					if (i == final_length-2) {
						final_length = final_length-1;
						i = final_length;
					}
					if (final_length > 0)
						final_length = final_length-1;
					i--;
				}
			}
		}
		if (j == length-1)
			print_values(final,final_length);
	}
}

/*
 * finds set intersection of values in keys
 */
void inter_value(char *key,char **keys,size_t length) {

	entry *first_key_node = find_key(key);
	if (first_key_node==NULL) {
		printf("no such key\n\n");
		return;
	}
	int result_length = first_key_node->length;
	int result[result_length];
	for (int i = 0; i < result_length; i++)
		result[i] = first_key_node->values[i];

	// sort first key
	for (int m = 0; m < result_length-1; m++) {
		for (int n = 0; n < result_length-m-1; n++) {
			if (result[n]>result[n+1]) {
				int temp = result[n+1];
				result[n+1] = result[n];
				result[n] = temp;
			}
		}
	}
	// uniq first key
	for (int i = 0; i < result_length-1; i++) {
		if (result[i]==result[i+1]) {
			for (int j = i; j < result_length-1; j++)
				result[j] = result[j+1];

			if (result_length > 0)
				result_length = result_length-1;
			i--;
		}
	}
	// iterate through keys
	for (size_t j = 0; j < length; j ++) {
		entry *current = find_key(keys[j]);
		if (current==NULL) {
			printf("no such key\n\n");
			return;
		}
		int temp_length = current->length;
		int temp[temp_length];
		for (int i = 0;i < temp_length;i++)
			temp[i] = current->values[i];
		// sort second key
		for (int m = 0; m < temp_length-1; m++) {
			for (int n = 0; n < temp_length-m-1; n++) {
				if (temp[n]>temp[n+1]) {
					int p = temp[n+1];
					temp[n+1] = temp[n];
					temp[n] = p;
				}
			}
		}
		// uniq second key
		for (int i = 0; i < temp_length-1; i++) {
			if (temp[i]==temp[i+1]) {
				for (int j = i; j < temp_length-1; j++)
					temp[j] = temp[j+1];

				if (temp_length > 0)
					temp_length = temp_length-1;
				i--;
			}
		}
		int final_length = result_length + temp_length;
		int final[result_length + temp_length];
		for (int k = 0; k < result_length; k++)
			final[k] = result[k];
		for (int j = result_length; j < final_length; j++)
			final[j] = temp[j-result_length];

		// sort
		for (int m = 0; m < final_length-1; m++) {
			for (int n = 0; n < final_length-m-1; n++) {
				if (final[n]>final[n+1]) {
					int temp = final[n+1];
					final[n+1] = final[n];
					final[n] = temp;
				}
			}
		}
		// remove non repeated
		for (int i = 0; i < final_length-1; i++) {
			if (final[i]!=final[i+1]) {
				for (int j = i; j < final_length-1; j++)
					final[j] = final[j+1];

				if (i == final_length-2)
					final_length = final_length-1;

				if (final_length > 0)
					final_length = final_length-1;
				i--;
			}
		}
		// uniq
		for (int i = 0; i < final_length-1; i++) {
			if (final[i]==final[i+1]) {
				for (int j = i; j < final_length-1; j++)
					final[j] = final[j+1];

				if (final_length > 0)
					final_length = final_length-1;
				i--;
			}
		}
		if (j == length-1)
			print_values(final,final_length);
	}
}

/*
 * finds set union of values in keys
 */
void union_value(char *key,char **keys,size_t length) {

	entry *first_key_node = find_key(key);
	if (first_key_node==NULL) {
		printf("no such key\n\n");
		return;
	}
	int result_length = first_key_node->length;
	int result[result_length];
	for (int i = 0; i < result_length; i++)
		result[i] = first_key_node->values[i];

	// iterate through keys
	for (size_t j = 0; j < length; j ++) {
		entry *current = find_key(keys[j]);
		if (current==NULL) {
			printf("no such key\n\n");
			return;
		}
		int final[result_length + current->length];
		for (int k = 0; k < result_length; k++) {
			final[k] = result[k];
		}

		// append values
		int final_length = result_length + current->length;
		for (int l = result_length; l < final_length; l++) {
			final[l] = current->values[l-result_length];
		}

		// sort
		for (int m = 0; m < final_length-1; m++) {
			for (int n = 0; n < final_length-m-1; n++) {
				if (final[n]>final[n+1]) {
					int temp = final[n+1];
					final[n+1] = final[n];
					final[n] = temp;
				}
			}
		}
		// uniq
		for (int i = 0; i < final_length-1; i++) {
			if (final[i]==final[i+1]) {
				for (int j = i; j < final_length-1; j++)
					final[j] = final[j+1];

				if (final_length > 0)
					final_length = final_length-1;
				i--;
			}
		}
		if (j == length-1)
			print_values(final,final_length);
	}
}

/*
 * used to print values when given values
 */
void print_values(int *values,size_t length) {

	if (length==0) {
		printf("[]\n\n");
		return;
	}
	printf("[");
	for (int i = 0; i < length-1; i++)
		printf("%d ",values[i]);

	printf("%d]\n\n",values[length-1]);
}
