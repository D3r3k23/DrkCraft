namespace DrkCraft
{
#ifdef EN_LOG
    #define LOG()

#elif
    #define LOG()
#endif // EN_LOG

    class Log
    {
        static void init();
    };
}
