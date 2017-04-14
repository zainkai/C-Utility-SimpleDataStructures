/*************************************************************************************
** Copyright (c) 2017 Kevin Turkington                                              **
**                                                                                  **
** Permission is hereby granted, free of charge, to any person obtaining a copy     **
** of this software and associated documentation files (the "Software"), to deal    **
** in the Software without restriction, including without limitation the rights     **
** to use, copy, modify, merge, publish, distribute, sublicense, and/or sell        **
** copies of the Software, and to permit persons to whom the Software is            **
** furnished to do so, subject to the following conditions:                         **
**                                                                                  **
** The above copyright notice and this permission notice shall be included in all   **
** copies or substantial portions of the Software.                                  **
**                                                                                  **
** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR       **
** IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,         **
** FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE      **
** AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER           **
** LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,    **
** OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE    **
** SOFTWARE.                                                                        **
**************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEBUG 0

#define SAFE_FREE(x) do { if ((x) != NULL) {free(x); x=NULL;} } while(0)

typedef void* TYPE;

typedef struct
{
    unsigned int size;
    unsigned int capacity;
    TYPE* data;
} adtarr;

adtarr* adtarr_create(int init_capacity)
{
    int i;

    if(init_capacity == 0){
        return NULL;
    }

    adtarr* obj = malloc(sizeof(adtarr));
    obj->data = malloc(init_capacity * sizeof(TYPE));
    obj->capacity = init_capacity;
    obj->size = 0;

    //prevents dangling pointers.
    for(i = 0; i < init_capacity + 1;i++){
        obj->data[i] = NULL;
    }

    return obj;
}

void _adtarr_freedata(adtarr* obj, unsigned int minidx,unsigned int maxidx){
    int i;

    for(i = minidx; i < maxidx +1; i++){
        SAFE_FREE(obj->data[i]);
    }
}

int adtarr_free(adtarr* obj)
{
    if(obj == NULL){
        return EXIT_FAILURE;
    }

    _adtarr_freedata(obj,0,obj->capacity); 

    free(obj->data);
    SAFE_FREE(obj);

    return EXIT_SUCCESS;
}

int adtarr_size(adtarr* obj){
    if(obj == NULL){
        return EXIT_FAILURE;
    }
    
    return obj->size;
}

int adtarr_index(adtarr* obj){
    if(obj == NULL){
        return EXIT_FAILURE;
    }
    
    return obj->size -1;
}

int adtarr_chknull(adtarr* obj, int idx)
{
    if(obj == NULL || idx < -1){
        return EXIT_FAILURE;
    } else if(idx >= obj->capacity || obj->size < idx) {
        return EXIT_FAILURE;
    } else if(idx == -1) {
        idx = obj->size -1;
    }

    return obj->data[idx] == NULL ? EXIT_FAILURE : EXIT_SUCCESS;
}

TYPE adtarr_get(adtarr* obj, int idx)
{
    if(obj == NULL || idx < -1){
        return NULL;
    } else if(idx >= obj->capacity || obj->size < idx) {
        return NULL;
    } else if(idx == -1) {
        idx = obj->size -1;
    }

    return obj->data[idx];
}

TYPE _adtarr_save(adtarr* obj,int idx, TYPE item)
{
    TYPE temp_item = malloc(sizeof(item));
    memcpy(temp_item,item,sizeof(TYPE));
    obj->size++;

    return obj->data[idx] = temp_item;
}

TYPE adtarr_set(adtarr* obj,int idx, TYPE item)
{
    if(obj == NULL || item == NULL || idx < -1){
        return NULL;
    } else if(idx >= obj->capacity || obj->size < idx) {
        return NULL;
    } else if(idx == -1) {
        idx = obj->size -1;
    }

    return _adtarr_save(obj,idx,item);
}

//UNSAFE can cause array to be non contiguous.
int _adtarr_clearitem(adtarr* obj,int idx){
    if(obj == NULL || idx < -1){
        return EXIT_FAILURE;
    } else if(idx >= obj->capacity || obj->size < idx) {
        return EXIT_FAILURE;
    } else if(idx == -1) {
        idx = obj->size -1;
    }

    SAFE_FREE(obj->data[idx]);
    obj->size--;

    return EXIT_SUCCESS;
}

adtarr* adtarr_resize(adtarr* obj, int newcapacity)
{
    if(obj == NULL || newcapacity <= 0){
        return NULL;
    }

    int i;
    TYPE* Newdata = malloc(newcapacity * sizeof(TYPE));
    

    for(i = 0; i < newcapacity;i++){
        //swapping pointers to items.
        Newdata[i] = obj->data[i];
    }

    if((newcapacity - obj->capacity) > 0){
        if(obj->size > newcapacity){
            obj->size = newcapacity; 
        }
        _adtarr_freedata(obj,newcapacity,obj->capacity);
    }

    free(obj->data);
    obj->data = Newdata;
    obj->capacity = newcapacity;

    return obj;
}

int adtarr_insert(adtarr* obj, int idx, TYPE item)
{
    int i;

    if(obj == NULL || item == NULL || idx < -1){
        return EXIT_FAILURE;
    } else if(idx >= obj->capacity || obj->size < idx) {
        return EXIT_FAILURE;
    } else if(idx == -1) {
        idx = obj->size;
    }

    for(i = idx; i < obj->capacity - 1;i++){
        obj->data[i + 1] = obj->data[i]; 
    }

    _adtarr_save(obj,idx,item);

    return EXIT_SUCCESS;
}

int adtarr_additem(adtarr* obj, TYPE item)
{
    if(obj == NULL || item == NULL){
        return EXIT_FAILURE;
    } else if(obj->capacity <= obj->size) {
        return EXIT_FAILURE;
    }

    _adtarr_save(obj,obj->size-1,item);
    return EXIT_SUCCESS;
}

int adtarr_remove(adtarr* obj, int idx)
{
    int i;

    if(obj == NULL || idx < -1){
        return EXIT_FAILURE;
    } else if(idx >= obj->capacity || obj->size < idx) {
        return EXIT_FAILURE;
    } else if(idx == -1) {
        idx = obj->size -1;
    }

    SAFE_FREE(obj->data[idx]);

    for(i = idx; i < obj->size - 1; i++){
        obj->data[i] = obj->data[i+1];
    }

    obj->size--;

    return EXIT_SUCCESS;
}

#if DEBUG

int main()
{
    int item1 = 39;

    adtarr* arr = adtarr_create(100);
    printf("arrayCap:%d\n",arr->capacity);

    adtarr_set(arr,88,&item1);
    adtarr_resize(arr,10);
    printf("arrayCap:%d\n",arr->capacity);

    
    adtarr_set(arr,0,&item1);
    
    int temp = *(int*)adtarr_get(arr,0);
    printf(":::%d\n",temp);

    // adtarrClearItem(arr,0);

    item1 = 69;
    adtarr_insert(arr,3,&item1);
    adtarr_remove(arr,3);

    // temp = *(int*)adtarr_get(arr,3);
    // printf(":::%d\n",temp);

    adtarr_free(arr);


    return 0;
}

#endif