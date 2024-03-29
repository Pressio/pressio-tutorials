include(FindUnixCommands)

set(CMD "python3 ${MESHDRIVER} -n 24 32 --outDir ${OUTDIR} -s ${STENCILVAL} --bounds -1.0 1.0 -1.0 1.0 --periodic x y")
execute_process(COMMAND ${BASH} -c ${CMD} RESULT_VARIABLE RES)
if(RES)
  message(FATAL_ERROR "Mesh generation failed")
else()
  message("Mesh generation succeeded!")
endif()

execute_process(COMMAND ${EXENAME} RESULT_VARIABLE CMD_RESULT)
if(RES)
  message(FATAL_ERROR "run failed")
else()
  message("run succeeded!")
endif()
