#ifndef ABC_H
#define ABC_H

// ---------- <dyn_array.h> ----------
#ifndef DYN_ARRAY_H
#define DYN_ARRAY_H

#include <assert.h>
#include <stdlib.h>

#define DEFINE_DYN_ARRAY(type, name) \
    typedef struct \
    { \
        size_t size, capacity; \
        type* data; \
    } name; \
    static inline void name##_alloc(name* arr, size_t capacity) \
    { \
        assert(arr != NULL); \
        arr->size     = 0; \
        arr->data     = malloc(capacity * sizeof(*arr->data)); \
        assert(arr->data != NULL); \
        arr->capacity = capacity; \
    } \
    static inline void name##_free(name* arr) \
    { \
        assert(arr != NULL); \
        assert(arr->data != NULL); \
        free(arr->data); \
    } \
    static inline void name##_realloc(name* arr, size_t capacity) \
    { \
        assert(arr != NULL); \
        if (arr->capacity == capacity) \
            return; \
        arr->data     = realloc(arr->data, capacity * sizeof(*arr->data)); \
        assert(arr->data != NULL); \
        arr->capacity = capacity; \
    } \
    static inline void name##_append(name* arr, type val) \
    { \
        assert(arr != NULL); \
        while (arr->size >= arr->capacity) \
            name##_realloc(arr, arr->capacity * 2); \
        arr->data[arr->size++] = val; \
    }

#endif // DYN_ARRAY_H
// ---------- </dyn_array.h> ----------

#include <stdint.h>
#include <stdio.h>

#define ABC_LOG(msg)           printf("[abc] " msg "\n")
#define ABC_LOGCLK(name, time) printf("[abc] @%s: %.3fs.\n", name, time)
#define ABC_LOGVAR(fmt, name)  printf("[abc] %s: " fmt "\n", #name, name)
#define ABC_LOGERR(msg)        printf("[abc] ERROR (at %s): " msg "\n", __func__)

#define ABC_MSG_INVALID_CLOCK "Invalid clock!"

typedef uint64_t abc_clock_t;
typedef double abc_second_t;

typedef struct
{
    abc_clock_t id;
    const char* name;
    abc_second_t total, start;
} ABClock;

DEFINE_DYN_ARRAY(ABClock, dyn_array_abclock)

/*
#ifdef _WIN32
#include <windows.h>
static inline abc_second_t abc_now()
{
    static int is_frequency_initialized = 0;
    static LARGE_INTEGER frequency;
    if (!is_frequency_initialized)
    {
        QueryPerformanceFrequency(&frequency); 
        is_frequency_initialized = 1;
    }
    LARGE_INTEGER now;
    QueryPerformanceCounter(&now);
    return (abc_second_t)now.QuadPart / (abc_second_t)frequency.QuadPart;
}
#else
*/
#include <time.h>
static inline abc_second_t abc_now()
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec * 1e-9;
}
/*
#endif
*/

static inline dyn_array_abclock* abc_clock_central(void)
{
    static int is_clock_initialized = 0;
    static dyn_array_abclock clock_central;
    if (!is_clock_initialized)
    {
        dyn_array_abclock_alloc(&clock_central, 16);
        is_clock_initialized = 1;
    }
    return &clock_central;
}

static inline void abc_clock_central_free(void)
{
    dyn_array_abclock* clock_central = abc_clock_central();
    if (clock_central)
        dyn_array_abclock_free(clock_central);
}

static inline abc_clock_t abc_clock_register(const char* name)
{
    dyn_array_abclock* clock_central = abc_clock_central();
    const abc_clock_t id             = clock_central->size;
    dyn_array_abclock_append(clock_central, (ABClock){
        .id    = id,
        .name  = name,
        .total = 0,
        .start = 0,
    });
    return id;
}

static inline void abc_clock_start(abc_clock_t id)
{
    dyn_array_abclock* clock_central = abc_clock_central();
    if (id >= clock_central->size)
    {
        ABC_LOGERR(ABC_MSG_INVALID_CLOCK);
        return;
    }
    clock_central->data[id].start = abc_now();
}

static inline abc_second_t abc_clock_stop(abc_clock_t id)
{
    dyn_array_abclock* clock_central = abc_clock_central();
    if (id >= clock_central->size)
    {
        ABC_LOGERR(ABC_MSG_INVALID_CLOCK);
        return 0;
    }
    ABClock* clock             = &clock_central->data[id];
    const abc_second_t delta_t = abc_now() - clock->start;
    clock->start = 0;
    clock->total += delta_t;
    return delta_t;
}

static inline void abc_clock_reset(abc_clock_t id)
{
    dyn_array_abclock* clock_central = abc_clock_central();
    if (id >= clock_central->size)
    {
        ABC_LOGERR(ABC_MSG_INVALID_CLOCK);
        return;
    }
    ABClock* clock = &clock_central->data[id];
    clock->start = 0;
    clock->total = 0;
}

static inline void abc_clock_log(abc_clock_t id)
{
    dyn_array_abclock* clock_central = abc_clock_central();
    if (id >= clock_central->size)
    {
        ABC_LOGERR(ABC_MSG_INVALID_CLOCK);
        return;
    }
    const ABClock* clock       = &clock_central->data[id];
    const abc_second_t delta_t = abc_now() - clock->start;
    ABC_LOGCLK(clock->name, delta_t);
}

#endif // ABC_H

