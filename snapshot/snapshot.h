#ifndef SNAPSHOT_H
#define SNAPSHOT_H

#define MAX_KEY 16
#define MAX_LINE 1024
#define MAX_COMMAND 32

typedef struct entry entry;
typedef struct snapshot snapshot;

struct entry {
	char key[MAX_KEY];
	int* values;
	size_t length;
	entry* next;
	entry* prev;
};

struct snapshot {
	bool dropped;
	int id;
	entry* entries;
	snapshot* next;
	snapshot* prev;
};

const char* HELP =
	"BYE   clear database and exit\n"
	"HELP  display this help message\n"
	"\n"
	"LIST KEYS       displays all keys in current state\n"
	"LIST ENTRIES    displays all entries in current state\n"
	"LIST SNAPSHOTS  displays all snapshots in the database\n"
	"\n"
	"GET <key>    displays entry values\n"
	"DEL <key>    deletes entry from current state\n"
	"PURGE <key>  deletes entry from current state and snapshots\n"
	"\n"
	"SET <key> <value ...>     sets entry values\n"
	"PUSH <key> <value ...>    pushes values to the front\n"
	"APPEND <key> <value ...>  appends values to the back\n"
	"\n"
	"PICK <key> <index>   displays value at index\n"
	"PLUCK <key> <index>  displays and removes value at index\n"
	"POP <key>            displays and removes the front value\n"
	"\n"
	"DROP <id>      deletes snapshot\n"
	"ROLLBACK <id>  restores to snapshot and deletes newer snapshots\n"
	"CHECKOUT <id>  replaces current state with a copy of snapshot\n"
	"SNAPSHOT       saves the current state as a snapshot\n"
	"\n"
	"MIN <key>  displays minimum value\n"
	"MAX <key>  displays maximum value\n"
	"SUM <key>  displays sum of values\n"
	"LEN <key>  displays number of values\n"
	"\n"
	"REV <key>   reverses order of values\n"
	"UNIQ <key>  removes repeated adjacent values\n"
	"SORT <key>  sorts values in ascending order\n"
	"\n"
	"DIFF <key> <key ...>   displays set difference of values in keys\n"
	"INTER <key> <key ...>  displays set intersection of values in keys\n"
	"UNION <key> <key ...>  displays set union of values in keys\n";

void command_bye ();
void command_help();

// check if command exists and refer to corresponding function if it does
void check_command(char *line);

// initializes an empty doubly linked list
void entries_init();

//initializes snapshots
 void snapshots_init();

// adds new entry
void entry_add(char *key, int* value, size_t length);

// creates a new entry
entry* create_entry(char *key, int*value, size_t length);

// used to edit values in already existing key
void entry_edit(entry *node, char *key, int *value, size_t length);

// adds new snapshot
void snapshot_add();

// creates a new snapshot
snapshot* create_snapshot(int id);

// restores current state to snapshot and deletes newer snapshots
void snapshot_rollback(int id);

// replaces current state with copy of snapshot
void snapshot_checkout(int id);

// finds snapshot using given id
snapshot* find_snapshot(int id);

// deletes snapshot by id
void snapshot_delete(int id);

// deletes entry from current state using key
void entry_delete_key(char *key);

//deletes given entry
void entry_delete_node(entry *node);

// deletes entry from current state and snapshots
void entry_purge(char *key);

// displays an entry's values
bool get_values(char *key);

// finds node containing given key
entry *find_key(char *key);

// displays all keys in current state
void list_keys();

// displays all keys and matching values in current state
void list_entries();

// display all snapshots in the database
void list_snapshots();

// pushes given values to front
void push_values(char *key, int *values, size_t length);

// appends given values to end
void append_values(char *key, int *value, size_t length);

// displays value at given index
void pick_value(char *key, int index);

// displays and removes value at index
void pluck_value(char *key,int index);

// displays and removes the front value
void pop_value(char *key);

// displays minimum value
void min_value(char *key);

// displays maximum value
void max_value(char *key);

// displays sum of values
void sum_value(char *key);

// display number of values
void len_value(char *key);

// reverses order of values
void rev_value(char *key);

// removes repeated adjacent values
bool uniq_value(char *key);

// sort values in ascending order
bool sort_value(char *key);

//finds symmetrical set difference of values in keys
void diff_value(char *key,char **keys,size_t length);

// finds set intersection of values in keys
void inter_value(char *key,char **keys,size_t length);

// finds set union of values in keys
void union_value(char *key,char **keys,size_t length);

// used to print values when given values
void print_values(int *values,size_t length);

#endif
