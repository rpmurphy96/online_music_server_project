//
// Created by Ryan Murphy on 4/15/19.
//

/*

  The required function prototypes and several variable names for this project were provided by Donald Yeung.

*/

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <math.h>
#include <string.h>

//function prototypes
void print_albums(int n_albums);
void print_album(int album_ID);
void open_account(int user_ID);
int print_account(int user_ID);
int add_playlist(int entered_user_ID, int entered_album_ID, int entered_track_number);
int close_account(int user_ID);

// typedef's (ideally these would be moved to a different .h file)
typedef struct _playlist {
    int album_ID; //the album ID for the playlist entry
    int track_num; //specifies the track number in the album containing the song on the playlist
    struct _playlist *next;
} playlist, *p_playlist;

typedef struct _account {
    int ID; //ID hold the unique user ID
    p_playlist playlist; //head pointer for a linked list that stores the user's playlist information. //CHANGED THIS*******
    struct _account *next;
} account, *p_account;

typedef struct _album {
    int num_tracks; //the number of tracks
    char **tracks; //a pointer to a pointer array (of size num_tracks) that holds the title of each track
    int *playlist_hits; //a pointer to an array of integers (one per track)
} album, *p_album;

typedef struct tracks_ {
    char *track;
} tracks, *p_tracks;

typedef struct _playlist_hits {
    int track_hits; //the number of hits a particular track has
} playlist_hits, *pplaylist_hits;


//Global pointer to a structs
p_account head_ptr_my_accounts;
p_album global_album_ptr;
p_playlist head_ptr_my_playlists;

int main(int argc, char *argv[]) {

    int i, j, k, p, n;
    int n_albums;
    int input1, input2, input3, input4;
    int album_ID;
    int user_ID;
    int track_number;
    int run = 0;

    FILE*  album_file = fopen(argv[1], "r");
    FILE*  cmd_file = fopen(argv[2], "r");

    if (album_file == NULL || cmd_file == NULL) {
        printf("Could not open files\n"); //Error correction if any of the files can't be opened
        exit(0);
    }

    //PARSE DATA (READ IN THE DATA FROM THE ALBUMS FILE)
    //see how many albums we should create
    fscanf(album_file, "%d", &n_albums);
    //create (malloc) the array of albums
    global_album_ptr = (p_album) malloc(n_albums * sizeof(album));
    int number_of_tracks;
    int track_length;
    char temp_track[1000];

    for (i = 0; i < n_albums; i++) {
        fscanf(album_file, "%d", &number_of_tracks);
        global_album_ptr[i].num_tracks = number_of_tracks;

        global_album_ptr[i].tracks = (char **) malloc((global_album_ptr[i].num_tracks) * sizeof(tracks));
        global_album_ptr[i].playlist_hits = (int *) malloc((global_album_ptr[i].num_tracks) * sizeof(playlist_hits));
        for (k = 0; k < global_album_ptr[i].num_tracks; k++) {
            fscanf(album_file, "%d %[^\n]", &track_length, temp_track);
            global_album_ptr[i].tracks[k] = malloc(track_length * sizeof(char) + 1);
            strcpy(global_album_ptr[i].tracks[k], temp_track);
        }
    }

    //Set the head pointer to my accounts linked list to NULL
    head_ptr_my_accounts = NULL;

    while (1) {
        fscanf(cmd_file, "%d", &input1);
        if (input1 == 1) {
            fscanf(cmd_file, "%d", &input2);
            album_ID = input2;
            print_album(album_ID);
        }
        if (input1 == 2){
            fscanf(cmd_file, "%d", &input2);
            user_ID = input2;
            open_account(user_ID);
        }
        if (input1 == 3){
            fscanf(cmd_file, "%d", &input2);
            user_ID = input2;
            print_account(user_ID);
        }
        if (input1 == 4){ //It's possible that I may need to change the album_ID and track_number to have unique
            // variables to that I don't mess with other parts of the project
            fscanf(cmd_file, "%d", &input2);
            user_ID = input2;
            fscanf(cmd_file, "%d", &input3);
            album_ID = input3;
            fscanf(cmd_file, "%d", &input4);
            track_number = input4;
            add_playlist(user_ID, album_ID, track_number);
        }
        if (input1 == 5){
            fscanf(cmd_file, "%d", &input2);
            user_ID = input2;
            close_account(user_ID);
        }
        if (input1 == 6){
            break;
        }
        run++;
    }
    return 0;
}

//Function definitions (Ideally these would be placed into a separate .c file)
void print_albums(int n_albums){
    char *p;
    int i, k;
    char track_name[1000];

    for (i=0; i<n_albums; i++){
        printf("\n\nAlbum %d) \n\tNumber of tracks: %d \n\tTracks:\n", i+1, global_album_ptr[i].num_tracks);
        for (k=0; k<global_album_ptr[i].num_tracks; k++){
            p = global_album_ptr[i].tracks[k];
            strcpy(track_name, p);
            printf("%100s ", track_name);
            printf("(hits:%d)\n", global_album_ptr[i].playlist_hits[k]);
        }
    }
}

