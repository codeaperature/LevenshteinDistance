//
//  main.c
//  LevenshteinDistance
//
//  Created by Stephan Warren on 3/7/16.
//  Copyright © 2016 Stephan Warren. All rights reserved.
//
/*
 LEVENSHTEIN DISTANCE
 VIEW CHALLENGE DESCRIPTION
 
 CHALLENGE DESCRIPTION:
 
 
 Two words are friends if they have a Levenshtein distance of 1 (For details see Levenshtein distance). That is, you can add, remove, or substitute exactly one letter in word X to create word Y. A word’s social network consists of all of its friends, plus all of their friends, and all of their friends’ friends, and so on. Write a program to tell us how big the social network for the given word is, using our word list.
 
 INPUT SAMPLE:
 
 The first argument will be a path to a filename, containing words, and the word list to search in. The first N lines of the file will contain test cases, they will be terminated by string 'END OF INPUT'. After that there will be a list of words one per line. E.g
 recursiveness
 elastic
 macrographies
 abbreviated
 abbreviate
 hello
 recursiveness
 elastic
 macrographies
 END OF INPUT
 aa
 aahed
 aahs
 aalii
 ...
 ...
 zymoses
 zymosimeters
 OUTPUT SAMPLE:
 
 For each test case print out how big the social network for the word is. In sample the social network for the word 'elastic' is 3 and for the word 'recursiveness' is 1. E.g.
 
 
 
 1
 3
 1
 Constraints:
 Number of test cases N in range(15, 30)
 The word list always will be the same and it's length will be around 10000 words
 
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>



#define BUF_LEN 32

//#define TIMER_ON
#ifdef TIMER_ON
#include <time.h>
#endif


#define MIN3(a,b,c) ((a < b) ? ((a < c) ? a : c) :((b < c) ? b : c))


//
//typedef struct {
//    char * w; // word
//    int c; //cnt
//    unsigned char l; // len
//} SWORD;

typedef struct SWORD_S {
    char * word;
    struct SWORD_S * next;
    char len;
} SWORD_S;


typedef struct {
    SWORD_S * runF;
    SWORD_S * runL;
    SWORD_S * first;
    SWORD_S * last;
    unsigned short cnt;
} SWORD_EXT;


typedef struct {
    unsigned char b[4];
} DICTIONARY;


char PFLAG = 0;

void xDupNLenNCnt(char *str, SWORD_EXT * sx) {
    SWORD_S * ret;
    assert((ret = malloc(sizeof(SWORD_S))) != NULL);
    char * send = str;
    while(*(++send) != '\n');
    ret->len = (int) (send - str);
    //    printf("len = %d, str = %s, *send = %c\n", ret->l, str, *send);
    char * cend = (ret->word = malloc(ret->len + 1)) + ret->len;
    *cend = '\0';
    while(send > str) *(--cend) = *(--send);
    ret->next = NULL;
    sx->first = ret;
    sx->last = ret;
    sx->runF = ret;
    sx->runL = ret;
    sx->cnt = 1;
}


unsigned char LevenshteinIs1(char * s, char * l, char slen, char llen) {
    
    char difflen = llen - slen;
    char flip = 0;
    //printf("diff = %d\n", difflen);
    if(difflen < 0) {
        flip = !0;
        difflen = -difflen;
        //    printf("Flip\n");
    }
    if(difflen > 1) return(0);
    else {
        unsigned char diffs = 0;
        if(slen == llen) {
            for(unsigned char i = 0; i < slen; i++) {
                if(s[i] != l[i]) {
                    diffs++;
                    if(diffs > 1) return(0);
                }
            }
            return(diffs); // the word doesn't itself doesn't count
        }
        else {
            if(flip) {
                char * t = s;
                s = l;
                l = t;
                llen++;
                slen--;
            }
            unsigned char i = 0;
            while(l[i] == s[i]) i++;
            unsigned char j = i + 1;
            while(s[i]) {
                if(s[i++] != l[j++]) return(0);
            }
            return(1);
        }
    }
    return(0);
    
}

int main(int argc, const char * argv[]) {
#ifdef TIMER_ON
    clock_t start = clock();
    clock_t diff;
#endif
    FILE *file = fopen(argv[1], "r");
    char line[BUF_LEN];
    //char flag = 1;
    SWORD_EXT sword[30];
    unsigned char dict_len[23104];
    unsigned short dict_cnt = 0;
    char sword_cnt = 0;
    unsigned char smaxl = 0;
    unsigned char lmaxl = 0;
    unsigned short run_cnt = 0;
    
    
    //    printf("ftell = %ld\n", ftell(file));
    while(strcmp("END OF INPUT\n", fgets(line, BUF_LEN, file))) {
        xDupNLenNCnt(line, &sword[sword_cnt]);
        //       printf("%d - >%s<\n", cnt, sword[cnt].w);
        sword_cnt++;
    }
    //    while(fscanf(file, "%s\n", line) == 1) {
    long dict_off = ftell(file);
    while(fgets(line, 1024, file)) {
        //        if(!strcmp(line, "elastins\n")) {
        //            printf("yeah\n");
        //            PFLAG = 1;
        //        }
        //       printf("ftell = %ld\n", ftell(file));
        unsigned char len = (unsigned char) strlen(line);
        unsigned char lenm1;
        unsigned char lenp1;
        if(line[lenm1 = len - 1] == '\n') {
            line[lenm1] = 0;
            lenp1 = len;
            len = lenm1--;
        }
        else {
            lenp1 = len + 1;
        }
        if(len > smaxl) lmaxl = len;
        dict_len[dict_cnt]= lenp1;
        
        //        printf(">>%s<< l = %d, %d %d\n",line, lenm1, len, lenp1);
        //        printf("  123456789a1234567890\n");
        
        for(int i = 0; i < sword_cnt; i++) {
            unsigned char x = LevenshteinIs1(line, sword[i].first->word, len, sword[i].first->len);
            if(x) {
                //                printf("%s(%d) vs %s(%d,%d), dist = %u\n", line, len,
                //                         sword[i].first->word, sword[i].first->len, (int) strlen(sword[i].first->word), x);
                sword[i].cnt++;
                run_cnt++;
                SWORD_S * tem = sword[i].last;
                assert((tem->next = malloc(sizeof(SWORD_S))) != NULL);
                tem = tem->next;
                tem->word = strdup(line);
                tem->len = len;
                tem->next = NULL;
                sword[i].last = tem;
            }
        }
        dict_cnt++;
    } // while
    
    long eof_off = ftell(file);
    
    for(unsigned short i = 0; i < sword_cnt; i++) {
        sword[i].runF = sword[i].runL;
        sword[i].runL = sword[i].last;
    }
    
    while(run_cnt) { // still adding new entries
        run_cnt = 0;
        long cur_off = dict_off;
        unsigned short di = 0;
        fseek(file, cur_off, SEEK_SET);
        while(eof_off > cur_off) { // loop dictionary
            fread(line, sizeof(char), dict_len[di], file);
            unsigned char lenm1 = dict_len[di]-1;
            line[lenm1] = '\0';
            //            printf("dline = %s\n", line);
            for(unsigned char si = 0; si < sword_cnt; si++) {
                SWORD_S * sptr1 = sword[si].runF;
                char match = 0;
                while(sptr1 != sword[si].runL->next) {
                    //                    printf("  %s vs %s: %s\n",
                    //                           sptr1->word, line, LevenshteinIs1(line, sptr1->word, lenm1, sptr1->len)? "T": "F");
                    if(LevenshteinIs1(line, sptr1->word, lenm1, sptr1->len)) {
                        SWORD_S * sptr2 = sword[si].first;
                        while(sptr2 != sword[si].last->next) {
                            //                            printf("      sub: %s vs %s\n", sptr1->word, sptr2->word);
                            if(!strcmp(line, sptr2->word)) {
                                match = 1;
                                sptr2 = sword[si].last->next;
                            }
                            else {
                                sptr2 = sptr2->next;
                            }
                        } // while
                        if(!match) {
                            //                            printf("    +++ adding %s\n", line);
                            run_cnt++;
                            SWORD_S * tem;
                            assert((tem = malloc(sizeof(SWORD_S))) != NULL);
                            sword[si].last->next = tem;
                            sword[si].last = tem;
                            tem->word = strdup(line);
                            tem->len = dict_len[di] - 1;
                            tem->next = NULL;
                            sword[si].last = tem;
                            sword[si].cnt++;
                        }
                    }
                    sptr1 = (!match) ?  sptr1->next : sword[si].runL->next;
                } // while each sought word has friends
                
            } // for of sought words
            
            cur_off+=dict_len[di];
            di++;
        } // looping dictionary
        for(unsigned char i = 0; i < sword_cnt; i++) {
            sword[i].runF = sword[i].runL;
            sword[i].runL = sword[i].last;
        }
        //        printf("runcnt = %d\n", run_cnt);
    } // stil adding entries
    
    
    for(int i = 0; i < sword_cnt; i++) {
        //        printf("%s: ", sword[i].first->word);
        printf("%d\n", sword[i].cnt);
    }
#ifdef TIMER_ON
    diff = clock() - start;
    clock_t  msec = diff * 1000 / CLOCKS_PER_SEC;
    printf("Time taken %d seconds %d milliseconds\n", (int) msec/1000, (int) msec%1000);
    printf("smaxl = %d, lmaxl = %d, dmaxl = %u\n", smaxl, lmaxl, dict_cnt);
#endif
    
    return 0;
}
