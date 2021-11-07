
#if defined(EN_LOG)
    #define LOG()

#else
    #define LOG()
#endif // EN_LOG

namespace DrkCraft
{

    class Log
    {
        static void init();
    };
}
