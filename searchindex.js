Search.setIndex({"docnames": ["advanced/comingsoon", "build", "endtoend/readthisfirst", "endtoend/swe_galerkin_default", "endtoend/swe_galerkin_hypred_1", "endtoend/swe_lspg_default", "endtoend/swe_lspg_hypred_1", "index", "license", "mydefs", "using_eigen/nonlinsolvers1", "using_eigen/ode1", "using_eigen/ode2", "using_eigen/ode3"], "filenames": ["advanced/comingsoon.rst", "build.rst", "endtoend/readthisfirst.rst", "endtoend/swe_galerkin_default.rst", "endtoend/swe_galerkin_hypred_1.rst", "endtoend/swe_lspg_default.rst", "endtoend/swe_lspg_hypred_1.rst", "index.rst", "license.rst", "mydefs.rst", "using_eigen/nonlinsolvers1.rst", "using_eigen/ode1.rst", "using_eigen/ode2.rst", "using_eigen/ode3.rst"], "titles": ["Coming soon", "Build instructions", "Start reading here", "2D SWE: default Galerkin", "2D SWE: hyper-reduced Galerkin (example 1)", "2D SWE: default LSPG", "2D SWE: hyper-reduced LSPG (example 1)", "Pressio Tutorials", "License", "&lt;no title&gt;", "nonlinear solvers: tutorial 1", "ode: tutorial 1", "ode: tutorial 2", "ode: tutorial 3"], "terms": {"These": 1, "tutori": [1, 2, 3, 4, 5, 6], "should": [1, 2, 3, 4, 5, 6], "easi": [1, 2], "quick": 1, "follow": [1, 3, 4, 5, 6, 8], "below": [1, 3, 4, 5, 6, 7], "cmake": [1, 3, 4, 5, 6], "3": [1, 7, 8, 10, 11, 12], "18": 1, "0": [1, 3, 4, 5, 6, 10, 11, 12, 13], "c": [1, 3, 4, 5, 6, 7], "14": [1, 10], "compil": 1, "our": [1, 7], "ci": 1, "current": 1, "test": [1, 3, 4, 5, 6, 7], "both": 1, "gcc": 1, "clang": 1, "python": [1, 2], "x": [1, 10, 12, 13], "specif": [1, 2, 7, 8], "packag": 1, "list": [1, 8], "here": [1, 3, 4, 5, 6, 7, 13], "git": 1, "clone": 1, "recurs": 1, "github": 1, "com": 1, "pressio": [1, 2, 3, 4, 5, 6, 8, 10, 11, 12, 13], "export": [1, 3, 4, 5, 6], "cxx": 1, "fullpath": 1, "your": [1, 4, 6], "builddir": [1, 3, 4, 5, 6, 12, 13], "home": 1, "tutorialbuild": 1, "mkdir": 1, "dcmake_build_typ": 1, "releas": 1, "s": [1, 8, 10, 11, 12, 13], "path": [1, 3, 4, 5, 6], "repo": [1, 3, 4, 5, 6], "b": [1, 6], "cd": [1, 3, 4, 5, 6], "make": 1, "j4": 1, "can": [1, 2, 4, 6, 7], "instal": 1, "pip3": 1, "py_requir": 1, "txt": [1, 3, 4, 5, 6], "individu": 1, "execut": [1, 2, 4], "end": [1, 2, 3, 4, 5, 6, 7, 13], "demo": [1, 2, 3, 4, 5, 6, 7], "found": 1, "insid": 1, "directori": [1, 3, 4, 5, 6], "If": [1, 4, 6, 7], "want": [1, 4, 6], "verifi": 1, "enabl": 1, "via": [1, 2, 7, 10], "dpressiotutorials_enable_test": 1, "ON": [1, 8], "do": [1, 3, 4, 5, 6, 7], "ctest": 1, "The": [1, 3, 4, 5, 6, 7], "have": [1, 2, 3, 4, 5, 6], "librari": [1, 7], "contain": [1, 7], "code": [1, 2, 3, 4, 5, 6, 8], "thi": [1, 3, 4, 5, 6, 8, 10, 12, 13], "conver": 1, "demoapp": [1, 2, 4, 6], "provid": [1, 8], "suit": [1, 2, 7], "physic": 1, "problem": [1, 2, 3, 4, 5, 6, 10, 11, 12, 13], "ar": [1, 2, 4, 6, 7, 8], "us": [1, 2, 3, 4, 5, 6, 7, 8, 10, 11, 12, 13], "demonstr": [1, 7, 13], "function": 1, "eigen": [1, 3, 4, 5, 6, 10, 11, 12, 13], "implement": [1, 2], "all": [1, 2, 3, 4, 5, 6], "rom": [1, 2, 7], "workflow": [1, 2, 7], "driver": [1, 2, 3, 4, 5], "section": [2, 3, 7], "offer": [2, 7], "cover": 2, "some": [2, 7], "method": 2, "describ": [2, 7], "how": [2, 3, 7, 10, 12, 13], "cannot": [2, 7], "simpli": [2, 7], "boil": 2, "down": 2, "present": [2, 7], "api": [2, 7], "becaus": 2, "set": [2, 3, 4, 5, 6, 13], "up": 2, "run": [2, 3, 5, 6, 11], "process": 2, "involv": [2, 7], "sever": [2, 7], "step": [2, 7, 11, 12, 13], "we": [2, 3, 4, 5, 6, 7, 13], "believ": 2, "best": [2, 7], "wai": [2, 7, 8], "understand": [2, 7], "also": [2, 4, 6], "what": [2, 7], "context": 2, "where": [2, 3, 4, 5, 6], "logic": 2, "sequenc": 2, "explain": [2, 7], "To": [2, 3, 4, 5, 6], "develop": 2, "base": [2, 7], "framework": 2, "one": [2, 10, 13], "express": [2, 8], "help": 2, "autom": 2, "extent": 2, "each": [2, 7], "fulli": 2, "document": [2, 8], "manner": 2, "see": [2, 3, 4, 5, 6], "exampl": [2, 3, 7], "show": [2, 4, 6, 7, 10, 12, 13], "ha": 2, "an": [2, 3, 4, 5, 6], "associ": 2, "yaml": [2, 3, 4, 5, 6], "file": [2, 13], "organ": [2, 7], "scope": 2, "script": [2, 3, 4, 5, 6], "written": [2, 8], "design": [2, 7], "easili": [2, 3, 7], "extend": 2, "ad": [2, 7], "new": [2, 3], "account": 2, "variat": 2, "reli": 2, "which": [2, 3, 4, 6, 7], "built": [2, 3, 4, 5, 6], "support": 2, "sampl": 2, "mesh": 2, "while": 2, "adopt": 2, "2d": 2, "shallow": [2, 3, 4, 5, 6], "water": [2, 3, 4, 5, 6], "equat": [2, 3, 4, 5, 6], "setup": 2, "similar": 2, "ani": [2, 8], "other": [2, 8], "would": 2, "onli": 2, "requir": 2, "minim": 2, "work": 2, "you": [2, 4, 5, 6, 7], "abl": 2, "variou": [2, 7], "map": 2, "explor": 2, "impact": 2, "chang": 2, "paramet": 2, "space": 2, "basi": [2, 3, 5], "size": [2, 6, 12, 13], "just": [2, 4, 6], "rerun": 2, "tailor": 2, "share": 2, "memori": 2, "mode": [2, 3, 4, 5, 6], "recycl": 2, "adapt": 2, "them": [2, 7], "anoth": 2, "NOT": [2, 8], "doe": 2, "applic": 2, "techniqu": [3, 4, 5, 6], "A": [3, 4, 5, 6, 8], "valid": [3, 4, 5, 6], "build": [3, 4, 5, 6], "env": [3, 4, 5, 6], "variabl": [3, 4, 5, 6], "reposrc": [3, 4, 5, 6], "full": [3, 4, 5, 6], "sourc": [3, 4, 5, 6, 8], "must": [3, 4, 5, 6, 8], "correct": [3, 4, 5, 6], "2d_swe_galerkin_default": 3, "shown": [3, 4, 5, 6], "exposit": [3, 4, 5, 6], "purpos": [3, 4, 5, 6, 8, 13], "automat": [3, 4, 5, 6], "copi": [3, 4, 5, 6], "so": [3, 4, 5, 6], "don": [3, 4, 5, 6, 7], "t": [3, 4, 5, 6, 7], "need": [3, 4, 5, 6], "anyth": [3, 4, 5, 6], "2d_swe": [3, 4, 5, 6], "parameterspac": [3, 4, 5, 6], "name": [3, 4, 5, 6, 8], "corioli": [3, 4, 5, 6], "pulsemag": [3, 4, 5, 6], "graviti": [3, 4, 5, 6], "trainpoint": [3, 4, 5, 6], "125": [3, 4, 5, 6], "9": [3, 4, 5, 6], "8": [3, 4, 5, 6, 12, 13], "testpoint": [3, 4, 5, 6], "meshsiz": [3, 4, 5, 6], "65": [3, 4, 5, 6], "inviscidfluxreconstruct": [3, 4, 5, 6], "weno3": [3, 4, 5, 6], "odeschem": [3, 4, 5, 6], "rk4": [3, 4], "timesteps": [3, 4, 5, 6], "005": [3, 4], "train": [3, 4, 5, 6], "finaltim": [3, 4, 5, 6, 11, 12, 13], "statesamplingfreq": [3, 4, 5, 6], "10": [3, 4, 5, 6, 10, 11, 12, 13], "rhssamplingfreq": [3, 4], "100": [3, 4, 6], "offlinerom": [3, 4, 5, 6], "pod": [3, 4, 5, 6], "statedata": [3, 4, 5, 6], "usetrainingrun": [3, 4, 5, 6], "rhsdata": [3, 4], "onlinerom": [3, 4, 5, 6], "algorithm": [3, 4, 5, 6], "defaultgalerkin": 3, "podtrunc": [3, 4, 5, 6], "energybas": [3, 4, 5, 6], "99": [3, 4, 5, 6], "999": [3, 4, 5, 6], "99999": [3, 4, 5, 6], "from": [3, 4, 5, 6, 7, 8, 12, 13], "within": [3, 4, 5, 6, 12, 13], "python3": [3, 4, 5, 6, 12, 13], "wf_fom": [3, 4, 5, 6], "py": [3, 4, 5, 6, 12, 13], "wf": [3, 4, 5, 6], "when": [3, 5], "being": [3, 4, 5, 6], "templat": [3, 4, 5, 6, 11, 12, 13], "class": [3, 4, 5, 6, 11, 12, 13], "fomsystemtyp": [3, 4, 5, 6], "parsertyp": [3, 4, 5, 6], "void": [3, 4, 5, 6, 10, 11, 12, 13], "run_fom_explicit": 3, "const": [3, 4, 5, 6, 10, 11, 12, 13], "fomsystem": [3, 4, 5, 6], "parser": [3, 4, 5, 6], "namespac": [3, 4, 5, 6, 10, 11, 12, 13], "pode": [3, 4, 5, 6, 11, 12, 13], "od": [3, 4, 5, 6, 7], "auto": [3, 4, 5, 6, 10, 11, 12, 13], "state": [3, 5, 11, 12, 13], "initialcondit": [3, 5], "write_vector_to_binari": [3, 5], "initial_st": [3, 5], "bin": [3, 4, 5, 6, 12, 13], "assert": [3, 5, 6], "is_explicit_schem": [3, 5], "stepperobj": [3, 5, 11, 12], "create_explicit_stepp": [3, 11, 12], "rhsobserv": 3, "rhsob": 3, "stateobserv": [3, 4, 5, 6, 12], "stateob": 3, "starttim": [3, 4, 5, 6, 11, 12, 13], "static_cast": [3, 5, 6, 12, 13], "typenam": [3, 4, 5, 6, 12, 13], "scalar_typ": [3, 4, 5, 6, 10, 11, 12, 13], "advance_n_step": [3, 4, 5, 6, 11], "stepcount": [3, 4, 5, 6, 11, 12, 13], "numstep": [3, 4, 5, 6], "At": [3, 4, 5, 6], "tree": [3, 5], "l": [3, 5], "produc": [3, 5], "cmakefil": [3, 4, 5, 6], "makefil": [3, 4, 5, 6], "cmake_instal": [3, 4, 5, 6], "fom_mesh": [3, 4, 5, 6], "fom_test_runid_0": [3, 4, 5, 6], "fom_train_runid_0": [3, 4, 5, 6], "fom_train_runid_1": [3, 4, 5, 6], "plot": [3, 4, 5, 6, 12, 13], "wf_offline_rom": [3, 4, 5, 6], "take": [3, 4, 5, 6], "care": [3, 4, 5, 6], "data": [3, 4, 5, 6, 8], "comput": [3, 4, 5, 6, 7, 10, 11], "creat": [3, 5, 13], "offline_rom": [3, 4, 5, 6], "subdirectori": [3, 4, 5, 6, 7], "pod_input": [3, 4, 5, 6], "rhs_left_singular_vector": [3, 4], "rhs_singular_valu": [3, 4], "rhs_snapshot": [3, 4], "state_left_singular_vector": [3, 4, 5, 6], "state_singular_valu": [3, 4, 5, 6], "state_snapshot": [3, 4, 5, 6, 12, 13], "wf_galerkin": [3, 4], "run_galerkin_default": 3, "prom": [3, 4, 5, 6], "pgal": [3, 4], "reduced_state_typ": [3, 4, 5, 6], "matrix": [3, 4, 5, 6, 11, 12, 13], "dynam": [3, 4, 5, 6], "create_basis_and_read_from_fil": [3, 4, 5, 6], "romfullmeshpodbasisfil": [3, 4, 5, 6], "rommodecount": [3, 4, 5, 6], "affineshift": [3, 4, 5, 6], "create_affine_shift_and_read_from_fil": [3, 5], "romaffineshiftfil": [3, 5], "trialspac": [3, 4, 5, 6], "create_trial_column_subspac": [3, 4, 5, 6], "std": [3, 4, 5, 6, 10, 11, 12, 13], "move": [3, 4, 5, 6], "romisaffin": [3, 4, 5, 6], "reducedst": [3, 4, 5, 6], "createreducedst": [3, 4, 5, 6], "fill_rom_state_from_ascii": [3, 4, 5, 6], "rominitialstatefil": [3, 4, 5, 6], "observ": [3, 4, 5, 6, 13], "create_unsteady_explicit_problem": [3, 4], "time_typ": [3, 4], "structur": [3, 4, 5, 6, 7], "default_galerkin_truncation_energybased_99": 3, "99999_runid_0": [3, 5, 6], "999_runid_0": [3, 5, 6], "wf_reconstruct_on_full_mesh": [3, 4, 5, 6], "suggest": 3, "few": [3, 5, 6], "experi": [3, 7], "note": [3, 4, 6], "befor": 3, "same": [3, 4, 6], "avoid": 3, "conflict": 3, "cleanup": 3, "exist": 3, "content": 3, "clean": 3, "differ": [3, 4, 6], "simul": 3, "time": [3, 5, 7, 11, 12, 13], "assess": [3, 4, 6], "perform": 3, "extrapol": 3, "regim": 3, "6": [3, 4, 5, 6, 10, 11, 13], "edit": 3, "add": 3, "point": 3, "outsid": 3, "rang": [3, 7], "gappi": 4, "2d_swe_galerkin_hypred_1": 4, "5": [4, 5, 6, 10, 12, 13], "samplemesh": [4, 6], "random": [4, 6], "hyperreducedgalerkin": 4, "stage": [4, 6], "put": [4, 6], "everyth": [4, 6], "sample_mesh_random_0": [4, 6], "In": [4, 6], "gener": [4, 6], "keep": [4, 6], "randomli": [4, 6], "cell": [4, 6], "whole": [4, 6, 7], "done": [4, 6], "presssio": [4, 6], "visual": [4, 6], "given": [4, 6], "its": [4, 6, 8], "natur": [4, 6], "might": [4, 5, 6, 7], "look": [4, 6, 7], "slightli": [4, 6], "tpl": [4, 6], "meshing_script": [4, 6], "plot_mesh": [4, 6], "wdir": [4, 6], "p": [4, 6], "like": [4, 6], "yellow": [4, 6], "denot": [4, 6], "white": [4, 6], "stencil": [4, 6], "mean": [4, 7], "scalartyp": [4, 6, 11, 12, 13], "fomrhstyp": 4, "explicitgalerkinhyperreduc": 4, "static_assert": 4, "is_vector_eigen": 4, "valu": 4, "vector": [4, 6, 13], "matrix_typ": 4, "colmajor": 4, "p_": 4, "public": [4, 11, 12, 13], "right_hand_side_operand_typ": 4, "string": 4, "projfil": 4, "int": [4, 6, 10, 11, 12, 13], "nummod": 4, "create_colmajor_eigen_matrix_and_load_from_binary_with_ext": 4, "resulttyp": 4, "enable_if_t": 4, "oper": [4, 11, 12, 13], "operand": 4, "unus": [4, 11, 12, 13], "transpos": 4, "run_galerkin_hyperreduc": 4, "modecount": [4, 6], "basisfil": [4, 6], "basisful": [4, 6], "basisonstencil": [4, 6], "create_basis_on_stencil_mesh": [4, 6], "create_affine_shift_on_stencil_mesh": [4, 6], "fom_rhs_typ": 4, "right_hand_side_typ": [4, 11, 12, 13], "hr_op_t": 4, "h": 4, "romgalerkinhypredoperatorfil": 4, "hyperreduced_galerkin_truncation_energybased_99": 4, "99999_sample_mesh_random_0": [4, 6], "100_runid_0": [4, 6], "999_sample_mesh_random_0": [4, 6], "hyperreducer_99": 4, "2d_swe_default_galerkin": 4, "abov": [4, 6, 8], "gain": [4, 6], "thank": [4, 6], "fraction": [4, 6], "check": [4, 6, 10, 11], "tail": [4, 6], "n": [4, 6, 10, 11, 12, 13], "fom_": [4, 6], "out": [4, 6, 8, 12, 13], "log": [4, 6, 10, 11, 12, 13], "someth": [4, 6, 7], "elaps": [4, 6], "71033": 4, "info": [4, 6, 10], "1806179": 4, "final": [4, 6, 10, 11, 12, 13], "logger": [4, 6], "86063": 4, "1805967": 4, "75992": 4, "1806103": 4, "Then": [4, 6], "hyperreduced_": [4, 6], "854369": 4, "1812457": 4, "01332": 4, "1812527": 4, "2d_swe_lspg_default": 5, "bdf1": [5, 6], "01": [5, 6, 12, 13], "nonlinearsolvernam": [5, 6], "newtonraphson": [5, 6], "nonlinearsolvertoler": [5, 6], "1e": [5, 6, 10, 13], "defaultlspg": 5, "gaussnewton": [5, 6], "min": [5, 6], "patient": [5, 6], "run_fom_implicit": 5, "plin": [5, 13], "linearsolv": [5, 6, 10, 13], "pnlin": [5, 6, 13], "nonlinearsolv": [5, 6, 10, 13], "stepper": [5, 13], "create_implicit_stepp": [5, 13], "linear": [5, 7, 13], "solver": [5, 6, 7, 13], "fom_jacobian_t": [5, 13], "jacobian_typ": [5, 10, 13], "linear_solver_t": [5, 6, 10, 13], "iter": [5, 13], "bicgstab": [5, 13], "nonlinear": [5, 7, 13], "nonlinsolvertyp": 5, "create_newton_raphson": [5, 13], "settoler": [5, 6, 10, 13], "advanc": [5, 7], "wf_lspg": [5, 6], "run_lspg_default": 5, "plspg": [5, 6], "create_unsteady_problem": [5, 6], "lspg_pick_solver_and_run": [5, 6], "lspgstepper": [5, 6], "lspgsteppertyp": [5, 6], "romstatetyp": [5, 6], "trait": [5, 6], "independent_variable_typ": [5, 6, 11, 12, 13], "hessian_t": [5, 6, 10], "solver_tag": [5, 6], "direct": [5, 6, 8, 10], "householderqr": [5, 6, 10], "create_gauss_newton": [5, 6, 10], "setstoppingcriterion": [5, 6], "stop": [5, 6], "whengradientabsolutenormbelowtoler": [5, 6], "default_lspg_truncation_energybased_99": 5, "2d_swe_lspg_hypred_1": 6, "hyperreducedlspg": 6, "inttyp": 6, "find_index": 6, "target": [6, 11], "j": [6, 10, 13], "return": [6, 10, 11, 12, 13], "numeric_limit": 6, "max": 6, "struct": [6, 10], "hypredupdat": 6, "vec_operand_typ": 6, "mat_ll_operand_typ": 6, "indices_": 6, "numdofspercell_": 6, "explicit": [6, 12], "numdofspercel": 6, "stfile": 6, "romstencilmeshgidsfil": 6, "safil": 6, "romsamplemeshgidsfil": 6, "stencilmeshgid": 6, "create_cell_gids_vector_and_fill_from_ascii": 6, "samplemeshgid": 6, "resiz": 6, "cout": [6, 10, 11, 12, 13], "updat": 6, "size_t": [6, 12, 13], "i": [6, 13], "index": 6, "updatesamplemeshoperandwithstencilmeshon": 6, "alpha": 6, "beta": 6, "r": [6, 10], "g": [6, 7], "k": 6, "col": 6, "run_lspg_hyperreduc": 6, "hrupdat": 6, "hyperreduced_lspg_truncation_energybased_99": 6, "102": 6, "308": 6, "1136219": 6, "1657": 6, "1133529": 6, "8976": 6, "1134515": 6, "53887": 6, "1138950": 6, "52652": 6, "1139027": 6, "wa": 7, "start": 7, "focu": 7, "project": 7, "reduc": 7, "order": 7, "model": 7, "strongli": 7, "multidisciplinari": 7, "topic": 7, "product": [7, 8], "level": [7, 10], "capabl": 7, "inevit": 7, "touch": 7, "multipl": 7, "field": 7, "e": 7, "algebra": 7, "optim": 7, "distribut": [7, 8], "hpc": 7, "integr": [7, 12, 13], "clearli": 7, "reflect": 7, "displai": 7, "stack": 7, "compon": 7, "handl": 7, "depend": 7, "ones": 7, "directli": 7, "constitut": 7, "foundat": 7, "top": 7, "therefor": 7, "meaning": 7, "well": 7, "unveil": 7, "detail": 7, "obviou": 7, "complet": 7, "practic": 7, "allow": 7, "collect": 7, "short": 7, "independ": 7, "self": 7, "fashion": 7, "activ": 7, "more": 7, "find": 7, "temporarili": 7, "readabl": 7, "obvious": 7, "format": 7, "bsd": [7, 8], "avail": 7, "publish": 7, "arxiv": 7, "preprint": 7, "http": 7, "org": 7, "ab": 7, "2003": 7, "07798": 7, "slack": 7, "header": 8, "copyright": 8, "2019": 8, "nation": 8, "technolog": 8, "engin": 8, "solut": [8, 10, 11], "sandia": 8, "llc": 8, "ntess": 8, "under": 8, "term": 8, "contract": 8, "de": 8, "na0003525": 8, "u": 8, "govern": 8, "retain": 8, "certain": 8, "right": 8, "softwar": 8, "claus": 8, "redistribut": 8, "binari": [8, 12, 13], "form": 8, "without": 8, "modif": 8, "permit": 8, "condit": 8, "met": 8, "1": [8, 12, 13], "notic": 8, "disclaim": 8, "2": [8, 10, 11, 13], "reproduc": 8, "materi": 8, "neither": 8, "holder": 8, "nor": 8, "contributor": 8, "mai": 8, "endors": 8, "promot": 8, "deriv": 8, "prior": 8, "permiss": 8, "IS": 8, "BY": 8, "THE": 8, "AND": 8, "AS": 8, "OR": 8, "impli": 8, "warranti": 8, "includ": [8, 10, 11, 12, 13], "BUT": 8, "limit": 8, "TO": 8, "OF": 8, "merchant": 8, "fit": 8, "FOR": 8, "particular": 8, "IN": 8, "NO": 8, "event": 8, "shall": 8, "BE": 8, "liabl": 8, "indirect": 8, "incident": 8, "special": 8, "exemplari": 8, "consequenti": 8, "damag": 8, "procur": 8, "substitut": 8, "good": 8, "servic": 8, "loss": 8, "profit": 8, "busi": 8, "interrupt": 8, "howev": 8, "caus": 8, "theori": 8, "liabil": 8, "whether": 8, "strict": 8, "tort": 8, "neglig": 8, "otherwis": 8, "aris": 8, "even": 8, "IF": 8, "advis": 8, "possibl": 8, "SUCH": 8, "question": 8, "contact": 8, "francesco": 8, "rizzi": 8, "fnrizzi": 8, "gov": 8, "solv": 10, "gauss": 10, "newton": 10, "variant": 10, "rosenbrock": 10, "solvers_linear": 10, "hpp": [10, 11, 12, 13], "solvers_nonlinear": 10, "core": [10, 11, 12, 13], "myrosenbrocksystem": 10, "doubl": [10, 11, 12, 13], "state_typ": [10, 11, 12, 13], "vectorxd": 10, "residual_typ": 10, "matrixxd": 10, "createst": [10, 11, 12, 13], "4": [10, 11, 13], "setzero": 10, "createresidu": 10, "createjacobian": [10, 13], "residu": 10, "re": 10, "x1": 10, "x2": 10, "x3": 10, "x4": 10, "jacobian": 10, "jj": 10, "20": 10, "first": [10, 13], "row": [10, 13], "second": [10, 13], "etc": 10, "main": [10, 11, 12, 13], "plog": 10, "pl": 10, "pnonl": 10, "initi": [10, 11, 12, 13], "logto": [10, 11, 12, 13], "termin": [10, 11, 12, 13], "setverbos": 10, "linear_solver_tag": 10, "gnsolver": 10, "05": 10, "setprecis": 10, "expect": [10, 11], "0000000156741": 10, "99999999912477": 10, "99999999651993": 10, "99999998889888": 10, "exact": 10, "minimum": 10, "endl": [10, 11], "ode_advanc": [11, 12, 13], "ode_steppers_explicit": [11, 12], "simplesystem": 11, "setconst": [11, 12, 13], "createrighthandsid": [11, 12, 13], "v": [11, 12, 13], "timein": [11, 12, 13], "rh": [11, 12, 13], "constexpr": [11, 12, 13], "ten": 11, "argc": [11, 12, 13], "char": [11, 12, 13], "argv": [11, 12, 13], "y": [11, 12, 13], "scheme": [11, 12, 13], "stepschem": [11, 12, 13], "forwardeul": 11, "fix": 11, "number": 11, "dt": [11, 12, 13], "until": 11, "advance_to_target_point": [11, 12, 13], "stepstartat": [11, 12, 13], "stepsiz": [11, 12, 13], "1024": 11, "2048": 11, "3074": 11, "origin": [12, 13], "lorenz": [12, 13], "system": [12, 13], "lorenz3": [12, 13], "n_": [12, 13], "rho_": [12, 13], "28": [12, 13], "sigma_": [12, 13], "beta_": [12, 13], "z": [12, 13], "samplefreq": 12, "myfile_": 12, "io": [12, 13], "samplefreq_": 12, "close": [12, 13], "timetyp": 12, "get": [12, 13], "save": [12, 13], "ext": [12, 13], "sizeof": [12, 13], "write": [12, 13], "reinterpret_cast": [12, 13], "privat": [12, 13], "ofstream": [12, 13], "rungekutta4": 12, "40": [12, 13], "statesamplefreq": 12, "type": [12, 13], "tutorial2": 12, "implicit": 13, "ode_steppers_implicit": 13, "trip_t": 13, "triplet": 13, "mutabl": 13, "tripletlist_": 13, "sinc": 13, "jacobain": 13, "fairli": 13, "den": 13, "could": 13, "dens": 13, "spars": 13, "sparsematrix": 13, "allocatetriplet": 13, "setfromtriplet": 13, "begin": 13, "bool": 13, "computejac": 13, "7": 13, "push_back": 13, "third": 13, "problem_typ": 13, "bdf2": 13, "13": 13, "myfil": 13, "lambda": 13, "tutorial3": 13}, "objects": {}, "objtypes": {}, "objnames": {}, "titleterms": {"come": 0, "soon": 0, "build": 1, "instruct": 1, "what": 1, "you": [1, 3], "need": 1, "step": [1, 3, 4, 5, 6], "Then": 1, "more": 1, "detail": 1, "depend": 1, "start": 2, "read": 2, "here": 2, "why": 2, "do": 2, "thi": [2, 7], "kei": 2, "featur": 2, "outcom": 2, "2d": [3, 4, 5, 6], "swe": [3, 4, 5, 6], "default": [3, 5], "galerkin": [3, 4], "prerequisit": [3, 4, 5, 6], "workflow": [3, 4, 5, 6], "file": [3, 4, 5, 6], "1": [3, 4, 5, 6, 10, 11], "execut": [3, 5, 6], "fom": [3, 4, 5, 6], "2": [3, 4, 5, 6, 12], "offlin": [3, 4, 5, 6], "rom": [3, 4, 5, 6], "3": [3, 4, 5, 6, 13], "4": [3, 4, 5, 6], "process": [3, 4, 5, 6, 12, 13], "result": [3, 4, 5, 6, 12, 13], "5": 3, "thing": 3, "can": 3, "try": 3, "hyper": [4, 6], "reduc": [4, 6], "exampl": [4, 6], "run": 4, "sampl": [4, 6], "mesh": [4, 6], "accuraci": [4, 6], "runtim": [4, 6], "comparison": [4, 6], "lspg": [5, 6], "pressio": 7, "tutori": [7, 10, 11, 12, 13], "scope": 7, "motiv": 7, "disclaim": 7, "work": 7, "progress": 7, "licens": [7, 8], "citat": 7, "question": 7, "nonlinear": 10, "solver": 10, "od": [11, 12, 13]}, "envversion": {"sphinx.domains.c": 2, "sphinx.domains.changeset": 1, "sphinx.domains.citation": 1, "sphinx.domains.cpp": 6, "sphinx.domains.index": 1, "sphinx.domains.javascript": 2, "sphinx.domains.math": 2, "sphinx.domains.python": 3, "sphinx.domains.rst": 2, "sphinx.domains.std": 2, "sphinx.ext.viewcode": 1, "sphinx.ext.intersphinx": 1, "sphinx": 56}})