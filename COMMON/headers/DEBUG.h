#ifdef DEBUG
    #define ON_DEBUG(...)  __VA_ARGS__    
#else
    #define ON_DEBUG(...)
#endif 