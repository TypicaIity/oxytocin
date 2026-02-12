import os
import sys
import subprocess
from glob import glob

CC = "x86_64-elf-gcc"
AS = "nasm"
LD = "x86_64-elf-ld"
QEMU = "qemu-system-x86_64"
TEA = "./tea.exe" # https://github.com/felixsidzed/tea

CCFLAGS = "-ffreestanding -fno-stack-protector -fpic -mno-red-zone -Wall -Wextra -Werror -include /opt/cross/lib/gcc/x86_64-elf/13.2.0/include/stdint-gcc.h -include src/common.h -Isrc"
ASFLAGS = ""
LDFLAGS = "-nostd -nostdlib -T linker.ld"
TEAFLAGS = "--triple x86_64-elf -v -64 -O0 -Isrc"

QEMUFLAGS = "-bios /usr/share/OVMF/OVMF_CODE_4M.fd -drive if=pflash,format=raw,readonly=on,file=/usr/share/OVMF/OVMF_CODE_4M.fd -drive if=pflash,format=raw,file=build/OVMF_VARS.fd -boot d -serial stdio"

def run(cmd: str, wait=True, **kw) -> int:
	print(cmd)
	if wait:
		try:
			return subprocess.check_call(cmd, shell=True, **kw)
		except subprocess.CalledProcessError as e:
			sys.exit(1)
			return 0
	else:
		subprocess.Popen(cmd, shell=True, **kw)
		return 0

def build(debug=False):
	bootFiles = glob(f"./boot/*.c", recursive=True)
	
	print(f"found {len(bootFiles)} bootloader source files")
	if len(bootFiles) == 0:
		sys.exit(1)

	bootObjects = []
	for file in bootFiles:
		name, ext = file.rsplit(".", 1)
		name = os.path.basename(name)
		out = os.path.join("build", f"{name}.o")
		run(f"{CC} -ffreestanding -fno-stack-protector -fpic -mno-red-zone -Wall -Wextra -Werror -I/usr/include/efi -I/usr/include/efi/x86_64 -fshort-wchar {'-g -c' if debug else '-c'} {file} -o {out}")
		bootObjects.append(out)

	boot = "build/boot.elf"
	run(f"{LD} -nostd -nostdlib -znocombreloc -T /usr/lib/elf_x86_64_efi.lds /usr/lib/crt0-efi-x86_64.o {' '.join(reversed(bootObjects))} -L/usr/lib -lefi -lgnuefi -o {boot}")

	efi = "build/BOOTX64.EFI"
	run(f"objcopy -j .text -j .sdata -j .data -j .dynamic -j .dynsym -j .rel -j .rela -j .reloc --target=efi-app-x86_64 {boot} {efi}")

	efi2 = os.path.join("build", "fatroot", "EFI", "BOOT")
	os.makedirs(efi2, exist_ok=True)
	run(f"mv {efi} {efi2}")

	os.makedirs("build/iso", exist_ok=True)
	run("cp -r build/fatroot/EFI build/iso/")

	files = []
	for ext in ("c", "s"):
		files.extend(glob(f"./src/**/*.{ext}", recursive=True))

	print(f"found {len(bootFiles)} kernel source files")
	if len(files) == 0:
		sys.exit(1)

	objects = []
	for file in files:
		name, ext = file.rsplit(".", 1)
		name = os.path.basename(name)
		out = os.path.join("build", f"{name}.o")
		if ext == "s":
			run(f"{AS} {ASFLAGS} -f elf64 {file} -o {out}")
		else:
			run(f"{CC} {CCFLAGS} {'-g -c' if debug else '-c'} {file} -o {out}")
		objects.append(out)

	kernel = "build/kernel.elf"
	run(f"{LD} {LDFLAGS} {' '.join(reversed(objects))} -L/usr/lib -lefi -lgnuefi -o {kernel}")

	run("cp -r build/kernel.elf build/iso/")

	fatimg = os.path.join("build", "iso", "boot")
	os.makedirs(fatimg, exist_ok=True)
	fatass = os.path.join(fatimg, "efiboot.img")
	run(f"dd if=/dev/zero of={fatass} bs=1M count=32")
	run(f"mkfs.vfat {fatass}")

	run(f"mmd -i {fatass} ::/EFI")
	run(f"mmd -i {fatass} ::/EFI/BOOT")
	run(f"mcopy -i {fatass} {efi2}/BOOTX64.EFI ::/EFI/BOOT/")
	run(f"mcopy -i {fatass} {kernel} ::")

	iso = os.path.join('build', 'os.iso')
	run(f"xorriso -as mkisofs -R -f -e boot/efiboot.img -no-emul-boot -isohybrid-gpt-basdat -o {iso} build/iso")

	run("cp /usr/share/OVMF/OVMF_VARS_4M.fd build/OVMF_VARS.fd")

	return iso

def main(argv) -> int:
	iso = build(len(argv) > 1 and argv[1] == "debug")
	if len(argv) <= 1:
		return 0
	
	if argv[1] == "run":
		return run(f"{QEMU} {QEMUFLAGS} -hdd build/os.iso")
	elif argv[1] == "debug":
		run(f"{QEMU} {QEMUFLAGS} -hdd {iso} -s -S", False)
		# TODO: fix
		return run(f"gdb -ex 'target remote localhost:1234' {os.path.join('build', 'kernel.elf')}", True)
	
	return 0

if __name__ == "__main__":
	sys.exit(main(sys.argv))
