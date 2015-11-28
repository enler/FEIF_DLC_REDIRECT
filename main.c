#include "main.h"

#define FEIF_DLC_TITLE_ID_HI 0x0004008C
#define FEIF_DLC_TITLE_ID_LOW 0x0012DC00

#define GET_VIRTUAL_FUNCTION(obj, off) (void*)(*(u32*)(*(u32*)obj + off))

#define IFILE_VIRTUAL_FUNCTION_TABLE_LEN 0x19
#define INDEX_OF_TRYREAD_IN_IFILE_VIRTUAL_FUNCTION_TABLE 0

#define ROMFS_LEVEL3_HEADER_LENGTH 0x28
#define ROMFS_LEVEL3_OFFSET 0x1000

int sdf_open(char *filename, int mode)
{
    Handle hd;
    Result retv;
    FS_path filePath;
    filePath.type = PATH_CHAR;
    filePath.size = strlen(filename) + 1;
    filePath.data = (u8*)filename;
    retv = OpenFileDirectly((Handle*)FsuHandleRef, &hd, 0, 9, PATH_EMPTY, (u8*)"", 1, filePath.type, filePath.data, filePath.size, mode, 0);
    if(retv < 0)
   	{
       	return retv;
    }
    return hd;
}

Handle OpenDLCFromSD(int contentIndex)
{
	u8 path[64];
	sprintf_s(path, 64, "/FEIF/%08X.bin", contentIndex);
	return sdf_open(path, 1);
}

static s32 (*IFile$$TryRead)(void * romfsIFile, s32* pOut, s64 offset, void* buffer, size_t size);

s32 hook_IFile$$TryRead(void * romfsIFile, s32* pOut, s64 offset, void* buffer, size_t size)
{
	return IFile$$TryRead(romfsIFile, pOut, offset + ROMFS_LEVEL3_OFFSET, buffer, size);
}

void fakingIFileVirtualFunctionTable(void * romfsIFile)
{
	static u32 fakeIFileVirtualFunctionTable[IFILE_VIRTUAL_FUNCTION_TABLE_LEN];
	u32 * virtualFunctionTable = (u32*)(*(u32*)romfsIFile);
	int index = 0;
	while(index < IFILE_VIRTUAL_FUNCTION_TABLE_LEN)
		fakeIFileVirtualFunctionTable[index++] = *(virtualFunctionTable++);
	IFile$$TryRead = (void*)fakeIFileVirtualFunctionTable[INDEX_OF_TRYREAD_IN_IFILE_VIRTUAL_FUNCTION_TABLE];
	fakeIFileVirtualFunctionTable[INDEX_OF_TRYREAD_IN_IFILE_VIRTUAL_FUNCTION_TABLE] = (u32)hook_IFile$$TryRead;
	*(u32*)romfsIFile = (u32)&fakeIFileVirtualFunctionTable;
}

Result HookOpenDataContent(void ** outIArchive, DataContentArchivePath * path, u32 maxFileNum, u32 maxDirNum, void * workingMemory, u32 workMemSize, int useCache)
{
	int contentIndex, titleID_low, titleID_hi;
	Handle dlcFile;
	contentIndex = path->contentIndex;
	titleID_hi = path->titleID_hi;
	titleID_low = path->titleID_low;
	if (titleID_hi == FEIF_DLC_TITLE_ID_HI && titleID_low == FEIF_DLC_TITLE_ID_LOW)
	{
		if ((int)(dlcFile = OpenDLCFromSD(contentIndex)) >= 0)
		{
			void * dlcRomfsIArchive = ContentRomFsArchive$$AllocateBuffer();
			memset(dlcRomfsIArchive, 0, 0x130);
			ContentRomFsArchive$$ContentRomFsArchive(dlcRomfsIArchive);
			void * dlcRomfsIFile = (void*)0;
			s32 (*ContentRomFsArchive$$OpenDirect)(void*, void**, u32) = GET_VIRTUAL_FUNCTION(dlcRomfsIArchive, 0x3C);
			if (!ContentRomFsArchive$$OpenDirect(dlcRomfsIArchive, &dlcRomfsIFile, dlcFile))
			{
				fakingIFileVirtualFunctionTable(dlcRomfsIFile);
				if (RomFsArchive$$Initialize(
						dlcRomfsIArchive,
						dlcRomfsIFile,
						maxFileNum,
						maxDirNum,
						workingMemory,
						workMemSize,
						useCache))
					{
						void (*File$$Close)(void*) = GET_VIRTUAL_FUNCTION(dlcRomfsIFile, 0x2C);
						File$$Close(dlcRomfsIFile);
					}
					else
					{
						*outIArchive = dlcRomfsIArchive;
						return 0;
					}
			}
			else
			{
				void (*ContentRomFsArchive$$DeleteObject)(void*) = GET_VIRTUAL_FUNCTION(dlcRomfsIArchive, 0x30);
				ContentRomFsArchive$$DeleteObject(dlcRomfsIArchive);
			}
		}
	}
	return OpenDataContent(outIArchive, path, maxFileNum, maxDirNum, workingMemory, workMemSize, useCache);
}

