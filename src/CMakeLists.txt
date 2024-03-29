
set(mse_cpp_SRCS
	dev/chip/dec/vax/cmctl.cpp
	dev/chip/dec/vax/cqbic.cpp
	dev/chip/dec/vax/cssc.cpp
	dev/comm/dec/console.cpp
#	dev/cpu/alpha/axp.cpp
#	dev/cpu/alpha/ev4.cpp
#	dev/cpu/alpha/opcodes.cpp
	dev/cpu/pdp10/execute.cpp
	dev/cpu/pdp10/opcode.cpp
	dev/cpu/pdp10/memory.cpp
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
	emu/dinvram.cpp
	emu/emucore.cpp
	emu/emufile.cpp
	emu/map/map.cpp
	emu/map/access.cpp
	emu/map/hem.cpp
	emu/map/hedr0.cpp
	emu/map/hedr1.cpp
	emu/map/hedr2.cpp
	emu/map/hedr3.cpp
	emu/map/hedw0.cpp
	emu/map/hedw1.cpp
	emu/map/hedw2.cpp
	emu/map/hedw3.cpp
	emu/map/heun.cpp
	emu/engine.cpp
	emu/exception.cpp
	emu/machine.cpp
	emu/misc.cpp
	emu/romloader.cpp
	emu/sysconfig.cpp
	emu/syslist.cpp
	emu/systems.cpp
	emu/validity.cpp
	emu/user.cpp
	formats/dec/image.cpp
	lib/util/delegate.cpp
	lib/util/corefile.cpp
	lib/util/unzip.cpp
	osd/ether.cpp
	osd/main.cpp
#	osd/osdfile.cpp
	osd/posixfile.cpp
	osd/telnet.cpp
	sys/dec/vax/ka650.cpp
)

set(mse_h_SRCS
	dev/chip/dec/vax/cmctl.h
	dev/chip/dec/vax/cssc.h
	dev/chip/dec/vax/cqbic.h
	dev/comm/dec/cty.h
#	dev/cpu/alpha/axp.h
#	dev/cpu/alpha/ev4.h
#	dev/cpu/ibm3x0/ibm3x0.h
#	dev/cpu/ibm700/ibm700.h
	dev/cpu/pdp10/opcode.h
	dev/cpu/pdp10/kx10.h
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
	emu/dinvram.h
	emu/emucore.h
	emu/emufile.h
	emu/map/map.h
	emu/map/he.h
	emu/map/hea.h
	emu/map/hem.h
	emu/map/hedr.h
	emu/map/hedri.h
	emu/map/hedw.h
	emu/map/hedwi.h
	emu/map/heun.h
	emu/engine.h
	emu/exception.h
	emu/machine.h
	emu/romloader.h
	emu/sysconfig.h
	emu/syslist.h
	emu/user.h
	emu/validity.h
	formats/dec/word10.h
	lib/util/delegate.h
	lib/util/corefile.h
	lib/util/templates.h
	lib/util/unzip.h
	osd/ether.h
	osd/socket.h
	osd/osdfile.h
	osd/posixfile.h
	osd/telnet.h
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
