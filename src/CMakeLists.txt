
set(mse_cpp_SRCS
	dev/comm/dec/console.cpp
#	dev/cpu/alpha/axp.cpp
#	dev/cpu/alpha/ev4.cpp
#	dev/cpu/alpha/opcodes.cpp
#	dev/cpu/pdp10/opcodes.cpp
#	dev/cpu/pdp11/models.cpp
#	dev/cpu/pdp11/opcodes.cpp
	dev/cpu/vax/cis.cpp
	dev/cpu/vax/cvax.cpp
	dev/cpu/vax/disasm.cpp
	dev/cpu/vax/execute.cpp
	dev/cpu/vax/fpu.cpp
	dev/cpu/vax/ka780.cpp
	dev/cpu/vax/mem.cpp
	dev/cpu/vax/opcodes.cpp
	dev/cpu/vax/v11.cpp
	dev/cpu/vax/vax.cpp
	emu/addrmap.cpp
	emu/cmdhandler.cpp
	emu/commands.cpp
	emu/console.cpp
	emu/core.cpp
	emu/debug.cpp
	emu/devauto.cpp
	emu/devcpu.cpp
	emu/device.cpp
	emu/devsys.cpp
	emu/devtty.cpp
	emu/diexec.cpp
	emu/didebug.cpp
	emu/dimem.cpp
	emu/emucore.cpp
	emu/emufile.cpp
	emu/emumap.cpp
	emu/emumap_hedr.cpp
	emu/emumap_hedr0.cpp
	emu/emumap_hedr1.cpp
	emu/emumap_hedw0.cpp
	emu/emumap_heun.cpp
	emu/engine.cpp
	emu/exception.cpp
	emu/machine.cpp
	emu/misc.cpp
	emu/romloader.cpp
	emu/sysconfig.cpp
	emu/syslist.cpp
	emu/systems.cpp
	emu/validity.cpp
	emu/osd/ether.cpp
	emu/osd/main.cpp
	emu/osd/telnet.cpp
	emu/user.cpp
	formats/dec/image.cpp
	lib/util/delegate.cpp
	lib/util/fileio.cpp
	lib/util/unzip.cpp
	osd/osdfileio.cpp
	sys/dec/vax/ka650.cpp
)

set(mse_h_SRCS
	dev/comm/dec/cty.h
#	dev/cpu/alpha/axp.h
#	dev/cpu/alpha/ev4.h
#	dev/cpu/ibm3x0/ibm3x0.h
#	dev/cpu/ibm700/ibm700.h
#	dev/cpu/pdp10/executes.h
#	dev/cpu/pdp10/ks10.h
#	dev/cpu/pdp10/opcodes.h
#	dev/cpu/pdp10/pdp10.h
#	dev/cpu/pdp11/models.h
#	dev/cpu/pdp11/pdp11.h
	dev/cpu/vax/cvax.h
	dev/cpu/vax/fpu.h
	dev/cpu/vax/ka780.h
	dev/cpu/vax/mmu.h
	dev/cpu/vax/mtpr.h
	dev/cpu/vax/nvax.h
	dev/cpu/vax/opcodes.h
	dev/cpu/vax/v11.h
	dev/cpu/vax/vax.h
	emu/addrmap.h
	emu/badcast.h
	emu/commands.h
	emu/console.h
	emu/debug.h
	emu/devauto.h
	emu/devcore.h
	emu/devcpu.h
	emu/device.h
	emu/devsys.h
	emu/diexec.h
	emu/didebug.h
	emu/dimem.h
	emu/emucore.h
	emu/emufile.h
	emu/emumap.h
	emu/emumap_he.h
	emu/emumap_hedr.h
	emu/emumap_hedw.h
	emu/emumap_heun.h
	emu/engine.h
	emu/exception.h
	emu/machine.h
	emu/romloader.h
	emu/osd/ether.h
	emu/osd/socket.h
	emu/osd/telnet.h
	emu/sysconfig.h
	emu/syslist.h
	emu/user.h
	emu/validity.h
	formats/dec/word10.h
	lib/util/delegate.h
	lib/util/fileio.h
	lib/util/templates.h
	lib/util/unzip.h
	osd/osdfileio.h
	sys/dec/alpha/externs.h
	sys/dec/pdp10/externs.h
	sys/dec/pdp11/externs.h
	sys/dec/vax/ka650.h
	sys/dec/vax/externs.h
)

set(tools_SRCS
	tools/dump.cpp
	tools/dump10.cpp
)

include_directories(${PROJECT_SOURCE_DIR}/src ${PROJECT_SOURCE_DIR}/3rdparty )

add_executable(mse ${mse_cpp_SRCS} ${mse_h_SRCS})
target_compile_definitions(mse PRIVATE ENABLE_DEBUG)
if(WIN32)
	target_link_libraries(mse ${WSOCK32_LIBRARY} ${WS2_32_LIBRARY} ${ZLIB_LIBRARIES} )
else(WIN32)
	target_link_libraries(mse ${ZLIB_LIBRARIES} )
endif(WIN32)
