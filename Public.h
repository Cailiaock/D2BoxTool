char * strsplit(const char * _Str, const char * _Split, const int _StrLen, const int _SplitLen, int * _BeginIndex, int * _ReturnLen);

bool CharsCompare(const char * _Buffer1, const char * _Buffer2, long _Size);

bool FileCompare(const char * _FileName1, const char * _FileName2);

char * ReadAllFile(const char * _FileName, const char * _Mode, long * _Size);

void WriteLog(const char * _Str);