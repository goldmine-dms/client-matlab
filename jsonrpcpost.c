#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifdef _MSC_VER
#define CURL_STATICLIB
#endif

#include <curl/curl.h>
#include "mex.h"

#define CONTENT_TYPE = application/json-rpc

struct string {
    char *ptr;
    size_t len;
};

void init_string(struct string *s) {
    s->len = 0;
    s->ptr = malloc(s->len+1);
    if (s->ptr == NULL) {
        fprintf(stderr, "malloc() failed\n");
        exit(EXIT_FAILURE);
    }
    s->ptr[0] = '\0';
}

size_t writefunc(void *ptr, size_t size, size_t nmemb, struct string *s)
{
    size_t new_len = s->len + size*nmemb;
    s->ptr = realloc(s->ptr, new_len+1);
    if (s->ptr == NULL) {
        fprintf(stderr, "realloc() failed\n");
        exit(EXIT_FAILURE);
    }
    memcpy(s->ptr+s->len, ptr, size*nmemb);
    s->ptr[new_len] = '\0';
    s->len = new_len;

    return size*nmemb;
}

char* query(char *json, char *service){

    CURL *curl;
    CURLcode res;
    struct curl_slist *hl = NULL; 
    struct string s;
    
    curl = curl_easy_init();
    init_string(&s);

    if(curl) {

        curl_easy_setopt(curl, CURLOPT_URL, service); 
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json);

        hl = curl_slist_append(hl, "Content-type: CONTENT_TYPE");

        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, hl); 

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &s);

        res = curl_easy_perform(curl);
        if(res != 0){
             mexErrMsgTxt(curl_easy_strerror(res));
        }
        curl_easy_cleanup(curl);
        curl_slist_free_all (hl); 
    }

    return s.ptr;
}

void mexFunction (int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[])
{

    char *json, *service, *ret;
    mwSize jsonlen, servicelen;

    if (nrhs != 2) { 
        mexErrMsgTxt("Two input arguments required.");

    }   
    if (nlhs > 1) {
        mexErrMsgTxt("Too many output arguments.");
    }   

    if (!mxIsChar(prhs[0]) || (mxGetM(prhs[0]) != 1 ) )  {
        mexErrMsgTxt("JSON must be a string.");
    }   

    if (!mxIsChar(prhs[1]) || (mxGetM(prhs[1]) != 1 ) )  {
        mexErrMsgTxt("Service URL must be a string.");
    }   

    jsonlen = mxGetN(prhs[0])*sizeof(mxChar)+1;
    json = mxMalloc(jsonlen);

    servicelen = mxGetN(prhs[1])*sizeof(mxChar)+1;
    service = mxMalloc(servicelen);

    mxGetString(prhs[0], json, jsonlen); 
    mxGetString(prhs[1], service, servicelen); 

    ret = query(json, service);

    plhs[0] = mxCreateString(ret);

    free(ret);
}

