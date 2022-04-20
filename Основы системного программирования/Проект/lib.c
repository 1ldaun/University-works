#include <getopt.h>
#include <stdio.h>
#include "plugin_api.h"
#include <stdlib.h>

static struct plugin_option asd[] = {{{"mac-addr-bin", required_argument, 0, 0}, "Принимает 1 аргумент - мак адрес в бинарной форме. Далее поиск будет осуществляться в том числе по заданному мак адресу."}};


int plugin_get_info(struct plugin_info* ppi) {

    ppi->plugin_purpose = "Проверяет файл на содержание заданного значение MAC-адреса в бинарной (little-endian или big-endian) форме.";
    ppi->plugin_author = "Шарифуллин Ильдан Айдарович";
    ppi->sup_opts_len = 1;
    ppi->sup_opts = asd;
    
    return 0;
}


int memcmp2(const void* buf1,                                   
           const void* buf2,
           size_t count)
{
    if(!count)
        return(0);

    while(--count && (*(char*)buf1 == *(char*)buf2) ) {
        buf1 = (char*)buf1 + 1;
        buf2 = (char*)buf2 + 1;
    }
    
    return(*((unsigned char*)buf1) - *((unsigned char*)buf2));
}

int BF(void *x, char *y, long unsigned int m) {                     
    int i;
//    printf("%s and %s\n", (char *)x, y);
     for(i=0; (*y != EOF) && (i <= 999); i++, y++) {
         if(memcmp2(y,x,m) == 0) {
             return 1;
         }
     }
    return 0;
}


int plugin_process_file(const char *fname,
        struct option in_opts[],
        size_t in_opts_len) {
    
    FILE* fl;
    char* strm = (char*) malloc(128*sizeof(char));
    int* reversed = (int*) malloc(18*sizeof(int));
    int* addrs_opts = (int*) malloc(in_opts_len*sizeof(int));
    int flg = 0, res = 0;
    int flag_OR = 0, flag_AND = 0, flag_wow = 0;
    
    for (int i = 0; i < (int)in_opts_len; i++){
        addrs_opts[i] = 0;
    }
    if ((fl = fopen(fname, "r")) == NULL)
    {
//        perror("openfile");
	if (getenv("LAB1DEBUG"))
        	printf("Не удалось открыть файл: %s\n", fname);
        return -1;
    }
    for (size_t i = 0; i < in_opts_len; i++){
//    printf("%s\n", in_opts[i].name);
    	if (in_opts[i].name != NULL) {
            if (*(in_opts[i].name) == 'O')
                flag_OR = 1;
            else if (*(in_opts[i].name) == 'A') {
                flag_AND = 1;
                res = 1;
            }
        }
    }
    if ((!flag_OR) && (!flag_AND)) {
        flag_AND = 1;
    }
    while (fgets(strm, 128, fl)) {
        for (size_t i = 0; i < in_opts_len; i++){
            flg = 0;
            if (in_opts[i].name != NULL) {
                if (BF("mac-addr-bin", (char*)in_opts[i].name, 13)) {
                    flg = BF(in_opts[i].flag, strm, 17);
                    }
/*                    if (flag_OR)
                        res |= flg;
                    else if (flag_AND)
                        res &= flg; 
	                    if (flg)
                    		printf("!!! %s and %s\n", (char *)in_opts[i].flag, strm);	*/
                    addrs_opts[i] += flg;
            }
        }
    }
    
    for (size_t i = 0; i < in_opts_len; i++){
        if (in_opts[i].name != NULL) {
            if (BF("mac-addr-bin", (char*)in_opts[i].name, 13)) {
                if (addrs_opts[i]) {    
                    if (getenv("LAB1DEBUG")){
                        printf("\tВ файле %s найдена строка %s \n", fname, (char*) in_opts[i].flag);
                    }
                    
                    if (flag_AND && !flag_wow) {
                    	res = 1;
                    	flag_wow = 1;
                    }
                    
                    if (flag_OR)
                        res |= 1;
                    else if (flag_AND) {
                        res &= 1;
                        flag_wow = 1;
                   }
                } else {
                    if (flag_OR)
                        res |= 0;
                    else if (flag_AND) {
                        res &= 0;
                        flag_wow = 1;
                    }
                }
            }
        }
    }
    
    fclose(fl);
    free(strm);
    free(reversed);
    free(addrs_opts);
    
    
    if (res == 0)
        return 1;
    else
        return 0;
}

