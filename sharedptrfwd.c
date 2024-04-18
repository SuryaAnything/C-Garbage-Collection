#include "sharedptrfwd.h"
#include <stdlib.h>

void __event_analysis_generator(char *event);
void cleanup() {
    __event_analysis_generator("Program ended");
}

pthread_mutex_t _base_mutex;
synchronized_ptr* __mt_cur_base_cx86 = {0};  // Multi-Threaded Base Current Context for x86
synchronized_ptr* __cr_twf_utop_cx86 = {0};  // Current Thread Wait-Free Top Context for x86

_SHARED_DECL __attribute__ ((__pure__))
synchronized_ptr* sync_with_dynamic_allocator(int _Size) {
    __event_analysis_generator("Allocating memory with synchronization");
    void *_mem = calloc(_Size, 1);
    synchronized_ptr* _ptr = malloc(sizeof(synchronized_ptr));
    _ptr->ref_cnt = 1; _ptr->acnt = _mem; _ptr->_ust = NULL;
    _ptr->static_cnt = 0;
    memset(_ptr->_collector,0,32*sizeof(struct _shared_allocator_build**));
    return _ptr;
}

_SHARED_DECL
void allocator_load(synchronized_ptr** _adrs) {
    pthread_mutex_lock(&_base_mutex);
    __event_analysis_generator("Loading allocator");
    (*_adrs)->_collector[(*_adrs)->static_cnt++] = _adrs;
    if(!__mt_cur_base_cx86) { 
        __mt_cur_base_cx86 = __cr_twf_utop_cx86 = *_adrs;
    }
    else {
        __cr_twf_utop_cx86->_ust = *_adrs;
        __cr_twf_utop_cx86 = *_adrs;
    }
    pthread_mutex_unlock(&_base_mutex);
}

_SHARED_DECL 
void move_ref(synchronized_ptr** _Src, synchronized_ptr** _Dest) {
    int _et = 0;
    pthread_mutex_lock(&_base_mutex);
    __event_analysis_generator("Moving reference");
    synchronized_ptr *_Tp = *_Src;
    if (_Tp->ref_cnt>0) {for(;_et<32;++_et) {
        if (_Tp->_collector[_et]==_Src) { break; }
    }
    if (_et<32) { 
        while (_et<(32-1)) {
            _Tp->_collector[_et] = _Tp->_collector[_et+1];
            ++_et;
        }
        _Tp->static_cnt--;
        _Tp->ref_cnt--;
    }}
    
    _Tp = *_Dest;

    _Tp->_collector[_Tp->static_cnt++] = _Src;
    _Tp->ref_cnt++;

    *_Src = *_Dest;
    
    pthread_mutex_unlock(&_base_mutex);
}

#ifdef SYNCHRONIZED_CONTROLLER
_SHARED_DECL 
void* __mgc_parallel_resource_controller() {
    __event_analysis_generator("Garbage collection initiated");
    while (__forever__){
        synchronized_ptr* _Tp = __mt_cur_base_cx86;
        synchronized_ptr* _pre = NULL;
        while (_Tp) {

            if (!_Tp->ref_cnt) {
                __event_analysis_generator(" % Collecting garbage % ");
                if (!_pre) { __mt_cur_base_cx86 = _Tp->_ust; }
                else { _pre->_ust = _Tp->_ust; }
                synchronized_ptr* _throwable = _Tp; _Tp = _Tp->_ust;
                free(_throwable->acnt);
            }
            else { _pre = _Tp; _Tp = _Tp->_ust; }        
        }
        __cr_twf_utop_cx86 = _pre;
        
    }
}
#endif

_SHARED_DECL 
void __attribute__((constructor)) __main__() {
    __event_analysis_generator("Program started");
    pthread_t thread;
    pthread_mutex_init(&_base_mutex, NULL);
    pthread_create(&thread, NULL, __mgc_parallel_resource_controller, NULL);
}

_SHARED_DECL 
void __attribute__((destructor)) __returned__() {
    pthread_mutex_destroy(&_base_mutex);
    atexit(cleanup);
    __event_analysis_generator("Program ended");
}

_SUPPRESS_UNUSED
struct _mem_tf_info get_memory_ref_count() {
    struct _mem_tf_info info;
    __event_analysis_generator("Getting memory reference count");
    synchronized_ptr* _Tp = __mt_cur_base_cx86;
    int _et = 0;
    while (_Tp) {
        info._mem_arr[_et++] = _Tp->ref_cnt;
        _Tp = _Tp->_ust;
    }
    info.cnt = _et;
    return info;
}

_SHARED_DECL
void sharedAdjList() {
    synchronized_ptr* _Tp = __mt_cur_base_cx86;
    int _et = 1;
    while (_Tp) {
        printf(" pointers ( %d ): ",_et++);
        for (int i = 0; i < _Tp->static_cnt; i++) {
            printf("%p ",_Tp->_collector[i]);
        }
        printf("\n");
        
        _Tp = _Tp->_ust;
    }
}


#define LOG_FILE_NAME "last_test.log"

FILE *log_file;

ULARGE_INTEGER createTimeValue, exitTimeValue, kernelTimeValue, userTimeValue;


void calculate_system_stats() {
    FILETIME createTime, exitTime, kernelTime, userTime;
    if (GetProcessTimes(GetCurrentProcess(), &createTime, &exitTime, &kernelTime, &userTime)) {
        SYSTEM_INFO sysInfo;
        FILETIME nowTime;
        ULARGE_INTEGER now;

        GetSystemInfo(&sysInfo);
        GetSystemTimeAsFileTime(&nowTime);

        memcpy(&now, &nowTime, sizeof(FILETIME));
        memcpy(&createTimeValue, &createTime, sizeof(FILETIME));
        memcpy(&exitTimeValue, &exitTime, sizeof(FILETIME));
        memcpy(&kernelTimeValue, &kernelTime, sizeof(FILETIME));
        memcpy(&userTimeValue, &userTime, sizeof(FILETIME));
    }
}


void __event_analysis_generator(char *event) {
    time_t current_time;
    char* c_time_string;
    current_time = time(NULL);
    c_time_string = ctime(&current_time);
    c_time_string[strlen(c_time_string)-1] = '\0';
    static int __begin__stats = 1;
    if (__begin__stats == 1) {
        calculate_system_stats();
    }

    MEMORYSTATUSEX mem_info;
    mem_info.dwLength = sizeof(mem_info);
    GlobalMemoryStatusEx(&mem_info);

    log_file = fopen(LOG_FILE_NAME, "a");
    if (log_file == NULL) {
        perror("Error opening log file");
        exit(EXIT_FAILURE);
    }

    
    if (__begin__stats) {
        fprintf(log_file, "Total Physical Memory: %.2f MB\n", mem_info.ullTotalPhys / (1024.0 * 1024));
        fprintf(log_file, "Available Physical Memory: %.2f MB\n", mem_info.ullAvailPhys / (1024.0 * 1024));
        fprintf(log_file, "Total Virtual Memory: %.2f MB\n", mem_info.ullTotalVirtual / (1024.0 * 1024));
        fprintf(log_file, "Available Virtual Memory: %.2f MB\n\n", mem_info.ullAvailVirtual / (1024.0 * 1024));
        __begin__stats = 0;
    }

    fprintf(log_file, "[%s]\n \n ***************{%s}***************\n\n", c_time_string, event);
    fclose(log_file);
}