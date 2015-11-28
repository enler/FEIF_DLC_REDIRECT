#define VER10

#define FUNC(name) \
	.global name; \
	.type name, %function; \
name:

.section .text
.arm

_text_start:
FUNC(jmp_HookOpenDataContent)
	ldr pc, =HookOpenDataContent

FUNC(jmp_HookGetAddOnContentRequiredMemorySize)
	ldr pc, =HookGetAddOnContentRequiredMemorySize

FUNC(jmp_HookGetNumContentInfos)
	ldr pc, =HookGetNumContentInfos

FUNC(jmp_HookApplicationControl$$ListContentInfos)
	ldr pc, =HookApplicationControl$$ListContentInfos

FUNC(OpenDataContent)
	stmfd sp!, {r0-r11,lr}
	ldr pc, OpenDataContentAddress

FUNC(GetAddOnContentRequiredMemorySize)
	stmfd sp!, {r4,lr}
	ldr pc, GetAddOnContentRequiredMemorySizeAddress

FUNC(GetNumContentInfos)
	stmfd sp!, {r0-r6,lr}
	ldr pc, GetNumContentInfosAddress

FUNC(ApplicationControl$$ListContentInfos)
	stmfd sp!, {r4,r5,lr}
	ldr pc, ApplicationControl$$ListContentInfosAddress

FUNC(Read)
	ldr pc, ReadAddress

FUNC(Close)
	ldr pc, CloseAddress

FUNC(ContentRomFsArchive$$AllocateBuffer)
	ldr pc, ContentRomFsArchive$$AllocateBufferAddress

FUNC(ContentRomFsArchive$$ContentRomFsArchive)
	ldr pc, ContentRomFsArchive$$ContentRomFsArchiveAddress

FUNC(RomFsArchive$$Initialize)
	ldr pc, RomFsArchive$$InitializeAddress

FUNC(OpenFileDirectly)
	ldr pc, OpenFileDirectlyAddress

FUNC(sprintf_s)
	ldr pc, sprintf_sAddress

#ifdef VER10
OpenDataContentAddress:
.word 0x12A6D4
GetAddOnContentRequiredMemorySizeAddress:
.word 0x22FC0C
GetNumContentInfosAddress:
.word 0x22AB50
ApplicationControl$$ListContentInfosAddress:
.word 0x22A90C
ReadAddress:
.word 0x2315E8
CloseAddress:
.word 0x231640
ContentRomFsArchive$$AllocateBufferAddress:
.word 0x12A91C
ContentRomFsArchive$$ContentRomFsArchiveAddress:
.word 0x12AA04
RomFsArchive$$InitializeAddress:
.word 0x12A2C4
OpenFileDirectlyAddress:
.word 0x120188
sprintf_sAddress:
.word 0x3A7EB8
.global FsuHandleRef;
FsuHandleRef:
.word 0x6DC298
#endif

#ifdef VER11
OpenDataContentAddress:
.word 0x0012AD14
GetAddOnContentRequiredMemorySizeAddress:
.word 0x0023010C
GetNumContentInfosAddress:
.word 0x0022B050
ApplicationControl$$ListContentInfosAddress:
.word 0x0022AE0C
ReadAddress:
.word 0x00231AE8
CloseAddress:
.word 0x00231B40
ContentRomFsArchive$$AllocateBufferAddress:
.word 0x001209DC
ContentRomFsArchive$$ContentRomFsArchiveAddress:
.word 0x00120AC4
RomFsArchive$$InitializeAddress:
.word 0x00120454
OpenFileDirectlyAddress:
.word 0x00120BA8
sprintf_sAddress:
.word 0x003A83B0
.global FsuHandleRef;
FsuHandleRef:
.word 0x006DD2C0

#endif