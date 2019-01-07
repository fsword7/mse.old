
set(mse_cpp_SRCS
	dev/comm/dec/console.cpp
	dev/cpu/alpha/axp.cpp
	dev/cpu/alpha/ev4.cpp
	dev/cpu/alpha/opcodes.cpp
	dev/cpu/pdp10/opcodes.cpp
	dev/cpu/pdp11/models.cpp
	dev/cpu/pdp11/opcodes.cpp
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
	emu/commands.cpp
	emu/console.cpp
	emu/core.cpp
	emu/debug.cpp
	emu/devcpu.cpp
	emu/devsys.cpp
	emu/devtty.cpp
	emu/misc.cpp
	emu/osd/ether.cpp
	emu/osd/main.cpp
	emu/osd/telnet.cpp
	emu/user.cpp
	formats/dec/image.cpp
	sys/drivers/alpha.cpp
	sys/drivers/hsc.cpp
	sys/drivers/ibm3x0.cpp
	sys/drivers/ibm700.cpp
	sys/drivers/pdp11.cpp
	sys/pdp10/pdp10.cpp
	sys/vax/ka640.cpp
	sys/vax/ka650.cpp
	sys/vax/ka780.cpp
	sys/vax/ka820.cpp
	sys/vax/vax.cpp
)

set(mse_h_SRCS
	dev/comm/dec/cty.h
	dev/cpu/alpha/axp.h
	dev/cpu/alpha/ev4.h
	dev/cpu/ibm3x0/ibm3x0.h
	dev/cpu/ibm700/ibm700.h
	dev/cpu/pdp10/executes.h
	dev/cpu/pdp10/ks10.h
	dev/cpu/pdp10/opcodes.h
	dev/cpu/pdp10/pdp10.h
	dev/cpu/pdp11/models.h
	dev/cpu/pdp11/pdp11.h
	dev/cpu/vax/cvax.h
	dev/cpu/vax/fpu.h
	dev/cpu/vax/ka780.h
	dev/cpu/vax/mmu.h
	dev/cpu/vax/mtpr.h
	dev/cpu/vax/nvax.h
	dev/cpu/vax/opcodes.h
	dev/cpu/vax/v11.h
	dev/cpu/vax/vax.h
	emu/commands.h
	emu/console.h
	emu/core.h
	emu/debug.h
	emu/devcore.h
	emu/devcpu.h
	emu/devsys.h
	emu/osd/ether.h
	emu/osd/socket.h
	emu/osd/telnet.h
	emu/user.h
	formats/dec/word10.h
	sys/pdp10/pdp10.h
	sys/vax/ka650.h
	sys/vax/vax.h
)

set(tools_SRCS
	tools/dump.cpp
	tools/dump10.cpp
)

include_directories(${PROJECT_SOURCE_DIR}/src)

add_executable(mse ${mse_cpp_SRCS} ${mse_h_SRCS})
target_compile_definitions(mse PRIVATE ENABLE_DEBUG)
if(WIN32)
	target_link_libraries(mse ${WSOCK32_LIBRARY} ${WS2_32_LIBRARY})
endif()