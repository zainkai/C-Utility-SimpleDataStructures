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
} adtstk;

adtstk* adtstk_create(int init_capacity)
{
    int i;
    
    if(init_capacity == 0){
        return NULL;
    }

    adtstk* obj = malloc(sizeof(adtstk));

    obj->data = malloc(init_capacity * sizeof(TYPE));
    obj->capacity = init_capacity;
    obj->size = 0;

    //prevents dangling pointers.
    for(i = 0; i < init_capacity + 1;i++){
        obj->data[i] = NULL;
    }

    return obj;
}

void _adtstk_freedata(adtstk* obj, unsigned int minidx,unsigned int maxidx){
    int i;

    for(i = minidx; i < maxidx +1; i++){
        SAFE_FREE(obj->data[i]);
    }
}

int adtstk_free(adtstk* obj)
{
    if(obj == NULL){
        return EXIT_FAILURE;
    }  

    _adtstk_freedata(obj,0,obj->capacity);

    SAFE_FREE(obj->data);
    SAFE_FREE(obj);

    return EXIT_SUCCESS;
}

int adtstk_size(adtstk* obj){
    if(obj == NULL){
        return EXIT_FAILURE;
    }
    
    return obj->size;
}

TYPE adtstk_top(adtstk* obj)
{
    if(obj == NULL){
        return NULL;
    } else if(obj->size == 0) {
        return NULL;
    }

    return obj->data[obj->size -1];
}

int adtstk_push(adtstk* obj, TYPE item)
{
    if(obj == NULL || item == NULL){
        return EXIT_FAILURE;
    } else if(obj->size == obj->capacity) {
        return EXIT_FAILURE;
    }

    TYPE temp_item = malloc(sizeof(item));
    memcpy(temp_item,item,sizeof(TYPE));
    obj->size++;
    obj->data[obj->size -1] = temp_item;

    return EXIT_SUCCESS;
}

int adtstk_pop(adtstk* obj){
    if(obj == NULL){
        return EXIT_FAILURE;
    } else if(obj->size == 0) {
        return EXIT_FAILURE;
    }
    
    SAFE_FREE(obj->data[obj->size -1]);
    obj->size--;

    return EXIT_SUCCESS;
}

int adtstk_resize(adtstk* obj, int newcapacity)
{
    if(obj == NULL || newcapacity <= 0){
        return EXIT_FAILURE;
    }

    int i;
    TYPE* newdata = malloc(newcapacity * sizeof(TYPE));

    for(i = 0; i < newcapacity;i++){
        //swapping pointers to items.
        newdata[i] = obj->data[i];
    }

    if((newcapacity - obj->capacity) > 0){
        if(obj->size > newcapacity){
            obj->size = newcapacity; 
        }
        _adtstk_freedata(obj,newcapacity,obj->capacity);
    }

    free(obj->data);
    obj->data = newdata;
    obj->capacity = newcapacity;

    return EXIT_SUCCESS;
}

#if DEBUG

int main()
{
    int i;

    adtstk* arr = adtstk_create(100);
    printf("init cap::%d\n",arr->capacity);

    for(i =0;i< 100;i++){
        adtstk_push(arr,&i);
    }

    adtstk_resize(arr,10);
    printf("resize cap::%d\n",arr->capacity);
    
    
    //int 
    int temp = *(int*)arr->data[9]; 
    //temp = *(int*)adtstk_top(arr); 

    printf("SIZE:::%d\n",adtstk_size(arr));  

    adtstk_pop(arr);
    printf(":::%d\n",temp);
    adtstk_pop(arr);

    adtstk_free(arr);

    return 0;
}

#endif