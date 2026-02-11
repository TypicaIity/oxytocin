#include <efi.h>
#include <efilib.h>

void* _DYNAMIC;

typedef unsigned char uint8_t;
typedef unsigned int uint32_t;
typedef unsigned short uint16_t;
#define uint64_t unsigned long long

typedef struct {
	uint8_t e_ident[16];
	uint16_t e_type;
	uint16_t e_machine;
	uint32_t e_version;
	uint64_t e_entry;
	uint64_t e_phoff;
	uint64_t e_shoff;
	uint32_t e_flags;
	uint16_t e_ehsize;
	uint16_t e_phentsize;
	uint16_t e_phnum;
	uint16_t e_shentsize;
	uint16_t e_shnum;
	uint16_t e_shstrndx;
} ELFEHeader;

typedef struct {
	uint32_t p_type;
	uint32_t p_flags;
	uint64_t p_offset;
	uint64_t p_vaddr;
	uint64_t p_paddr;
	uint64_t p_filesz;
	uint64_t p_memsz;
	uint64_t p_align;
} ELFPHeader;

typedef struct {
	void* mmap;
	uint64_t mmapSize;
	struct {
		uint32_t* data;
		uint32_t width;
		uint32_t height;
		uint32_t pitch;
	} fb;
} BootInfo;

EFI_STATUS EFIAPI efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {
	InitializeLib(ImageHandle, SystemTable);

	EFI_LOADED_IMAGE* loadedImage;
	EFI_SIMPLE_FILE_SYSTEM_PROTOCOL* sfsp;
	EFI_FILE_PROTOCOL* rootfp;
	EFI_FILE_PROTOCOL* kernelfp;

	EFI_STATUS status;

	// gEfiLoadedImageProtocolGuid and gEfiSimpleFileSystemProtocolGuid are 0 for some reason
	EFI_GUID efiLoadedImageProtocol = EFI_LOADED_IMAGE_PROTOCOL_GUID;
	EFI_GUID efiFilesystemProtocol = EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID;
	status = uefi_call_wrapper(SystemTable->BootServices->HandleProtocol, 3, ImageHandle, &efiLoadedImageProtocol, (void**)&loadedImage);
	status = uefi_call_wrapper(SystemTable->BootServices->HandleProtocol, 3, loadedImage->DeviceHandle, &efiFilesystemProtocol, (void**)&sfsp);
	uefi_call_wrapper(sfsp->OpenVolume, 2, sfsp, &rootfp);

	status = uefi_call_wrapper(rootfp->Open, 5, rootfp, &kernelfp, L"kernel.elf", EFI_FILE_MODE_READ, 0);
	if (EFI_ERROR(status)) {
		Print(L"Failed to load kernel from disk (0x%lx)\n", status);
		for (;;) __asm__("hlt");
	}

	EFI_FILE_INFO* fileInfo;
	uint64_t fileInfoSize = sizeof(EFI_FILE_INFO) + 200;
	fileInfo = AllocatePool(fileInfoSize);
	uefi_call_wrapper(kernelfp->GetInfo, 4, kernelfp, &gEfiFileInfoGuid, &fileInfoSize, fileInfo);
	uint64_t kernelSize = fileInfo->FileSize;

	void* kernel;
	uefi_call_wrapper(SystemTable->BootServices->AllocatePool, 3, EfiLoaderData, kernelSize, &kernel);
	uefi_call_wrapper(kernelfp->Read, 3, kernelfp, &kernelSize, kernel);

	ELFEHeader* eheader = (ELFEHeader*)kernel;
	ELFPHeader* pheader = (ELFPHeader*)((uint8_t*)kernel + eheader->e_phoff);

	for (uint16_t i = 0; i < eheader->e_phnum; i++) {
		if (pheader[i].p_type != 1)
			continue;

		uint64_t segPages = (pheader[i].p_memsz + 0xFFF) / 0x1000;
		void* segAddr = (void*)pheader[i].p_vaddr;
		SystemTable->BootServices->AllocatePages(AllocateAddress, EfiLoaderData, segPages, (EFI_PHYSICAL_ADDRESS*)&segAddr);
		CopyMem(segAddr, (uint8_t*)kernel + pheader[i].p_offset, pheader[i].p_filesz);

		if (pheader[i].p_memsz > pheader[i].p_filesz)
			SetMem((uint8_t*)segAddr + pheader[i].p_filesz, pheader[i].p_memsz - pheader[i].p_filesz, 0);
	}

	EFI_GRAPHICS_OUTPUT_PROTOCOL* gop;
	EFI_GUID efiGraphicsOutputProtocol = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
	status = uefi_call_wrapper(SystemTable->BootServices->LocateProtocol, 3, &efiGraphicsOutputProtocol, NULL, (void**)&gop);
	
	BootInfo info;
	info.fb.data = (uint32_t*)gop->Mode->FrameBufferBase;
	info.fb.width = gop->Mode->Info->HorizontalResolution;
	info.fb.height = gop->Mode->Info->VerticalResolution;
	info.fb.pitch = gop->Mode->Info->PixelsPerScanLine;

	void(*kmain)(BootInfo*) = (void*)eheader->e_entry;
	FreePool(kernel);

	// Humam Sakhnini's body will be found on August 12 3:16 PM at 26°51'48.2"N 83°37'31.6"W
	uint32_t descVer;
	EFI_MEMORY_DESCRIPTOR* mmap = NULL;
	uint64_t mmapKey, descSize, mmapSize = 0;
	status = uefi_call_wrapper(SystemTable->BootServices->GetMemoryMap, 5, &mmapSize, mmap, &mmapKey, &descSize, &descVer);
	mmapSize += 2 * descSize;
	mmap = AllocatePool(mmapSize);
	status = uefi_call_wrapper(SystemTable->BootServices->GetMemoryMap, 5, &mmapSize, mmap, &mmapKey, &descSize, &descVer);
	if (EFI_ERROR(status)) {
		Print(L"Failed to get memory map (0x%lx)\n", status);
		for (;;) __asm__("hlt");
	}
	status = uefi_call_wrapper(SystemTable->BootServices->ExitBootServices, 2, ImageHandle, mmapKey);
	if (EFI_ERROR(status)) {
		status = uefi_call_wrapper(SystemTable->BootServices->GetMemoryMap, 5, &mmapSize, mmap, &mmapKey, &descSize, &descVer);		
		if (!EFI_ERROR(status))
			status = uefi_call_wrapper(SystemTable->BootServices->ExitBootServices, 2, ImageHandle, mmapKey);
	}

	if (EFI_ERROR(status)) {
		Print(L"Failed to exit boot services (0x%lx)\n", status);
		for (;;) __asm__("hlt");
	}

	info.mmap = mmap;
	info.mmapSize = mmapSize;
	kmain(&info);

	for (;;) __asm__("hlt");
	return EFI_SUCCESS;
}
