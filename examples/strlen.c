size_t strlen(const char string[])
{
        size_t count = 0;
        while (string[count])
        {
                ++count;
        }
        return count;
}
