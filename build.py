import os
import sys
import time
import subprocess
from glob import glob
from tempfile import TemporaryDirectory

CC = "x86_64-elf-gcc"
AS = "nasm"
LD = "x86_64-elf-ld"

CCFLAGS = "-ffreestanding -std=gnu99 -O2 -Isrc -mcmodel=kernel -fno-stack-protector -mno-red-zone"
ASFLAGS = ""
LDFLAGS = "-nostd -T linker.ld"

def run(cmd: str, **kw) -> int:
	try:
		print(cmd)
		subprocess.check_call(cmd.split(" "), **kw)
	except subprocess.CalledProcessError:
		sys.exit(1)

def build():
	files = []
	for ext in ("c", "s"):
		files.extend(glob(f"./**/*.{ext}", recursive=True))

	print(f"found {len(files)} source files")

	objects = []
	for file in files:
		name, ext = file.rsplit(".", 1)
		if ext != "s":
			continue
		name = os.path.basename(name)

		out = os.path.join("build", f"{name}")

		if name in ("stage1", "stage2"):
			run(f"{AS} {ASFLAGS} -f bin {file} -o {out}.bin")
		else:
			run(f"{AS} {ASFLAGS} -f elf64 {file} -o {out}.o")
			out += ".o"
			if name == "stage3": objects.insert(0, out)
			else: objects.append(out)

	for file in files:
		name, ext = file.rsplit(".", 1)
		if ext != "c":
			continue
		name = os.path.basename(name)

		out = os.path.join("build", f"{name}.o")
		run(f"{CC} {CCFLAGS} -c {file} -o {out}")
		objects.append(out)

	iso = os.path.join("build", "os.iso")
	stage1 = os.path.join("build", "stage1.bin")
	stage2 = os.path.join("build", "stage2.bin")
	kernel = os.path.join("build", "kernel.raw")

	if len(objects) == 0:
		print("no object files to link")
		return 1

	_ = os.path.join("build", "kernel.bin")
	run(f"{LD} {LDFLAGS} -o {_} {' '.join(objects)}")
	run(f"objcopy -O binary {_} {kernel}")
    
	with TemporaryDirectory() as tmp:
		bootDir = os.path.join(tmp, "boot")
		os.makedirs(bootDir, exist_ok=True)
		
		image = os.path.join(bootDir, "image.bin")
		
		with open(image, "wb") as outfile:
			with open(stage1, "rb") as infile:
				outfile.write(infile.read())
			
			with open(stage2, "rb") as infile:
				outfile.write(infile.read())
			
			pos = outfile.tell()
			if pos % 512 != 0:
				outfile.write(b'\0' * (512 - (pos % 512)))
				pos = outfile.tell()
			
			print(f"kernel lba: {pos // 512}")
			
			with open(kernel, "rb") as infile:
				outfile.write(infile.read())
		
		run(
			f"xorriso -as mkisofs -b boot/{os.path.basename(image)} -c boot/boot.cat -no-emul-boot -boot-load-size 4 -boot-info-table -isohybrid-mbr {image} -o {iso} {tmp}",
			stdout=subprocess.DEVNULL,
			stderr=subprocess.DEVNULL,
			stdin=subprocess.DEVNULL,
		)
	return iso

def main(argv) -> int:
	iso = build()
	if len(argv) <= 1:
		return 0

	if argv[1] == "run":
		return run(f"qemu-system-x86_64 -hdd {iso} -net none")
	
	return 0

if __name__ == "__main__":
	sys.exit(main(sys.argv))
