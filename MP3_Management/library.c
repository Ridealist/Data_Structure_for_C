#include "library.h"
#include "string_tools.h"


#define NUM_CHARS 256   // 2^8 = 256 -> 1byte 가질 수 있는 서로 다른 값의 크기 (어차피 첫글자는 char 자료형 이기 때문에)
#define BUFFER_LENGTH 200

// TODO 배열은 다른 파일에서 공유 변수로 사용하지 않을 것이므로, 헤더 파일에 정의할 필요 없다.  공유 변수 최소화!
Artist *artist_directory[NUM_CHARS];
int song_index = 0;

// 어떤 배열의 type은 배열 "1칸"에 저장되는 타입이 무엇이냐에 대한 답


void initialize()     // TODO C언어에선 배열을 선언하면, 배열의 초기값이 어떻게 설정되어 있는지 모름. 항상 초기화를 해주어야 함!
{
    for (int i = 0; i < NUM_CHARS; i++) {
        artist_directory[i] = NULL;
    }
}

void load(FILE *fp)
{
    char buffer[BUFFER_LENGTH];
    char *name, *title, *path;

    while(read_line(fp, buffer, BUFFER_LENGTH) > 0) {
        name = strtok(buffer, "#");
        if (strcmp(name, " ") == 0)
            name = NULL;
        else
            // TODO name은 buffer에 일부 저장된 데이터인데 while loop을 돌면서 덮어쓰어져 버림
            // TODO strdup로 새로 생성해서 저장해주어야 함 
            name = strdup(name);

        title = strtok(NULL, "#");
        if (strcmp(title, " ") == 0)
            title = NULL;
        else
            title = strdup(title);
   
        path = strtok(NULL, "#");
        if (strcmp(path, " ") == 0)
            path = NULL;
        else
            path = strdup(path);

        add_song(name, title, path);
    }
    printf("Data file loaded.\n");
}

Artist *find_artist(char *name)
{
    Artist *p = artist_directory[(unsigned char)name[0]];
    while (p != NULL && strcmp(p->name, name) < 0 ) // 연결리스트는 '이름' 오름차순 정렬이므로 끝까지 볼 필요 없다
        p = p->next;                                // 내 이름보다 작을때까지만(앞 순서) looping을 돌아도 충분
    
    if (p != NULL && strcmp(p->name, name) == 0)
        return p;
    
    return NULL;
}


Artist *create_artist_instance(char *name)
{
    Artist *p = (Artist *)malloc(sizeof(Artist));   // 객체를 생성하는 일은 객체 생성 함수를 따로 만드는 것을 추천! 
    p->name = name;     // 객체 생성 후 초기화하는 것도 좋은 습관.
    p->head = NULL;
    p->tail = NULL;
    p->next = NULL;
    return p;
}


Artist *add_artist(char *name)
{
    Artist *ptr_artist = create_artist_instance(name);

    Artist *p = artist_directory[(unsigned char)name[0]];   // first node
    Artist *q = NULL;

    while (p != NULL && strcmp(p->name, name) < 0) {
        q = p;
        p = p->next;
    }

    if (p == NULL && q == NULL)     // unique node
        artist_directory[(unsigned char)name[0]] = ptr_artist;
    else if (q == NULL) {   // add at the front
        ptr_artist->next = p;
        artist_directory[(unsigned char)name[0]] = ptr_artist;
    }
    else {  // add after(q)
        ptr_artist->next = q->next;
        q->next = ptr_artist; 
    }

    return ptr_artist;  
}


Song *create_song_instance(Artist *ptr_artist, char *title, char *filepath)
{
    Song *ptr_song = malloc(sizeof(Song));
    ptr_song->artist = ptr_artist;
    ptr_song->title = title;
    ptr_song->path = filepath;
    ptr_song->index = song_index;
    song_index++;   // TODO 노래를 삭제하면 그 번호에 해당하는 값이 비어버리는 문제가 발생하기는 함... 
    
    return ptr_song;
}

SNode *create_snode_instance(Song *ptr_song)
{
    SNode *ptr_snode = malloc(sizeof(SNode));
    ptr_snode->song = ptr_song;
    ptr_snode->next = NULL;
    ptr_snode->prev = NULL;
    return ptr_snode;
}


