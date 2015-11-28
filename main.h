#ifndef TYPES_H
#define TYPES_H


typedef int BOOL;
#define TRUE 1
#define FALSE 0

#define true 1
#define false 0

#define NULL ((void*)0)

#define REG(x)   (*(volatile u32*)(x))
#define REG8(x)  (*(volatile  u8*)(x))
#define REG16(x) (*(volatile u16*)(x))
#define SW(addr, data)  *(u32*)(addr) = data

#define U64_MAX (0xFFFFFFFFFFFFFFFF)

typedef unsigned char u8;
typedef unsigned char uint8_t;

typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned int uint32_t;
typedef unsigned int size_t;

typedef unsigned long long u64;
typedef long long __int64;

typedef signed char s8;
typedef signed char bool;

typedef signed short s16;
typedef signed int s32;
typedef signed long long s64;

typedef volatile u8 vu8;
typedef volatile u16 vu16;
typedef volatile u32 vu32;
typedef volatile u64 vu64;

typedef volatile s8 vs8;
typedef volatile s16 vs16;
typedef volatile s32 vs32;
typedef volatile s64 vs64;

typedef u16 wchar;
typedef u16 wchar_t;

typedef u32 Handle;
typedef s32 Result;
typedef void (*ThreadFunc)(u32);

typedef unsigned char           bit8;
typedef unsigned short          bit16;
typedef unsigned int            bit32;
typedef unsigned long long int  bit64;


static inline char *strcpy(char *dst, char *src)
{
    char * cp = dst;  
    while( ((*cp++) = (*src++)) );        /* Copy src over dst */
    return( dst );  
}

static inline int strlen(char *s)
{
	int len;

	for (len = 0; s[len]; len++)
		;

	return len;
}

static inline char * memset(char *s, int d, int len)
{
	int i = 0;
	for(; i < len;i++)
		*(s + i) = d;
	return s + len;
}

#endif
#ifndef FS_H
#define FS_H

#define FS_OPEN_READ    1
#define FS_OPEN_WRITE   2
#define FS_OPEN_RW      3
#define FS_OPEN_CREATE  4

#define FS_ATTRIBUTE_NONE (0x00000000)
#define FS_ATTRIBUTE_READONLY (0x00000001)
#define FS_ATTRIBUTE_ARCHIVE (0x00000100)
#define FS_ATTRIBUTE_HIDDEN (0x00010000)
#define FS_ATTRIBUTE_DIRECTORY (0x01000000)

typedef enum{
	PATH_INVALID = 0,	// Specifies an invalid path.
	PATH_EMPTY = 1,		// Specifies an empty path.
	PATH_BINARY = 2,	// Specifies a binary path, which is non-text based.
	PATH_CHAR = 3,		// Specifies a text based path with a 8-bit byte per character.
	PATH_WCHAR = 4,		// Specifies a text based path with a 16-bit short per character.
}FS_pathType;

typedef struct{
	FS_pathType type;
	u32 size;
	u8* data;
}FS_path;

typedef struct{
	u32 id;
	FS_path lowPath;
	Handle handleLow, handleHigh;
}FS_archive;

static inline FS_path FS_makePath(FS_pathType type, char* path)
{
	return (FS_path){type, strlen(path)+1, (u8*)path};
}



typedef struct
{
	int titleID_low;
	int titleID_hi;
	int mediaType;
	int contentIndex;
} DataContentArchivePath;

Result Read(Handle * hd, int* rSize, int offsetLo, int offsetHi, u32* buffer, int length);
Result Close(Handle * hd);
Result GetNumContentInfos(int * outNum, int mediaType, int titleID_low, int titleID_hi);
Result ApplicationControl$$ListContentInfos(int * outNum, u8* tmdBuffer,int titleNum, int mediaType, int titleID_low, int titleID_hi, int index);
Result GetAddOnContentRequiredMemorySize(int * outMemorySize, int titleUniqueID, int contentIndex, int maxFileNum, int maxDirNum, int useCache);
Result OpenDataContent(void ** outIArchive, DataContentArchivePath * path, u32 maxFileNum, u32 maxDirNum, void * workingMemory, u32 workMemSize, int useCache);
void * ContentRomFsArchive$$AllocateBuffer();
Result OpenFileDirectly(Handle * fsuHandle, Handle * outHd, int a, int archiveID, int archivePathType, u8 * archivePathData, u32 archivePathLength,
	int filePathType, int filePathData, int filePathSize, int mode, int b);
s32 RomFsArchive$$Initialize(void* iArchive, void* iFile, u32 maxFileNum, u32 maxDirNum, void* workingMemory, u32 workMemSize, u32 useCache);
int sprintf_s(char *buf, int length ,char *fmt, ...);
Handle FsuHandleRef;

#endif