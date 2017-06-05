#include "File.h"

File::File()
{
}

File::~File()
{
}

int File::ReadFile(char* file_path)
{
    memset(file_buf_, 0, sizeof(file_buf_));
    int read_len = 0;
    if (f_ = fopen(file_path, "rb"))
    {
        read_len = fread(file_buf_, sizeof(char), sizeof(file_buf_), f_);
        fclose(f_);
    }
    return read_len;
}

int File::WriteFile(char* file_path)
{
    memset(file_buf_, 0, sizeof(file_buf_));
    int write_len = 0;
    if (f_ = fopen(file_path, "wb"))
    {
        write_len = fwrite(file_buf_, sizeof(char), sizeof(file_buf_), f_);
        fclose(f_);
    }
    return write_len;
}