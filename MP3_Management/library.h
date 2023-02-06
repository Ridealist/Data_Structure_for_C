#ifndef LIBRARY_H
#define LIBRARY_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>     // TODO NULL 값을 사용하려면 stdlib.h가 필요함!

// TODO 구조체는 type을 정의하는 것이므로, header 파일에 포함시키는게 좋음 -> 헤더가 include된 모든 파일에서 사용 가능

// 순환적 구조이므로 뒤에 정의된 내용이 앞 struct 멤버로 나올 수 밖에 없다.
// TODO typedef를 이용해서 프로토타입 전방 선언을 해준 후 뒤에서 정의를 한다.
typedef struct song Song;
typedef struct snode SNode;
typedef struct artist Artist;

struct song {
    Artist *artist;
    char *title;
    char *path;
    int index;
};

struct snode {
    struct snode *next, *prev;
    Song *song; 
};

struct artist {
    char *name;
    struct artist *next;
    SNode *head, *tail;
};

void initialize();
void add_song(char *artist, char *title, char *filepath);
void status();  // TODO 항상 함수 헤더파일에 포함하는 것 까먹지 말기!
void load(FILE *fp);
void search_songs(char *artist);
void search_song(char *artist, char *title);
void play(int index);

#endif  // LIBRARY_H