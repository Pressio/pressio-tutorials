
DOXYFILE = 'Doxyfile-mcss'
MAIN_PROJECT_URL: ""

LINKS_NAVBAR1 = [

  ("<a href=\"md_pages_utils_logger.html\">logger</a>", []),

  ("<a>solvers_nonlinear</a>",
   [
     ("<a href=\"md_pages_solvers_nonlinear_tutorial_0.html\">General options </a>", ),
     ("<a href=\"md_pages_solvers_nonlinear_tutorial_1.html\">Newton-Raphson (known types)</a>", ),
     ("<a href=\"md_pages_solvers_nonlinear_tutorial_2.html\">Newton-Raphson (custom types)</a>", ),
     ("<a href=\"md_pages_solvers_nonlinear_tutorial_3.html\">Gauss-Newton normal equations (known types)</a>", ),
     ("<a href=\"md_pages_solvers_nonlinear_tutorial_4.html\">Gauss-Newton normal equations (custom types)</a>", )
   ]),

  ("<a href=\"md_pages_ode_advancers_advancers.html\">ode_advancers</a>", []),

  ("<a>ode_steppers</a>", #("<a href=\"md_pages_tutorials.html\">Tutorials</a>",
   [
     ("<a href=\"md_pages_ode_steppers_tutorial1.html\">Explicit Time Integration (known types)</a>", ),
     ("<a href=\"md_pages_ode_steppers_tutorial2.html\">Explicit Time Integration (custom types)</a>", ),
     ("<a href=\"md_pages_ode_steppers_tutorial3.html\">Implicit Time Integration (known types)</a>", ),
     ("<a href=\"md_pages_ode_steppers_tutorial4.html\">Implicit Time Integration (custom types)</a>", )
   ]),

  ("<a>rom</a>", #("<a href=\"md_pages_tutorials.html\">Tutorials</a>",
   [
     ("<a href=\"md_pages_rom_tutorial_lin_dec.html\">Linear Decoder (known types)</a>", ),
     ("<a href=\"md_pages_rom_tutorial_lin_dec_custom_types.html\">Linear Decoder (custom types)</a>", ),
   ]),

  # ("<a href=\"md_pages_swe_main.html\">Shallow Water Eq.</a>",
  #  [
  #    ("<a href=\"md_pages_swe_fom.html\">FOM Driver</a>", ),
  #    ("<a href=\"md_pages_swe_lspg.html\">LSPG Driver</a>", ),
  #    ("<a href=\"md_pages_swe_hrlspg.html\">Hyper-reduced LSPG Driver</a>", ),
  #    ("<a href=\"md_pages_swe_endtoend.html\">End-to-end Usecase</a>", )
  #    #("<a href=\"md_pages_swe_endtoend.html\">End-to-end LSPG example</a>", )
  #  ])

  # # demos
  # ("<a href=>Demos</a>", #("<a href=\"md_pages_examples.html\">Full Examples</a>",
  #  [
  #    ("<a href=\"md_pages_examples_example1.html\">Example1</a>", )
  #  ]),

  # ('Classes', 'annotated', []),
  # ('Namespaces', 'namespaces', [])
]

LINKS_NAVBAR2 = []
#   ('Classes', 'annotated', []),
#   ('Namespaces', 'namespaces', [])
# ]

PLUGINS = ['m.htmlsanity', 'm.math', 'm.code', 'm.components', 'm.dot', 'm.images']

SHOW_UNDOCUMENTED = "YES"

FAVICON = 'favicon.ico'

# STYLESHEETS = [
#     'https://fonts.googleapis.com/css?family=Libre+Baskerville:400,400i,700,700i%7CSource+Code+Pro:400,400i,600',
#     '../css/m-light+documentation.compiled.css'
# ]
# THEME_COLOR = '#91cff4'
# FAVICON = 'favicon-light.png'

# STYLESHEETS = [
#     'https://fonts.googleapis.com/css?family=Source+Sans+Pro:400,400i,600,600i%7CSource+Code+Pro:400,400i,600',
#     '../css/m-dark+documentation.compiled.css'
# ]
# THEME_COLOR = '#cb4b16'
# FAVICON = 'favicon-dark.png'
