Search.setIndex({"docnames": ["advanced/comingsoon", "build", "endtoend/readthisfirst", "endtoend/swe_galerkin_default", "endtoend/swe_galerkin_hypred_1", "endtoend/swe_lspg_default", "endtoend/swe_lspg_hypred_1", "index", "license", "mydefs", "using_eigen/nonlinsolvers1", "using_eigen/nonlinsolvers2", "using_eigen/ode1", "using_eigen/ode2", "using_eigen/ode3"], "filenames": ["advanced/comingsoon.rst", "build.rst", "endtoend/readthisfirst.rst", "endtoend/swe_galerkin_default.rst", "endtoend/swe_galerkin_hypred_1.rst", "endtoend/swe_lspg_default.rst", "endtoend/swe_lspg_hypred_1.rst", "index.rst", "license.rst", "mydefs.rst", "using_eigen/nonlinsolvers1.rst", "using_eigen/nonlinsolvers2.rst", "using_eigen/ode1.rst", "using_eigen/ode2.rst", "using_eigen/ode3.rst"], "titles": ["Coming soon", "Build instructions", "Start reading here", "2D SWE: default Galerkin", "2D SWE: hyper-reduced Galerkin (example 1)", "2D SWE: default LSPG", "2D SWE: hyper-reduced LSPG (example 1)", "Pressio Tutorials", "License", "&lt;no title&gt;", "nonlinear solvers: tutorial 1", "nonlinear solvers: tutorial 2", "ode: tutorial 1", "ode: tutorial 2", "ode: tutorial 3"], "terms": {"These": 1, "tutori": [1, 3, 4, 5, 6], "should": [1, 2, 3, 4, 5, 6], "easi": 1, "quick": 1, "follow": [1, 3, 4, 5, 6, 8], "below": [1, 7], "cmake": [1, 3, 4, 5, 6], "3": [1, 8, 10, 11, 12, 13], "18": 1, "0": [1, 3, 4, 5, 6, 10, 11, 12, 13, 14], "c": [1, 3, 7], "17": 1, "compil": 1, "gcc": 1, "clang": 1, "ar": [1, 2, 4, 6, 7, 8], "test": [1, 3, 4, 5, 6], "our": [1, 3, 7], "ci": 1, "python": 1, "x": [1, 10, 11, 13, 14], "git": 1, "clone": [1, 11], "github": 1, "com": 1, "pressio": [1, 2, 3, 4, 5, 6, 8, 10, 11, 12, 13, 14], "export": [1, 3, 4, 5, 6], "cxx": 1, "fullpath": 1, "your": [1, 4, 6, 13, 14], "builddir": [1, 3, 4, 5, 6], "home": 1, "tutorialbuild": 1, "mkdir": 1, "dpressiotutorials_enable_test": 1, "ON": [1, 8], "dcmake_build_typ": 1, "releas": 1, "s": [1, 8, 11, 12, 13, 14], "path": [1, 3, 4, 5, 6], "repo": [1, 3, 4, 5, 6], "b": [1, 10], "cd": [1, 3, 4, 5, 6], "make": 1, "j4": 1, "ensur": 1, "have": [1, 2, 3, 4, 5, 6, 10], "all": [1, 3, 4, 5, 6], "requir": [1, 2, 10], "packag": 1, "instal": 1, "pip3": 1, "py_requir": 1, "txt": [1, 3, 4, 5, 6], "ctest": 1, "individu": 1, "execut": [1, 2], "end": [1, 2, 3, 4, 5, 6, 14], "demo": [1, 2, 7], "can": [1, 2, 4, 6, 7], "found": 1, "insid": 1, "directori": 1, "The": [1, 3, 4, 5, 6, 7], "rom": [1, 2], "librari": [1, 7], "contain": 1, "code": [1, 2, 3, 8], "thi": [1, 2, 3, 4, 5, 6, 7, 8], "conver": 1, "demoapp": [1, 2, 4, 6], "provid": [1, 2, 8], "suit": [1, 7], "physic": 1, "problem": [1, 2, 3, 4, 5, 6, 10, 11, 12, 13, 14], "us": [1, 2, 3, 4, 5, 6, 8, 10, 11, 12, 13, 14], "demonstr": [1, 7, 14], "function": 1, "eigen": [1, 10, 11, 12, 13, 14], "implement": [1, 2], "workflow": [1, 2, 7], "driver": [1, 2, 3], "describ": [2, 7], "how": [2, 3, 7], "cannot": [2, 7], "simpli": [2, 3, 7], "boil": 2, "down": 2, "present": [2, 7], "api": [2, 7], "becaus": 2, "set": [2, 3, 4, 5, 6, 14], "up": 2, "run": [2, 12, 13, 14], "process": 2, "involv": [2, 7], "sever": [2, 7], "step": [2, 7, 12, 13, 14], "we": [2, 3, 4, 6, 7, 14], "believ": 2, "best": [2, 7], "wai": [2, 7, 8], "understand": [2, 7], "also": [2, 3, 4, 6], "what": [2, 3, 7], "offer": [2, 7], "context": 2, "via": [2, 7, 11, 13], "where": [2, 3, 4, 5, 6], "logic": 2, "sequenc": 2, "explain": [2, 7], "To": 2, "develop": 2, "base": [2, 3, 7], "framework": 2, "one": [2, 11, 14], "express": [2, 8], "do": [2, 3, 4, 5, 6, 7], "help": 2, "autom": [2, 3], "some": [2, 7], "extent": 2, "each": [2, 7], "document": [2, 8], "manner": 2, "see": [2, 3, 4, 5, 6], "exampl": [2, 7], "defin": 2, "yaml": [2, 3, 4, 5, 6], "file": [2, 14], "which": [2, 3, 4, 6, 7], "script": 2, "easili": [2, 3, 7], "extend": 2, "ad": 2, "new": [2, 3], "method": 2, "account": 2, "variat": 2, "reli": 2, "built": [2, 3, 4, 5, 6], "support": 2, "sampl": 2, "mesh": 2, "while": 2, "adopt": 2, "specif": [2, 3, 7, 8], "2d": 2, "shallow": [2, 3, 4, 5, 6], "water": [2, 3, 4, 5, 6], "equat": [2, 3, 4, 5, 6], "setup": 2, "similar": 2, "ani": [2, 8], "other": [2, 8], "would": 2, "onli": 2, "minim": 2, "work": [2, 7], "you": [2, 7], "abl": 2, "variou": [2, 7], "map": 2, "explor": 2, "impact": 2, "chang": 2, "paramet": 2, "space": 2, "basi": [2, 3], "size": [2, 13, 14], "just": [2, 3, 4, 6], "rerun": 2, "tailor": 2, "share": 2, "memori": 2, "mode": [2, 3, 4, 5, 6], "recycl": 2, "adapt": 2, "them": [2, 7], "anoth": 2, "NOT": [2, 8], "doe": 2, "applic": 2, "techniqu": [3, 4, 5, 6], "A": [3, 4, 5, 6, 8, 10], "valid": [3, 4, 5, 6], "build": [3, 4, 5, 6, 13, 14], "here": [3, 4, 5, 6, 7, 14], "env": [3, 4, 5, 6], "variabl": [3, 4, 5, 6], "reposrc": [3, 4, 5, 6], "full": [3, 4, 5, 6, 7], "sourc": [3, 4, 5, 6, 8], "2d_swe_galerkin_default": 3, "shown": [3, 4, 5, 6], "exposit": [3, 4, 5, 6], "purpos": [3, 4, 5, 6, 8, 14], "automat": [3, 4, 5, 6], "copi": [3, 4, 5, 6], "so": [3, 4, 5, 6, 10], "don": [3, 4, 5, 6], "t": [3, 4, 5, 6], "need": [3, 4, 5, 6], "anyth": [3, 4, 5, 6], "2d_swe": [3, 4, 5, 6], "parameterspac": [3, 4, 5, 6], "name": [3, 4, 5, 6, 8], "corioli": [3, 4, 5, 6], "pulsemag": [3, 4, 5, 6], "graviti": [3, 4, 5, 6], "trainpoint": [3, 4, 5, 6], "125": [3, 4, 5, 6], "9": [3, 4, 5, 6], "8": [3, 4, 5, 6, 13, 14], "testpoint": [3, 4, 5, 6], "meshsiz": [3, 4, 5, 6], "65": [3, 4, 5, 6], "inviscidfluxreconstruct": [3, 4, 5, 6], "weno3": [3, 4, 5, 6], "odeschem": [3, 4, 5, 6], "rk4": [3, 4], "timesteps": [3, 4, 5, 6], "005": [3, 4], "train": [3, 4, 5, 6], "finaltim": [3, 4, 5, 6, 12, 13, 14], "statesamplingfreq": [3, 4, 5, 6], "10": [3, 4, 5, 6, 11, 12, 13, 14], "rhssamplingfreq": [3, 4], "100": [3, 4, 6], "offlinerom": [3, 4, 5, 6], "pod": [3, 4, 5, 6], "statedata": [3, 4, 5, 6], "usetrainingrun": [3, 4, 5, 6], "rhsdata": [3, 4], "onlinerom": [3, 4, 5, 6], "algorithm": [3, 4, 5, 6], "defaultgalerkin": 3, "podtrunc": [3, 4, 5, 6], "energybas": [3, 4, 5, 6], "99": [3, 4, 5, 6], "999": [3, 4, 5, 6], "99999": [3, 4, 5, 6], "from": [3, 4, 5, 6, 7, 8], "within": [3, 4, 5, 6], "python3": [3, 4, 5, 6, 13, 14], "wf_fom": [3, 4, 5, 6], "py": [3, 4, 5, 6, 13, 14], "wf": [3, 4, 5, 6], "first": [3, 11, 14], "stage": [3, 4, 6], "creat": [3, 5, 14], "input": 3, "gener": [3, 4, 6, 7], "data": [3, 4, 5, 6, 8], "point": 3, "specifi": 3, "At": [3, 4, 5, 6], "tree": [3, 5], "l": [3, 5], "produc": [3, 5], "cmakefil": [3, 4, 5, 6], "makefil": [3, 4, 5, 6], "cmake_instal": [3, 4, 5, 6], "fom_mesh": [3, 4, 5, 6], "fom_test_runid_0": [3, 4, 5, 6], "fom_train_runid_0": [3, 4, 5, 6], "fom_train_runid_1": [3, 4, 5, 6], "plot": [3, 4, 5, 6, 13, 14], "correspond": 3, "id": 3, "wf_offline_rom": [3, 4, 5, 6], "take": [3, 4, 5, 6], "care": [3, 4, 5, 6], "comput": [3, 4, 5, 6, 7, 10, 11, 12], "an": [3, 4, 5, 6], "offline_rom": [3, 4, 5, 6], "subdirectori": [3, 4, 5, 6], "case": 3, "pod_input": [3, 4, 5, 6], "rhs_left_singular_vector": [3, 4], "bin": [3, 4, 5, 6, 13, 14], "rhs_singular_valu": [3, 4], "rhs_snapshot": [3, 4], "state_left_singular_vector": [3, 4, 5, 6], "state_singular_valu": [3, 4, 5, 6], "state_snapshot": [3, 4, 5, 6, 13, 14], "wf_galerkin": [3, 4], "write": [3, 13, 14], "prepar": 3, "initi": [3, 10, 11, 12, 13, 14], "condit": [3, 8], "actual": 3, "everi": 3, "more": 3, "happen": 3, "total": 3, "number": [3, 12], "vari": 3, "truncat": 3, "energi": 3, "valu": [3, 10, 11, 14], "top": [3, 7], "higher": 3, "target": [3, 12], "singular": 3, "again": 3, "done": [3, 4, 6], "reduc": [3, 7], "read": 3, "project": [3, 7], "onto": 3, "note": [3, 4, 6], "monitor": 3, "evolut": 3, "state": [3, 12, 13, 14], "store": 3, "its": [3, 4, 6, 8], "histori": 3, "postprocess": 3, "structur": [3, 4, 5, 6, 7], "default_galerkin_truncation_energybased_99": 3, "99999_runid_0": [3, 5, 6], "999_runid_0": [3, 5, 6], "wf_reconstruct_on_full_mesh": [3, 4, 5, 6], "section": [3, 7], "suggest": 3, "few": [3, 5, 6], "experi": [3, 7], "befor": 3, "same": [3, 4, 6], "avoid": 3, "conflict": 3, "cleanup": 3, "exist": 3, "content": 3, "clean": 3, "differ": [3, 4, 6], "simul": 3, "time": [3, 7, 12, 13, 14], "assess": [3, 4, 6], "perform": 3, "extrapol": 3, "regim": 3, "replac": 3, "6": [3, 4, 5, 6, 11, 12, 14], "add": 3, "outsid": 3, "rang": [3, 7], "gappi": 4, "2d_swe_galerkin_hypred_1": 4, "5": [4, 5, 6, 11, 13, 14], "samplemesh": [4, 6], "random": [4, 6], "hyperreducedgalerkin": 4, "put": [4, 6], "everyth": [4, 6], "sample_mesh_random_0": [4, 6], "In": [4, 6], "keep": [4, 6], "randomli": [4, 6], "cell": [4, 6], "whole": [4, 6, 7], "presssio": [4, 6], "visual": [4, 6], "given": [4, 6], "natur": [4, 6], "might": [4, 5, 6, 7], "look": [4, 6], "slightli": [4, 6], "tpl": [4, 6], "meshing_script": [4, 6], "plot_mesh": [4, 6], "wdir": [4, 6], "p": [4, 6], "show": [4, 6, 7], "like": [4, 6], "yellow": [4, 6], "denot": [4, 6], "white": [4, 6], "stencil": [4, 6], "hyperreduced_galerkin_truncation_energybased_99": 4, "99999_sample_mesh_random_0": [4, 6], "100_runid_0": [4, 6], "999_sample_mesh_random_0": [4, 6], "hyperreducer_99": 4, "2d_swe_default_galerkin": 4, "abov": [4, 6, 8], "want": [4, 6], "gain": [4, 6], "thank": [4, 6], "fraction": [4, 6], "check": [4, 6, 10, 11, 12], "If": [4, 6], "tail": [4, 6], "n": [4, 6, 10, 11, 12, 14], "fom_": [4, 6], "out": [4, 6, 8, 13, 14], "log": [4, 6, 10, 11, 12, 13, 14], "someth": [4, 6, 7], "elaps": [4, 6], "71033": 4, "info": [4, 6, 10, 11], "1806179": 4, "final": [4, 6, 10, 11, 12, 13, 14], "logger": [4, 6], "86063": 4, "1805967": 4, "75992": 4, "1806103": 4, "Then": [4, 6], "hyperreduced_": [4, 6], "854369": 4, "1812457": 4, "01332": 4, "1812527": 4, "basic": 5, "2d_swe_lspg_default": 5, "bdf1": [5, 6], "01": [5, 6, 13, 14], "nonlinearsolvernam": [5, 6], "newtonraphson": [5, 6], "nonlinearsolvertoler": [5, 6], "1e": [5, 6, 11, 14], "defaultlspg": 5, "gaussnewton": [5, 6], "min": [5, 6], "patient": [5, 6], "wf_lspg": [5, 6], "default_lspg_truncation_energybased_99": 5, "2d_swe_lspg_hypred_1": 6, "hyperreducedlspg": 6, "hyperreduced_lspg_truncation_energybased_99": 6, "102": 6, "308": 6, "1136219": 6, "1657": 6, "1133529": 6, "8976": 6, "1134515": 6, "53887": 6, "1138950": 6, "52652": 6, "1139027": 6, "wa": 7, "start": 7, "focu": 7, "order": 7, "model": 7, "strongli": 7, "multidisciplinari": 7, "product": [7, 8], "level": [7, 10, 11], "capabl": 7, "inevit": 7, "mean": 7, "touch": 7, "multipl": 7, "field": 7, "e": 7, "g": 7, "linear": [7, 14], "algebra": 7, "nonlinear": [7, 14], "solver": [7, 14], "optim": 7, "integr": [7, 13, 14], "program": 7, "distribut": [7, 8], "hpc": 7, "clearli": 7, "reflect": 7, "compon": 7, "handl": 7, "depend": 7, "ones": 7, "allow": 7, "user": 7, "directli": 7, "stack": 7, "constitut": 7, "foundat": 7, "therefor": 7, "design": 7, "meaning": 7, "well": 7, "unveil": 7, "detail": 7, "obviou": 7, "organ": 7, "complet": 7, "practic": 7, "collect": 7, "short": 7, "independ": 7, "fashion": 7, "od": 7, "tbd": 7, "progress": 7, "bsd": [7, 8], "licens": 7, "avail": 7, "publish": 7, "find": 7, "arxiv": 7, "preprint": 7, "http": 7, "org": 7, "ab": 7, "2003": 7, "07798": 7, "slack": 7, "header": 8, "copyright": 8, "2019": 8, "nation": 8, "technolog": 8, "engin": 8, "solut": [8, 10, 11, 12], "sandia": 8, "llc": 8, "ntess": 8, "under": 8, "term": 8, "contract": 8, "de": 8, "na0003525": 8, "u": 8, "govern": 8, "retain": 8, "certain": 8, "right": 8, "softwar": 8, "claus": 8, "redistribut": 8, "binari": [8, 13, 14], "form": 8, "without": 8, "modif": 8, "permit": 8, "met": 8, "1": [8, 11, 13, 14], "must": 8, "notic": 8, "list": 8, "disclaim": 8, "2": [8, 10, 12, 14], "reproduc": 8, "materi": 8, "neither": 8, "holder": 8, "nor": 8, "contributor": 8, "mai": 8, "endors": 8, "promot": 8, "deriv": 8, "prior": 8, "written": 8, "permiss": 8, "IS": 8, "BY": 8, "THE": 8, "AND": 8, "AS": 8, "OR": 8, "impli": 8, "warranti": 8, "includ": [8, 10, 11, 12, 13, 14], "BUT": 8, "limit": 8, "TO": 8, "OF": 8, "merchant": 8, "fit": 8, "FOR": 8, "particular": 8, "IN": 8, "NO": 8, "event": 8, "shall": 8, "BE": 8, "liabl": 8, "direct": [8, 11], "indirect": 8, "incident": 8, "special": 8, "exemplari": 8, "consequenti": 8, "damag": 8, "procur": 8, "substitut": 8, "good": 8, "servic": 8, "loss": 8, "profit": 8, "busi": 8, "interrupt": 8, "howev": 8, "caus": 8, "theori": 8, "liabil": 8, "whether": 8, "strict": 8, "tort": 8, "neglig": 8, "otherwis": 8, "aris": 8, "even": 8, "IF": 8, "advis": 8, "possibl": 8, "SUCH": 8, "question": 8, "contact": 8, "francesco": 8, "rizzi": 8, "fnrizzi": 8, "gov": 8, "solvers_nonlinear": [10, 11], "hpp": [10, 11, 12, 13, 14], "core": [10, 11, 12, 13, 14], "spars": [10, 14], "option": [10, 11, 14], "struct": [10, 11], "myproblem": 10, "state_typ": [10, 11, 12, 13, 14], "vectorxd": [10, 11], "residual_typ": [10, 11], "jacobian_typ": [10, 11, 14], "sparsematrix": [10, 14], "doubl": [10, 11, 12, 13, 14], "createst": [10, 11, 12, 13, 14], "const": [10, 11, 12, 13, 14], "setzero": [10, 11], "return": [10, 11, 12, 13, 14], "createresidu": [10, 11], "createjacobian": [10, 11, 14], "void": [10, 11, 12, 13, 14], "residualandjacobian": [10, 11], "re": [10, 11], "std": [10, 11, 12, 13, 14], "jin": [10, 11], "4": [10, 11, 12, 14], "auto": [10, 11, 12, 13, 14], "jac": 10, "coeffref": 10, "incorrect": 10, "entri": 10, "line": 10, "search": 10, "class": [10, 12, 13, 14], "mylinearsolv": 10, "a_t": 10, "typenam": [10, 13, 14], "b_t": 10, "x_t": 10, "public": [10, 12, 13, 14], "solv": [10, 11], "bicgstab": [10, 14], "int": [10, 11, 12, 13, 14], "main": [10, 11, 12, 13, 14], "namespac": [10, 11, 12, 13, 14], "plog": [10, 11], "pnonl": [10, 11], "nonlinearsolv": [10, 11, 14], "logto": [10, 11, 12, 13, 14], "termin": [10, 11, 12, 13, 14], "setverbos": [10, 11], "create_newton_solv": [10, 14], "updatemethod": 10, "updat": [10, 11], "backtrackstrictlydecreasingobject": 10, "setupdatecriterion": [10, 11], "cout": [10, 11, 12, 14], "setprecis": [10, 11], "14": [10, 11], "expect": [10, 12], "93185165": 10, "51763809": 10, "variant": 11, "rosenbrock": 11, "gauss": 11, "newton": 11, "levenberg": 11, "marquardt": 11, "solvers_linear": 11, "myrosenbrocksystem": 11, "scalar_typ": [11, 12, 13, 14], "matrixxd": 11, "r": 11, "j": [11, 14], "x1": 11, "x2": 11, "x3": 11, "x4": 11, "20": 11, "row": [11, 14], "second": [11, 14], "etc": 11, "pl": 11, "linearsolv": [11, 14], "hessian_t": 11, "linear_solver_tag": 11, "householderqr": 11, "linear_solver_t": [11, 14], "x0": 11, "create_gauss_newton_solv": 11, "setstoptoler": [11, 14], "op": 11, "exact": 11, "minimum": 11, "create_levenberg_marquardt_solv": 11, "lmschedule2": 11, "ode_advanc": [12, 13, 14], "ode_steppers_explicit": [12, 13], "templat": [12, 13, 14], "scalartyp": [12, 13, 14], "simplesystem": 12, "independent_variable_typ": [12, 13, 14], "matrix": [12, 13, 14], "rhs_type": [12, 13, 14], "setconst": [12, 13, 14], "createrh": [12, 13, 14], "v": [12, 13, 14], "rh": [12, 13, 14], "timein": [12, 13, 14], "constexpr": [12, 13, 14], "ten": 12, "argc": [12, 13, 14], "char": [12, 13, 14], "argv": [12, 13, 14], "y": [12, 13, 14], "pode": [12, 13, 14], "scheme": [12, 13, 14], "stepschem": [12, 13, 14], "forwardeul": 12, "stepperobj": [12, 13], "create_explicit_stepp": [12, 13], "fix": 12, "dt": [12, 13, 14], "starttim": [12, 13, 14], "advance_n_step": 12, "stepcount": [12, 13, 14], "endl": 12, "until": 12, "advance_to_target_point": [12, 13, 14], "unus": [12, 13, 14], "stepstartat": [12, 13, 14], "stepsiz": [12, 13, 14], "1024": 12, "2048": 12, "3074": 12, "origin": [13, 14], "lorenz": [13, 14], "system": [13, 14], "explicit": 13, "lorenz3": [13, 14], "n_": [13, 14], "rho_": [13, 14], "static_cast": [13, 14], "28": [13, 14], "sigma_": [13, 14], "beta_": [13, 14], "z": [13, 14], "stateobserv": 13, "samplefreq": 13, "myfile_": 13, "io": [13, 14], "samplefreq_": 13, "close": [13, 14], "timetyp": 13, "oper": 13, "get": [13, 14], "size_t": [13, 14], "ext": [13, 14], "sizeof": [13, 14], "reinterpret_cast": [13, 14], "privat": [13, 14], "ofstream": [13, 14], "rungekutta4": 13, "40": [13, 14], "statesamplefreq": 13, "move": [13, 14], "dir": [13, 14], "type": [13, 14], "tutorial2": 13, "ode_eigen_exe2": 13, "implicit": 14, "ode_steppers_implicit": 14, "trip_t": 14, "triplet": 14, "mutabl": 14, "vector": 14, "tripletlist_": 14, "sinc": 14, "jacobain": 14, "fairli": 14, "den": 14, "could": 14, "dens": 14, "allocatetriplet": 14, "setfromtriplet": 14, "begin": 14, "rhsandjacobian": 14, "jopt": 14, "7": 14, "push_back": 14, "third": 14, "i": 14, "problem_typ": 14, "plin": 14, "stepper": 14, "bdf2": 14, "create_implicit_stepp": 14, "fom_jacobian_t": 14, "iter": 14, "13": 14, "myfil": 14, "lambda": 14, "observ": 14, "save": 14, "tutorial3": 14, "ode_eigen_exe3": 14}, "objects": {}, "objtypes": {}, "objnames": {}, "titleterms": {"come": 0, "soon": 0, "build": 1, "instruct": 1, "what": 1, "you": [1, 3, 4, 5, 6], "need": 1, "step": [1, 3, 4, 5, 6], "verifi": 1, "Then": 1, "more": 1, "detail": 1, "depend": 1, "start": 2, "read": 2, "here": 2, "kei": 2, "featur": 2, "outcom": 2, "2d": [3, 4, 5, 6], "swe": [3, 4, 5, 6], "default": [3, 5], "galerkin": [3, 4], "prerequisit": [3, 4, 5, 6], "To": [3, 4, 5, 6], "run": [3, 4, 5, 6], "demo": [3, 4, 5, 6], "script": [3, 4, 5, 6], "below": [3, 4, 5, 6], "must": [3, 4, 5, 6], "insid": [3, 4, 5, 6], "correct": [3, 4, 5, 6], "directori": [3, 4, 5, 6], "workflow": [3, 4, 5, 6], "file": [3, 4, 5, 6], "1": [3, 4, 5, 6, 7, 10, 12], "execut": [3, 5, 6], "fom": [3, 4, 5, 6], "2": [3, 4, 5, 6, 7, 11, 13], "offlin": [3, 4, 5, 6], "rom": [3, 4, 5, 6, 7], "3": [3, 4, 5, 6, 7, 14], "4": [3, 4, 5, 6], "process": [3, 4, 5, 6, 13, 14], "result": [3, 4, 5, 6, 13, 14], "5": 3, "thing": 3, "can": 3, "try": 3, "hyper": [4, 6], "reduc": [4, 6], "exampl": [4, 6], "sampl": [4, 6], "mesh": [4, 6], "accuraci": [4, 6], "runtim": [4, 6], "comparison": [4, 6], "lspg": [5, 6], "pressio": 7, "tutori": [7, 10, 11, 12, 13, 14], "scope": 7, "motiv": 7, "end": 7, "us": 7, "demoapp": 7, "self": 7, "contain": 7, "eigen": 7, "data": 7, "type": 7, "advanc": 7, "topic": 7, "miscellanea": 7, "question": 7, "licens": 8, "nonlinear": [10, 11], "solver": [10, 11], "od": [12, 13, 14]}, "envversion": {"sphinx.domains.c": 2, "sphinx.domains.changeset": 1, "sphinx.domains.citation": 1, "sphinx.domains.cpp": 6, "sphinx.domains.index": 1, "sphinx.domains.javascript": 2, "sphinx.domains.math": 2, "sphinx.domains.python": 3, "sphinx.domains.rst": 2, "sphinx.domains.std": 2, "sphinx.ext.viewcode": 1, "sphinx.ext.intersphinx": 1, "sphinx": 56}})