#pragma once

char* concat_str(const char* str1, const char* str2);
char* color(int code);
void remove_directory(const char* path);
void copy_file(const char* src, const char* dest);
void copy_directory(const char* src, const char* dest);
int confirm();