void insert_node(Artist *ptr_artist, SNode *ptr_snode)
{
    SNode *p = ptr_artist->head;
    SNode *q = NULL;
    while (p != NULL && strcmp(p->song->title, ptr_snode->song->title) < 0) {
        q = p;
        p = p->next;
    }
    // TODO p, q를 사용하는 것보다 논리적으로 더 명료하게 if 조건문 분기시키기 고려
    // if (ptr_artist->head == NULL)
    if (p == NULL && q == NULL) {   // 1. empty
        ptr_artist->head = ptr_snode;
        ptr_artist->tail = ptr_snode; // TODO 이중연결리스트에서는 노드가 1개이면 head와 tail을 모두 연결해야 한다!
    }
    // else if (p == ptr_artist->head)
    else if (q == NULL) {   // 2. at the front
        ptr_snode->next = p;
        p->prev = ptr_snode;
        ptr_artist->head = ptr_snode;
    }
    // else 로 맨 뒤에서 처리
    else if (p != NULL && q != NULL) {  // 3. in the middle
        ptr_snode->next = p;
        ptr_snode->prev = q;
        p->prev = ptr_snode;
        q->next = ptr_snode;
    }
    // else if (q == ptr_artist->tail)
    else if (p == NULL && q != NULL) {  // 4. at th end
        q->next = ptr_snode;
        ptr_snode->prev = q;
        ptr_artist->tail = ptr_snode;
    }
}


void add_song(char *artist, char *title, char *filepath)
{
    // find if the artist already exists
    // return NULL if not  
    Artist *ptr_artist = find_artist(artist);
    if (ptr_artist == NULL) {
        ptr_artist = add_artist(artist); // 새로운 함수 만들기 1. 함수 이름, 2. 함수 매개변수, 3. 함수 리턴값
    }

    // add the song to the artist pointed by ptr_artist
    Song *ptr_song = create_song_instance(ptr_artist, title, filepath);
    SNode *ptr_snode = create_snode_instance(ptr_song);

    // insert snode
    insert_node(ptr_artist, ptr_snode);
}


void print_song(Artist *ptr_artist, Song *ptr_song) {
    printf("    %d : %s, %s, %s\n", ptr_song->index, ptr_artist->name, ptr_song->title, ptr_song->path);
}

void print_artist(Artist *ptr_artist)
{
    printf("%s\n", ptr_artist->name);
    SNode *p = ptr_artist->head;
    while (p != NULL)
    {
        Song *ptr_song = p->song;
        print_song(ptr_artist, ptr_song);
        p = p->next;
    }
}

void status()
{
    for (int i = 0; i < NUM_CHARS; i++) {
        // if (artist_directory[i] == NULL)
        //     continue;
        Artist *p = artist_directory[i];
        while (p != NULL)
        {
            print_artist(p);
            p = p->next;
        }
        // p == NULL이면 while 문을 돌지 않고 바로 빠져나와 print_artist 함수 실행되지 않음
    }
}

SNode *find_snode(Artist *ptr_artist, char *title)
{
    SNode *p = ptr_artist->head;
    while (p != NULL && strcmp(p->song->title, title) < 0) {
        p = p->next;
    }
    if (p == NULL || strcmp(p->song->title, title) != 0) {
        return NULL; 
    }
    return p;
}

void search_songs(char *artist)
{
    Artist *ptr_artist = find_artist(artist);
    if (ptr_artist == NULL) {
        printf("No such artist exists.\n");
        return;
    }

    SNode *p = ptr_artist->head;
    printf("Found:\n");
    print_artist(ptr_artist);
}

void search_song(char *artist, char *title)
{
    Artist *ptr_artist = find_artist(artist);
    if (ptr_artist == NULL) {
        printf("No such artist exists.\n");
        return;
    }

    SNode *p = find_snode(ptr_artist, title);
    if (p == NULL) {
        printf("No such song exists.\n");
        return;
    }
    
    printf("Found:\n");
    print_song(ptr_artist, p->song);
}
