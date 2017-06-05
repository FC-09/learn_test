#include <iostream>

class File
{
public:
    File();

    ~File();

    int ReadFile(char*);

    int WriteFile(char*);

private:
    char file_buf_[30 * 1024];
    FILE* f_;
};