Result HookGetAddOnContentRequiredMemorySize(int * outMemorySize, int titleUniqueID, int contentIndex, int maxFileNum, int maxDirNum, int useCache)
{
	if (titleUniqueID == (FEIF_DLC_TITLE_ID_LOW >> 8))
	{
		if (useCache)
		{
			Handle dlcFile;
			if ((int)(dlcFile = OpenDLCFromSD(contentIndex)) >= 0)
			{
				u32 romfsHeader[ROMFS_LEVEL3_HEADER_LENGTH / 4];
				int readedSize = 0;
				int size = maxFileNum * 0x20 + maxDirNum * 0x18;
				if (!Read(&dlcFile, &readedSize, ROMFS_LEVEL3_OFFSET, 0, romfsHeader, 0x28))
				{
					size += romfsHeader[2] + romfsHeader[4] + romfsHeader[6] + romfsHeader[8];
				}
				Close(&dlcFile);
				*outMemorySize = size;
				return 0;
			}
		}
		else
		{
			*outMemorySize = maxFileNum * 0x20 + maxDirNum * 0x18;
			return 0;
		}
	}
	return GetAddOnContentRequiredMemorySize(outMemorySize, titleUniqueID, contentIndex, maxFileNum, maxDirNum, useCache);
}

Result HookGetNumContentInfos(int * outNum, int mediaType, int titleID_low, int titleID_hi)
{
	if (titleID_low == FEIF_DLC_TITLE_ID_LOW && titleID_hi == FEIF_DLC_TITLE_ID_HI)
	{
		int hd;
		if ((hd = sdf_open("/FEIF/DLCInfo.bin", 1)) >= 0)
		{
			int readedSize = 0;
			int dlcLength = 0;
			if(!Read(&hd, &readedSize, 0, 0, &dlcLength, 0x4))
			{
				*outNum = dlcLength;
				Close(&hd);
				return 0;
			}
			Close(&hd);
		}
	}
	return GetNumContentInfos(outNum, mediaType, titleID_low, titleID_hi);
}

Result HookApplicationControl$$ListContentInfos(int * outNum, u8* tmdBuffer,int titleNum, int mediaType, int titleID_low, int titleID_hi, int index)
{
	if (titleID_low == FEIF_DLC_TITLE_ID_LOW && titleID_hi == FEIF_DLC_TITLE_ID_HI)
	{
		int hd;
		if ((hd = sdf_open("/FEIF/DLCInfo.bin", 1)) >= 0)
		{
			int readedSize = 0;
			int dlcLength = 0;
			
			if (!Read(&hd, &readedSize, 4, 0, tmdBuffer, titleNum * 0x18))
			{
				*outNum = titleNum;
				Close(&hd);
				return 0;
			}
			Close(&hd);
		}
	}
	return ApplicationControl$$ListContentInfos(outNum, tmdBuffer, titleNum, mediaType, titleID_low, titleID_hi, index);
}
