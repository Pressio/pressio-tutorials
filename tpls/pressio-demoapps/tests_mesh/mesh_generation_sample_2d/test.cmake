include(FindUnixCommands)

# generate full mesh
if (${PER} STREQUAL none)
  set(CMD "python3 ${FMESHDRIVER} -n ${nx} ${ny} --outDir ${OUTDIR}/full -s ${ss} --bounds 0.0 1.0 0.0 1.0")
endif()
if (${PER} STREQUAL onlyx)
  set(CMD "python3 ${FMESHDRIVER} -n ${nx} ${ny} --outDir ${OUTDIR}/full -s ${ss} --bounds 0.0 1.0 0.0 1.0 --periodic x")
endif()
if (${PER} STREQUAL onlyy)
  set(CMD "python3 ${FMESHDRIVER} -n ${nx} ${ny} --outDir ${OUTDIR}/full -s ${ss} --bounds 0.0 1.0 0.0 1.0 --periodic y")
endif()
if (${PER} STREQUAL all)
  set(CMD "python3 ${FMESHDRIVER} -n ${nx} ${ny} --outDir ${OUTDIR}/full -s ${ss} --bounds 0.0 1.0 0.0 1.0 --periodic x y")
endif()

execute_process(COMMAND ${BASH} -c ${CMD} RESULT_VARIABLE RES)
if(RES)
  message(FATAL_ERROR "Mesh generation failed")
else()
  message("Mesh generation succeeded!")
endif()

# generate sample mesh
set(CMD "python3 ${SMESHDRIVER} --outDir ${OUTDIR} --fullMeshDir ${OUTDIR}/full")
execute_process(COMMAND ${BASH} -c ${CMD} RESULT_VARIABLE RES)
if(RES)
  message(FATAL_ERROR "Mesh generation failed")
else()
  message("Mesh generation succeeded!")
endif()

# use python script to make comparison
set(CMD "python3 compare_mesh_files.py 1e-10")
execute_process(COMMAND ${BASH} -c ${CMD} RESULT_VARIABLE RES)
if(RES)
  message(FATAL_ERROR "Diff failed")
else()
  message("Mesh files diff succeeded!")
endif()