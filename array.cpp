#include <fxcg/heap.h>

class Array
{
    public:
        void **data = nullptr;
        unsigned char size = 0;
        void add(void *ptr);
        void remove(void *ptr);
        void clear();
};

void Array::add(void *ptr)
{
    void **temp = (void **)sys_malloc(sizeof(void *) * (size + 1));
    for (int i = 0; i < size; i++)
    {
        temp[i] = data[i];
    }
    sys_free(data);
    temp[size] = ptr;
    size++;
    data = temp;
}

void Array::remove(void *ptr)
{
    void **temp = (void **)sys_malloc(sizeof(void *) * (size - 1));
    for (int i = 0; i < size; i++)
    {
        if (data[i] != ptr)
        {
            *temp++ = data[i];
        }
    }
    sys_free(data);
    sys_free(ptr);
    size--;
    data = temp - size;
}

void Array::clear()
{
    for (int i = 0; i < size; i++)
    {
        sys_free(data[i]);
    }
    sys_free(data);
    data = nullptr;
    size = 0;
}