void print_album(int album_ID){
    char *p;
    int k;
    char track_name[1000];
    printf("ALBUM %d\n", album_ID);
    for (k=0; k < global_album_ptr[album_ID].num_tracks; k++){
        printf("  %d: ", global_album_ptr[album_ID].playlist_hits[k]);
        p = global_album_ptr[album_ID].tracks[k];
        strcpy(track_name, p);
        printf("%s\n", track_name);
    }
    printf("\n");
}

//insert the new user account at the head of the accounts linked list
void open_account(int user_ID){
    p_account new = (p_account) malloc(sizeof(account));
    if (head_ptr_my_accounts == NULL){ //if this is the first account in the linked list
        new->ID = user_ID;
        new->playlist = NULL;
        new->next = NULL;
        head_ptr_my_accounts = new;
    } else { //if this is NOT the first account in the linked list
        new->ID = user_ID;
        new->playlist = NULL;
        new->next = head_ptr_my_accounts;
        head_ptr_my_accounts = new;
    }
}

//print the user account that's associated with the entered user ID
int print_account(int user_ID){
    char track_name[1000];
    char *p;
    int track_number;
    int album_number;
    p_account cur = head_ptr_my_accounts;
    p_playlist curplay;
    while(cur){
        if (cur->ID == user_ID){
            printf("ACCOUNT %d\n", user_ID);
            if (cur->playlist == NULL){
                printf("EMPTY PLAYLIST\n\n");
                return 0;
            }
            curplay = cur->playlist; //curplay is now the head pointer to this users playlist linked list
            //Now run through the playlist linked list and print all the nodes (the tracks w their album_ID's)
            while (curplay){ //while curplay is not NULL
                album_number = curplay->album_ID;
                track_number = curplay->track_num;
                printf("  ALBUM %d:  ", album_number);
                p = global_album_ptr[album_number].tracks[track_number];
                strcpy(track_name, p);
                printf("%s\n", track_name);
                curplay = curplay->next;
            }
        }
        cur = cur->next;
    }
    printf("\n");
    return 0;
}

int add_playlist(int entered_user_ID, int entered_album_ID, int entered_track_number){
    p_playlist new_playlist = (p_playlist) malloc(sizeof(playlist));
    new_playlist->album_ID = entered_album_ID;
    new_playlist->track_num = entered_track_number;
    new_playlist->next = NULL; //bc we're inserting the new playlist at the tail
    p_account cur = head_ptr_my_accounts;
    //find the user account with the specified user_ID
    while(cur){
        if (cur->ID == entered_user_ID){
            if (cur->playlist == NULL){ //meaning this is the first playlist in the playlist linked list
                cur->playlist = new_playlist;
                global_album_ptr[entered_album_ID].playlist_hits[entered_track_number]++;
                return 0;
            }
            p_playlist curplay;
            curplay = cur->playlist;
            while(curplay->next){ //this line transverses the user account's playlist linked list till it hits the tail node
                curplay = curplay->next;
            }
            //now do tail-insertion
            curplay->next = new_playlist;
        }
        cur = cur->next;
    }
    //Now increment the playlist hits
    global_album_ptr[entered_album_ID].playlist_hits[entered_track_number]++;
    return 0;
}

int close_account(int user_ID){
    p_account cur_account;
    p_account cur_account_prev;
    p_playlist cur_playlist;
    p_playlist cur_playlist_prev;
    cur_account = head_ptr_my_accounts;
    cur_account_prev = head_ptr_my_accounts;
    //find the user account with the specified user_ID
    while (cur_account){
        if (cur_account->ID == user_ID){
            cur_playlist = cur_account->playlist;
            //go through the account's playlist and decrement every track we find then delete the link node
            if (cur_playlist->next == NULL){
                global_album_ptr[cur_playlist->album_ID].playlist_hits[cur_playlist->track_num]--;
                free(cur_playlist);
            }
            while(cur_playlist){
                global_album_ptr[cur_playlist->album_ID].playlist_hits[cur_playlist->track_num]--;
                cur_playlist_prev = cur_playlist;
                cur_playlist = cur_playlist->next;
                if (cur_playlist_prev != cur_account->playlist)
                    free(cur_playlist_prev);
            }
            //Now go through and delete the user's account
            if (head_ptr_my_accounts->next == NULL){ //if there is only one account in our accounts linked list
                //delete the one and only (matching) account and set the head pointer to NULL
                free(cur_account);
                head_ptr_my_accounts = NULL;
                return 0;
            } else if (head_ptr_my_accounts == cur_account){ //if the account is a head node
                //perform head deletion
                head_ptr_my_accounts = cur_account->next;
                free(cur_account);
                return 0;
            } else if (cur_account->next == NULL){ //if the account is the tail node in our accounts linked list
                //perform tail deletion
                cur_account_prev->next = NULL;
                free(cur_account);
                return 0;
            } else if (cur_account->ID == user_ID){
                //perform middle node deletion
                cur_account_prev->next = cur_account->next;
                free(cur_account);
                return 0;
            }
        }
        cur_account_prev = cur_account;
        cur_account = cur_account->next;
    }
    return 0;
}
