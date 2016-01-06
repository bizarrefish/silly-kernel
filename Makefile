CURRENTDIR:= `pwd`
BUILDDIR:=${CURRENTDIR}/build
INCLUDEDIR:=${CURRENTDIR}/include


CMODULES:= keyboard kernel mm video sysinterface std process
ASMODULES:= boot keyboard process


CFLAGS:= -c -g -m32
ASFLAGS:= --march=i386 --32 -g


all: compile link floppy run

#(foreach MODULE,${CMODULES}, make-$MODULE:
#	gcc ${CFLAGS} modules/c/${MODULE}/*.c \
#	-I ${INCLUDEDIR} \
#	-I modules/c/${MODULE} \
#	-o${BUILDDIR}/c-${MODULE}.o;)

#buildall: $(foreach MODULE,${CMODULES}, make-$MODULE );

compile:
	$(foreach MODULE,${CMODULES}, gcc ${CFLAGS} modules/c/${MODULE}/*.c \
						-I ${INCLUDEDIR} \
						-I modules/c/${MODULE} \
						-o${BUILDDIR}/c-${MODULE}.o;)

	$(foreach MODULE,${ASMODULES}, as ${ASFLAGS} modules/asm/${MODULE}/*.s \
						-I modules/asm/${MODULE} \
						-o ${BUILDDIR}/asm-${MODULE}.o;)

link: compile
	ld -Tlinker.ld build/*.o -o build/image -melf_i386 -g

floppy: link
	mkdir files
	sudo mount -oloop floppy.img files
	sudo cp build/image files/image
	sudo umount files
	rmdir files

run: floppy
	sudo qemu-system-x86_64 -fda floppy.img


debug: floppy
	sudo qemu-system-x86_64 -fda floppy.img -s -S &
	gdb -ex "target remote localhost:1234" --symbols=build/image

clean:
	rm build/*


